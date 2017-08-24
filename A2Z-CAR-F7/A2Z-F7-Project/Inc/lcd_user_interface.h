/*
 * lcd_user_interface.h
 *
 *  Created on: 2017. aug. 8.
 *      Author: AND
 */

#ifndef APPLICATION_USER_LCD_USER_INTERFACE_H_
#define APPLICATION_USER_LCD_USER_INTERFACE_H_

#include "stm32f7xx_hal.h"
#include "stm32746g_discovery_ts.h"
#include "cmsis_os.h"
#include <math.h>
#include "lcd_log.h"

//#define     YWINDOW_MIN         9

int8_t move;

void draw_background();
void draw_sensor_data(int sensor_num, uint8_t radius);
void detect_start_stop_thread();



#endif /* APPLICATION_USER_LCD_USER_INTERFACE_H_ */