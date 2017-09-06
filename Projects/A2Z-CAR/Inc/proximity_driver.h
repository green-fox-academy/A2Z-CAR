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
#include "motor_control.h"

uint8_t object_flag;

int8_t proximity_driver_init();
int8_t proximity_control_thread();

uint32_t distance;
#endif /* APPLICATIONS_USER_PROXIMITY_DRIVER_H_ */
