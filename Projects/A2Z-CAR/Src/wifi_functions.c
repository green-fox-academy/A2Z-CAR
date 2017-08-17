#include "wifi_functions.h"
#include "pwm_driver.h"
#include "motor_control.h"

uint8_t RemoteIP[] = {10, 27, 99, 91};
uint8_t RxData [1];
char* modulename;
uint8_t TxData[] = "Hello big brother board!";
uint16_t RxLen;
uint8_t  MAC_Addr[6];
uint8_t  IP_Addr[4];
uint16_t Datalen;
int32_t Socket = -1;
uint8_t adc_values[9];

int8_t wifi_init()
{
	uint16_t Trials = CONNECTION_TRIAL_MAX;

	/*Initialize  WIFI module */
	if(WIFI_Init() ==  WIFI_STATUS_OK) {
	    printf("> WIFI Module Initialized.\n");
	    if(WIFI_GetMAC_Address(MAC_Addr) == WIFI_STATUS_OK) {
	        printf("> es-wifi module MAC Address : %X:%X:%X:%X:%X:%X\n",
	               MAC_Addr[0],
	               MAC_Addr[1],
	               MAC_Addr[2],
	               MAC_Addr[3],
	               MAC_Addr[4],
	               MAC_Addr[5]);
	    } else {
	    	printf("> ERROR : CANNOT get MAC address\n");
	        BSP_LED_On(LED2);
	    }

	    if( WIFI_Connect(SSID, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK) {
	    	printf("> es-wifi module connected \n");
	    	if(WIFI_GetIP_Address(IP_Addr) == WIFI_STATUS_OK) {
	    		printf("> es-wifi module got IP Address : %d.%d.%d.%d\n",
	    		               IP_Addr[0],
	    		               IP_Addr[1],
	    		               IP_Addr[2],
	    		               IP_Addr[3]);

				printf("> Trying to connect to Server: %d.%d.%d.%d:8002 ...\n",
					   RemoteIP[0],
					   RemoteIP[1],
					   RemoteIP[2],
					   RemoteIP[3]);
		        while (Trials--) {
		        	if( WIFI_OpenClientConnection(0, WIFI_TCP_PROTOCOL, "TCP_CLIENT", RemoteIP, 8002, 0) == WIFI_STATUS_OK) {
						printf("> TCP Connection opened successfully.\n");
						Socket = 0;
		        	}
		        }
		        if(!Trials) {
		            printf("> ERROR : Cannot open Connection\n");
		            BSP_LED_On(LED2);
		        }
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
	printf("wifi thread starting... \n");
	while(1) {
		printf("trying to send data\n");
		if(Socket != -1) {
			char buff;
			sprintf("S#1:%d, S#2:%d,S#3:%d,S#4:%d,S#5:%d,S#6:%d,S#7:%d,S#8:%d,S#9:%d\n", adc_values[0], adc_values[1],adc_values[2],adc_values[3],adc_values[4],adc_values[5],adc_values[6],adc_values[7],adc_values[9]);
			if(WIFI_SendData(Socket, buff, sizeof(buff), &Datalen, WIFI_WRITE_TIMEOUT) != WIFI_STATUS_OK) {
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


void wifi_receive_thread(void const * argument)
{
	while (1) {
		if(Socket != -1) {
			if(WIFI_ReceiveData(Socket, RxData, sizeof(RxData), &Datalen, WIFI_READ_TIMEOUT) == WIFI_STATUS_OK) {
				if (Datalen > 0) {
					if (RxData[0] == 1) {									// start signal
						printf("Start signal received\n");
						motor_pwm_set_duty(15);
					} else if (RxData[0] == 0) {							// stop signal
						printf("Stop signal received\n");
						if (disable_drive() == -1) {
							printf("Error: unable to disable drive!\n");
						}
					}
				}
			} else {
				printf("Error: failed to receive data!\n");
			}
		} else {
			printf("Socket error!\n");
		}
		osDelay(10);
	}

	while (1) {
		/* Delete the thread */
		osThreadTerminate(NULL);
	}
}


