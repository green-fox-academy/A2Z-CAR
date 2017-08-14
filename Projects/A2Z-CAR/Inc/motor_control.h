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
float pi_control();
void motor_control_thread(void const * argument);

/* Exported constants --------------------------------------------------------*/

#endif /* __MOTOR_CONTROL_H */
