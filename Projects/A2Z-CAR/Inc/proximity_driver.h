/*
 * proximity_driver.h
 *
 *  Created on: 2017. aug. 15.
 *      Author: AND
 */

#ifndef APPLICATIONS_USER_PROXIMITY_DRIVER_H_
#define APPLICATIONS_USER_PROXIMITY_DRIVER_H_

#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"

TIM_HandleTypeDef proxim_timer_handle;
TIM_IC_InitTypeDef proxim_timer_ic_init;
int8_t proximity_driver_init();
int8_t proximity_ic1_init();
int8_t proximity_ic1_init();
int8_t proximity_sensor_trigger_init();
void proximity_send_trigger();
int8_t proximity1_exti_init();
int8_t proximity_timer_init();
int8_t proximity_control_thread();


#endif /* APPLICATIONS_USER_PROXIMITY_DRIVER_H_ */
