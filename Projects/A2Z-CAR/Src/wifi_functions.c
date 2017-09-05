#include "wifi_functions.h"
#include "pwm_driver.h"
#include "motor_control.h"
#include "adc_driver.h"
#include "main.h"
#include "proximity_driver.h"

#define SSID     "A66 Guest"
#define PASSWORD "Hello123"
#define WIFI_WRITE_TIMEOUT 1000
#define WIFI_READ_TIMEOUT  1000

uint8_t remote_ip[] = {10, 27, 99, 194};
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
#ifdef DEBUG_MODE
		        printf("> es-wifi module MAC Address : %X:%X:%X:%X:%X:%X\n",
	               mac_addr[0],
	               mac_addr[1],
	               mac_addr[2],
	               mac_addr[3],
	               mac_addr[4],
	               mac_addr[5]);
#endif
	    } else {
#ifdef DEBUG_MODE
	    	printf("> ERROR : CANNOT get MAC address\n");
#endif
	        BSP_LED_On(LED2);
	    }
	    // connect to the network
	    if( WIFI_Connect(SSID, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK) {
#ifdef DEBUG_MODE
	    	printf("> es-wifi module connected \n");
#endif
	    	if(WIFI_GetIP_Address(ip_addr) == WIFI_STATUS_OK) {
#ifdef DEBUG_MODE
	    		printf("> es-wifi module got IP Address : %d.%d.%d.%d\n",
	    		               ip_addr[0],
	    		               ip_addr[1],
	    		               ip_addr[2],
	    		               ip_addr[3]);
#endif
			} else {
#ifdef DEBUG_MODE
		        printf("> ERROR : es-wifi module CANNOT get IP address\n");
#endif
		        BSP_LED_On(LED2);
		    }
		} else {
#ifdef DEBUG_MODE
	        printf("> ERROR : es-wifi module NOT connected\n");
#endif
	        BSP_LED_On(LED2);
	    }
	} else {
#ifdef DEBUG_MODE
	    printf("> ERROR : WIFI Module cannot be initialized.\n");
#endif
	    BSP_LED_On(LED2);
	    return -1;
	}

	return 0;
}

void wifi_comm_thread(void const * argument)
{
	uint32_t socket = 0;
	uint8_t started = 0;
	uint8_t connected = 0;
	sensor_data buff;

	while(1) {
#ifdef DEBUG_MODE
		printf("Trying to connect to server: %d.%d.%d.%d:8002 ...\n",
			remote_ip[0],
			remote_ip[1],
			remote_ip[2],
			remote_ip[3]);
#endif
		if (WIFI_OpenClientConnection(socket, WIFI_TCP_PROTOCOL, "TCP_CLIENT", remote_ip, remote_port, 0) == WIFI_STATUS_OK) {
#ifdef DEBUG_MODE
			printf("TCP connection opened successfully\n");
#endif
			connected = 1;
#ifdef DEBUG_MODE
			printf("Trying to send data\n");
#endif
			while (connected) {
				for (int i = 0; i < 9; i++) {
					buff.buff_adc_data[i] = adc_values[i];
				}

				buff.buff_distance = distance;
				buff.line_feedback = 1;

				if (WIFI_SendData(socket, &buff, sizeof(buff), &data_len, WIFI_WRITE_TIMEOUT) != WIFI_STATUS_OK) {
#ifdef DEBUG_MODE
					printf("> ERROR : Failed to send data\n");
#endif
					connected = 0;
					if (started == 1) {
#ifdef DEBUG_MODE
						printf("Stopping car\n");
#endif
						stop_drive();
						started = 0;
					}
				} else {
#ifdef DEBUG_MODE
					printf("Data sent\n");
#endif
					if (WIFI_ReceiveData(socket, &rec_data, sizeof(rec_data), &data_len, WIFI_READ_TIMEOUT) == WIFI_STATUS_OK) {
						if (data_len > 0) {
							if (rec_data == 1) {				// go signal
#ifdef DEBUG_MODE
								printf("Go signal received\n");
#endif
								if (started == 0) {
#ifdef DEBUG_MODE
									printf("Starting car\n");
#endif
									go();
									started = 1;
								}
							} else if (rec_data == 0) {			// stop signal
#ifdef DEBUG_MODE
								printf("Stop signal received\n");
#endif
								if (started == 1) {
#ifdef DEBUG_MODE
									printf("Stopping car\n");
#endif
									stop_drive();
									started = 0;
								}
							} else if (rec_data == 3) {			// accelerate signal
#ifdef DEBUG_MODE
								printf("Accelerate signal received\n");
#endif
								if (started == 1) {
									accelerate();
								}
							} else if (rec_data == 2) {			// decelerate signal
#ifdef DEBUG_MODE
								printf("Decelerate signal received\n");
#endif
								if (started == 1) {
									decelerate();
								}
							} else if (rec_data == -1) {		// disable signal
#ifdef DEBUG_MODE
								printf("Disable signal received\n");
#endif
								disable_drive();
								terminate_thread();
							}

						} else {
#ifdef DEBUG_MODE
							printf("No data\n");
#endif
//							if (started == 1) {
//								printf("Stopping car\n");
//								stop_drive();
//								started = 0;
//							}
						}

					} else {
#ifdef DEBUG_MODE
						printf("Nothing received\n");
#endif
						connected = 0;
						if (started == 1) {
#ifdef DEBUG_MODE
							printf("Stopping car\n");
#endif
							stop_drive();
							started = 0;
						}
					}
				}

				osDelay(20);
			}

			WIFI_CloseClientConnection(socket);

		} else {
#ifdef DEBUG_MODE
			printf("> ERROR : Cannot open connection\n");
#endif
			if (started == 1) {
#ifdef DEBUG_MODE
				printf("Stopping car\n");
#endif
				stop_drive();
				started = 0;
			}
		}

		osDelay(500);
	}

	terminate_thread();
}
