#include "wifi_functions.h"
#include "pwm_driver.h"
#include "motor_control.h"

uint8_t remote_ip[] = {10, 27, 99, 89};
uint16_t remote_port = 8002;
uint16_t server_port = 8002;
uint8_t rec_data;
char* modulename;
uint8_t sent_data[] = "Hello big brother board!";
uint16_t rec_len;
uint8_t  mac_addr[6];
uint8_t  ip_addr[4];
uint16_t data_len;
int32_t socket;
uint16_t trials = CONNECTION_TRIAL_MAX;
uint8_t adc_values[9];

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


void wifi_send_thread(void const * argument)
{
	printf("WiFi thread starting... \n");
	while(1) {
		printf("> Trying to connect to server: %d.%d.%d.%d:8002 ...\n",
			   remote_ip[0],
			   remote_ip[1],
			   remote_ip[2],
			   remote_ip[3]);
		while (trials--) {
			if( WIFI_OpenClientConnection(0, WIFI_TCP_PROTOCOL, "TCP_CLIENT", remote_ip, remote_port, 0) == WIFI_STATUS_OK) {
				printf("> TCP Connection opened successfully.\n");
				socket = 0;
			}
		}
		if (!trials) {
			printf("> ERROR : Cannot open Connection\n");
			BSP_LED_On(LED2);
		}
		printf("trying to send data\n");
		if(socket != -1) {
			char buff;
			sprintf("S#1:%d, S#2:%d,S#3:%d,S#4:%d,S#5:%d,S#6:%d,S#7:%d,S#8:%d,S#9:%d\n", adc_values[0], adc_values[1],adc_values[2],adc_values[3],adc_values[4],adc_values[5],adc_values[6],adc_values[7],adc_values[9]);
			if(WIFI_SendData(socket, buff, sizeof(buff), &data_len, WIFI_WRITE_TIMEOUT) != WIFI_STATUS_OK) {
				printf("> ERROR : Failed to send Data.\n");
			} else {
				printf("Data sent\n");
			}
		}
		osDelay(500);
	}

	while (1) {
		/* Delete the thread */
		osThreadTerminate(NULL);
	}
}


//void wifi_receive_thread(void const * argument)
//{
//	while (1) {
//		printf("Starting TCP server...\n");
//		if (WIFI_StartServer(socket, WIFI_TCP_PROTOCOL, "IoT server", server_port) == WIFI_STATUS_OK) {
//			printf("TCP server started\nReceiving data...\n");
//			if (WIFI_ReceiveData(socket, &rec_data, sizeof(rec_data), &data_len, WIFI_READ_TIMEOUT) == WIFI_STATUS_OK) {
//				if (data_len > 0) {
//					if (rec_data == 1) {								// start signal
//						printf("Start signal received\n");
//						motor_pwm_set_duty(25);
//					} else if (rec_data == 0) {							// stop signal
//						printf("Stop signal received\n");
//						if (disable_drive() == -1) {
//							printf("Error: unable to disable drive!\n");
//						}
//					}
//				}
//			} else {
//				printf("No data received\n");
//			}
//			printf("Closing the socket...\n");
//			if (WIFI_StopServer(socket) == WIFI_STATUS_OK) {
//				printf("Socket closed\n");
//			}
//		} else {
//			printf("Failed to start TCP server!\n");
//		}
//		osDelay(10);
//	}
//
//	while (1) {
//		/* Delete the thread */
//		osThreadTerminate(NULL);
//	}
//}


