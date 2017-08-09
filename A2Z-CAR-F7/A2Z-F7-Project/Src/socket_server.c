/* Includes ------------------------------------------------------------------*/

#include "socket_server.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>
#include "lcd_user_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void terminate_thread()
{
	while (1)
		osThreadTerminate(NULL);
}

void socket_server_thread(void const *argument)
{
	LCD_UsrLog("Socket server - startup...\n");
	LCD_UsrLog("Socket server - waiting for IP address...\n");

	// Wait for an IP address
	while (!is_ip_ok())
		osDelay(10);
	LCD_UsrLog("Socket server - IP address is ok\n");

	// Create server socket
	int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (server_socket < 0) {
		LCD_ErrLog("Socket server - can't create socket\n");
		terminate_thread();
	}
	LCD_UsrLog("Socket server - socket created\n");

	// Create address structure and bind the socket to it
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_socket, (struct sockaddr*)&(server_addr), sizeof(server_addr)) < 0) {
		LCD_ErrLog("Socket server - can't bind socket\n");
		terminate_thread();
	}
	LCD_UsrLog("Socket server - socket bind ok\n");

	// Start listening
	if (listen(server_socket, SERVER_QUEUE_SIZE) < 0) {
		LCD_ErrLog("Socket server - can't listen\n");
		terminate_thread();
	}
	LCD_UsrLog("Socket server - listening...\n");

	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_socket;
	uint8_t buff[9] = {255, 255, 255, 255, 255, 255, 255, 255, 255};

	while (1) {
		// Accept incoming connections
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
		LCD_UsrLog("Socket server - connection accepted\n");
		// Check the client socket
		if (client_socket < 0) {
			LCD_ErrLog("Socket server - invalid client socket\n");
		} else {
			// Define buffer for incoming message

			int received_bytes = 0;

			// Receive data
			do {

				received_bytes = recv(client_socket, buff, sizeof(buff), 0);
				// Check for error
				if (received_bytes < 0) {
					LCD_ErrLog("Socket server - can't receive\n");
				} else {

					//set LCD user feedback

					for (uint8_t i = 0; i < 9; i++) {
						LCD_UsrLog("S#%d:%d; ", i + 1, buff[i]);
						draw_sensor_data(i, buff[i]);
						osDelay(30);
					}
				}
				osDelay(1000);
				LCD_UsrLog("\n");
				draw_background();
			} while (received_bytes != 9);

			// Close the socket
			closesocket(client_socket);

		LCD_UsrLog("Socket server - connection closed\n");
		}
	}

	// Close socket
	closesocket(server_socket);

	while (1) {
		LCD_UsrLog("Socket server - server socket closed\n");
		osDelay(1000);
	}

	terminate_thread();
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/