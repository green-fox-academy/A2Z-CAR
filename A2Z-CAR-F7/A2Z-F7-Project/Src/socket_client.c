/* Includes ------------------------------------------------------------------*/

#include "socket_client.h"
#include "socket_server.h"
#include "lcd_log.h"
#include "lcd_user_interface.h"
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

void socket_client_thread(void const *argument)
{
	int client_socket;

	// Try to connect to the server
	if (connect_to_server(&client_socket, REMOTE_PORT, REMOTE_IP) == 0) {
		int sent_bytes = send(client_socket, &move, sizeof(move), 0);
		if (sent_bytes == 1) {
			LCD_UsrLog("Socket client - command sent\n");
		}
		closesocket(client_socket);
	} else {
		LCD_UsrLog("Unable to create socket!\n");
	}

	LCD_UsrLog("Socket client - terminating\n");

	while (1) {
		osThreadTerminate(NULL);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
