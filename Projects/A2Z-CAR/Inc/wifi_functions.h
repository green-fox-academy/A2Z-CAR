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
#define SSID     "A66 Guest"
#define PASSWORD "Hello123"

#define WIFI_WRITE_TIMEOUT 10000
#define WIFI_READ_TIMEOUT  10000
#define CONNECTION_TRIAL_MAX          10

/* Exported functions ------------------------------------------------------- */
void wifi_init();
void wifi_send_thread(void const * argument);

#endif /* __WIFI_FUNCTIONS_H */
