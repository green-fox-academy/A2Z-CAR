/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WIFI_FUNCTIONS_H
#define __WIFI_FUNCTIONS_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"
#include "wifi.h"
#include "cmsis_os.h"

typedef struct
{
	uint8_t buff_adc_data[9];
	uint32_t buff_distance;
	uint8_t line_feedback;

} sensor_data;

uint8_t user_command_flag;
uint8_t wifi_flag;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int8_t wifi_init();
void wifi_comm_thread(void const * argument);

#endif /* __WIFI_FUNCTIONS_H */
