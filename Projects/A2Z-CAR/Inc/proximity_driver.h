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

TIM_HandleTypeDef ic_handle;
TIM_IC_InitTypeDef ic_ic_init;
int8_t proximity_ic1_init();
int8_t proximity_ic1_init();
int8_t proximity_sensor_trigger_init();
void proximity_send_trigger();
float get_freq();

#endif /* APPLICATIONS_USER_PROXIMITY_DRIVER_H_ */
