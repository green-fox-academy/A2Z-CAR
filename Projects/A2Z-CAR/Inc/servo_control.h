/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SERVO_CONTROL_H
#define __SERVO_CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void set_servo_angle(int8_t angle);
void stop();
void set_servo();
void led_init();
void servo_control_thread(void const * argument);

#endif /* __SERVO_CONTROL_H */
