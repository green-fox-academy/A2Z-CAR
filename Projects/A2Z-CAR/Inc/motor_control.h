/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void pin_init();
void set_direction(int8_t dir);
void stop_drive();
void disable_drive();
void go();
void accelerate();
void decelerate();
void motor_control_thread(void const * argument);

/* Exported constants --------------------------------------------------------*/
uint8_t started;

#endif /* __MOTOR_CONTROL_H */
