/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_MATRIX_H
#define __LED_MATRIX_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32746g_discovery_ts.h"
#include "cmsis_os.h"
#include <math.h>
#include "lcd_log.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define LED_MATRIX_COLS		5
#define LED_MATRIX_ROWS		7

/* Exported variables --------------------------------------------------------*/
osMessageQId message_q_id;           // Declare an ID for the message queue


/* Exported functions ------------------------------------------------------- */
void led_matrix_update_thread(void const *argument);
void led_matrix_waterfall_thread(void const *argument);
void led_matrix_ts_thread();
void turn_on_a_led_thread();
#endif /* __LED_MATRIX_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
