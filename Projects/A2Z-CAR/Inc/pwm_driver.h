/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PWM_DRIVER_H
#define __PWM_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
TIM_HandleTypeDef servo_pwm_handle;
TIM_HandleTypeDef motor_pwm_handle;
TIM_HandleTypeDef proxi_pwm_handle;
TIM_OC_InitTypeDef servo_pwm_oc_init;
TIM_OC_InitTypeDef motor_pwm_oc_init;
TIM_OC_InitTypeDef proxi_pwm_oc_init;


GPIO_InitTypeDef GPIO_InitDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int8_t servo_pwm_init();
int8_t motor_pwm_init();
void servo_pwm_set_duty(float duty);
void motor_pwm_set_duty(float duty);


#endif /* __PWM_DRIVER_H */
