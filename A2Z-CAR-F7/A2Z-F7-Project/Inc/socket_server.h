/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOCKET_SERVER_H
#define __SOCKET_SERVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32746g_discovery_ts.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define SERVER_PORT			1234
#define SERVER_QUEUE_SIZE	2
#define SERVER_BUFF_LEN		255
#define CLIENT_BUFF_LEN		SERVER_BUFF_LEN

/* Exported functions ------------------------------------------------------- */
void socket_server_thread(void const *argument);

#endif /* __SOCKET_SERVER_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
