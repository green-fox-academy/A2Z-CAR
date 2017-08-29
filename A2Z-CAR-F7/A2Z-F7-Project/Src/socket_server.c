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

typedef struct
{
	uint8_t buff_adc_data[9];
	uint32_t buff_distance;
} sensor_data;

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


	while (1) {
		// Accept incoming connections
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
		LCD_UsrLog("Socket server - connection accepted\n");
		// Check the client socket
		if (client_socket < 0) {
			LCD_ErrLog("Socket server - invalid client socket\n");
		} else {
			// Define buffer for incoming message
			sensor_data buff;

			int received_bytes = 0;

			// Receive data
			while (1) {
				received_bytes = recv(client_socket, &buff, sizeof(buff), 0);
				// Check for error
				if (received_bytes < 1) {
					LCD_ErrLog("Socket server - can't receive\n");
					break;
				}

				//set LCD user feedback
				draw_background();
					for (uint8_t i = 0; i < 9; i++) {
						LCD_UsrLog("S#%d:%d; ", i + 1, buff.buff_adc_data[i]);
						draw_sensor_data(i, buff.buff_adc_data[i], buff.buff_distance);
					}
				LCD_UsrLog("\nSocket server - data received\n");

				int sent_bytes = 0;
				sent_bytes = send(client_socket, &move, sizeof(move), 0);
				if (sent_bytes < 1) {
					LCD_ErrLog("Socket server - can't send\n");
					break;
				}
				LCD_UsrLog("Socket server - data sent\n");

				osDelay(20);
			}
		}
		// If not connected, close the last socket, wait a little bit and then try to reconnect
		closesocket(client_socket);
		LCD_UsrLog("Socket server - connection closed\n");
		osDelay(10);
		LCD_UsrLog("Socket server - listening...\n");
	}
	terminate_thread();
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
