#include "wifi_functions.h"
#include "pwm_driver.h"
#include "motor_control.h"
#include "adc_driver.h"
#include "main.h"
#include "proximity_driver.h"
#include "servo_control.h"

#define SSID     "A66 Guest"
#define PASSWORD "Hello123"
#define WIFI_WRITE_TIMEOUT 100
#define WIFI_READ_TIMEOUT  100

uint8_t remote_ip[] = {10, 27, 99, 70};
uint16_t remote_port = 8002;
int8_t rec_data;
uint8_t  mac_addr[6];
uint8_t  ip_addr[4];
uint16_t data_len;

int8_t wifi_init()
{
	/*Initialize  WIFI module */
	if(WIFI_Init() ==  WIFI_STATUS_OK) {
	    printf("> WIFI Module Initialized.\n");
	    if(WIFI_GetMAC_Address(mac_addr) == WIFI_STATUS_OK) {
	        printf("> es-wifi module MAC Address : %X:%X:%X:%X:%X:%X\n",
	               mac_addr[0],
	               mac_addr[1],
	               mac_addr[2],
	               mac_addr[3],
	               mac_addr[4],
	               mac_addr[5]);
	    } else {
	    	printf("> ERROR : CANNOT get MAC address\n");
	        BSP_LED_On(LED2);
	    }
	    // connect to the network
	    if( WIFI_Connect(SSID, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK) {
	    	printf("> es-wifi module connected \n");
	    	if(WIFI_GetIP_Address(ip_addr) == WIFI_STATUS_OK) {
	    		printf("> es-wifi module got IP Address : %d.%d.%d.%d\n",
	    		               ip_addr[0],
	    		               ip_addr[1],
	    		               ip_addr[2],
	    		               ip_addr[3]);
			} else {
		        printf("> ERROR : es-wifi module CANNOT get IP address\n");
		        BSP_LED_On(LED2);
		    }
		} else {
	        printf("> ERROR : es-wifi module NOT connected\n");
	        BSP_LED_On(LED2);
	    }
	} else {
	    printf("> ERROR : WIFI Module cannot be initialized.\n");
	    BSP_LED_On(LED2);
	    return -1;
	}

	return 0;
}

void wifi_comm_thread(void const * argument)
{
	uint32_t socket = 0;
	sensor_data buff;
	user_command_flag = 0;
	wifi_flag = 0;

	while(1) {
//		printf("Trying to connect to server: %d.%d.%d.%d:8002 ...\n",
//			remote_ip[0],
//			remote_ip[1],
//			remote_ip[2],
//			remote_ip[3]);
		if (WIFI_OpenClientConnection(socket, WIFI_TCP_PROTOCOL, "TCP_CLIENT", remote_ip, remote_port, 0) == WIFI_STATUS_OK) {
//			printf("TCP connection opened successfully\n");
//			printf("Trying to send data\n");
			while (1) {
				for (int i = 0; i < 9; i++) {
					buff.buff_adc_data[i] = adc_values[i];
				}

				buff.buff_distance = distance;
				buff.line_feedback = line_flag;

				if (WIFI_SendData(socket, &buff, sizeof(buff), &data_len, WIFI_WRITE_TIMEOUT) != WIFI_STATUS_OK) {
					wifi_flag = 20;
					printf("> ERROR : Failed to send data\n");
					break;

				} else {
					wifi_flag = 10;
//					printf("Data sent\n");

					if (WIFI_ReceiveData(socket, &rec_data, sizeof(rec_data), &data_len, WIFI_READ_TIMEOUT) == WIFI_STATUS_OK) {
						if (data_len > 0) {
//							printf("Data received\n");
							if (rec_data == 1) {				// go signal
//								printf("Go signal received\n");
								user_command_flag = 10;
							} else if (rec_data == 3) {			// accelerate signal
								printf("Accelerate signal received\n");
								user_command_flag = 30;
							} else if (rec_data == 2) {			// decelerate signal
								printf("Decelerate signal received\n");
								user_command_flag = 20;
							}

						} else {
//							printf("No signal\n");
							user_command_flag = 40;
						}

					} else {
						wifi_flag = 20;
						printf("No connection\n");
						break;
					}
				}

//				printf("                                                                                                          started: %d\n", started);
				osDelay(10);
			}

			WIFI_CloseClientConnection(socket);

		} else {
			wifi_flag = 20;
			printf("> ERROR : Cannot open connection\n");
		}

		osDelay(1000);
	}

	terminate_thread();
}
