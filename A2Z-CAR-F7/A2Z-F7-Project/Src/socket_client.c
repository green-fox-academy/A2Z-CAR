/* Includes ------------------------------------------------------------------*/

#include "socket_client.h"
#include "socket_server.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

int connect_to_server(int *client_sock, uint16_t server_port, char *server_ip);
int send_data(int *socket);
/* Private functions ---------------------------------------------------------*/


int connect_to_server(int *client_sock, uint16_t server_port, char *server_ip)
{
	// Creating client socket
	(*client_sock) = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	if (*client_sock < 0) {
		LCD_ErrLog("Socket client - can't create socket\n");
		return -1;
	}

	// Creating server address structure
	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(server_port);
	addr_in.sin_addr.s_addr = inet_addr(server_ip);

	// Connecting the client socket to the server
	int connect_retval = connect(*client_sock, (struct sockaddr *)&addr_in, sizeof(addr_in));
	if (connect_retval < 0) {
		LCD_ErrLog("Socket client - can't connect to server\n");
		return -1;
	}
	else {
		LCD_UsrLog("Socket client - connected to server\n");
		return 0;
	}
}

int send_data(int *socket)
{
	char buff[CLIENT_BUFF_LEN];

	// Send the data
	int sent_bytes = send(*socket, buff, CLIENT_BUFF_LEN, 0);

	if (sent_bytes < 0)
		LCD_ErrLog("Socket client - can't send data\n");

	// Close the string
	buff[sent_bytes] = '\0';

	// Print message
	LCD_UsrLog("Socket server - message:");
	LCD_UsrLog(buff);
	LCD_UsrLog("\n");

	return sent_bytes;
}

void socket_client_thread(void const *argument)
{
	LCD_UsrLog("Socket client - startup...\n");
	LCD_UsrLog("Socket client - waiting for IP address...\n");

	while (1) {
		// Wait for an IP address
		while (!is_ip_ok())
			osDelay(10);

		int connected = 0;
		int client_socket;

		// Try to connect to the server
		if (connect_to_server(&client_socket, SERVER_PORT, CLIENT_SERVER_IP) == 0)
			connected = 1;

		// If connected
		while (connected) {

			if (send_data(&client_socket) <= 0)
				connected = 0;
			osDelay(CLIENT_SEND_INTERVAL);
		}

		// If not connected close the last socket and wait a little bit and then try to reconnect
		closesocket(client_socket);
		osDelay(CLIENT_RECONNECT_INTERVAL);
	}

	 while (1) {
	    /* Delete the Thread */
	    osThreadTerminate(NULL);
	  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
