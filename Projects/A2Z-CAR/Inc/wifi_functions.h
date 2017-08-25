/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WIFI_FUNCTIONS_H
#define __WIFI_FUNCTIONS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"
#include "wifi.h"
#include "cmsis_os.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int8_t wifi_init();
void wifi_comm_thread(void const * argument);

#endif /* __WIFI_FUNCTIONS_H */
