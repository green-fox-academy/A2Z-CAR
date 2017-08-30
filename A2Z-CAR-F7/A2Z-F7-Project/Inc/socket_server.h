/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOCKET_SERVER_H
#define __SOCKET_SERVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32746g_discovery_ts.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define SERVER_PORT			8002
#define SERVER_QUEUE_SIZE	2
#define SERVER_BUFF_LEN		255
#define CLIENT_BUFF_LEN		SERVER_BUFF_LEN

typedef struct
{
	uint8_t buff_adc_data[9];
	uint16_t buff_distance;

} sensor_data;

/* Exported functions ------------------------------------------------------- */
void terminate_thread();
void socket_server_thread(void const *argument);

#endif /* __SOCKET_SERVER_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
