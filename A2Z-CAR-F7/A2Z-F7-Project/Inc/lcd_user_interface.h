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
int8_t prev;

void draw_background();
void draw_buttons();
void detect_start_stop_thread();
void draw_line_sensor_data(int sensor_num, uint8_t radius, uint8_t line_feedback);
void draw_proximity_sensor_data(uint32_t distance);



#endif /* APPLICATION_USER_LCD_USER_INTERFACE_H_ */
