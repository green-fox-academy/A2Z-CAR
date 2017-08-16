#include "lcd_user_interface.h"

TS_StateTypeDef ts_state;
int8_t start_car = 0;
int8_t stop_car = 0;


void draw_background()
{
	int x = 15;
	int y = 48;
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);

	for (uint8_t j = 0; j < 9; j++) {
		BSP_LCD_FillRect(x, y, 49, 49);
		x += 50;
	}

	x = 40;
	y = 73;
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	for (uint8_t j = 0; j < 9; j++) {
		BSP_LCD_DrawCircle(x, y, 25);
		x += 50;
	}
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
	BSP_LCD_FillRect(400, 140, 80, 50);
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillRect(400, 200, 80, 50);


}

void draw_sensor_data(int sensor_num, uint8_t radius)
{
	int x = 40 + (sensor_num * 50);
	int y = 73;


	//LCD_UsrLog("radius, %d\n", radius);
	if (radius <= 10) {
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillCircle(x, y, 5);
	} else if ((radius > 10) && (radius <= 20)){
		BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
		BSP_LCD_FillCircle(x, y, 10);
	} else if ((radius > 20) && (radius <= 30)){
		BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
		BSP_LCD_FillCircle(x, y, 15);
	} else if ((radius > 30) && (radius <= 40)){
		BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
		BSP_LCD_FillCircle(x, y, 20);
	} else if (radius > 40) {
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillCircle(x, y, 25);
	}

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

}

void detect_start_stop_command()
{
	while(1){
		BSP_TS_GetState(&ts_state);

		if ((ts_state.touchX[0] > 400) && (ts_state.touchY[0] > 140) && (ts_state.touchY[0] < 190)) {
			start_car = 1;
			LCD_UsrLog ((char *)"Start command detected\n");

		} else if ((ts_state.touchX[0] > 400) && (ts_state.touchY[0] > 200) && (ts_state.touchY[0] < 250)) {
			stop_car = 1;
			LCD_UsrLog ((char *)"Stop command detected\n");

		} else {
			osDelay(100);
		}
	}

	while (1)
		osThreadTerminate(NULL);
}

