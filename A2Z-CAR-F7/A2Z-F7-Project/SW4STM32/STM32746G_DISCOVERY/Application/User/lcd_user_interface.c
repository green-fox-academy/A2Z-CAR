#include "lcd_user_interface.h"


void draw_background()
{

	//draw basic background for sensor data visualization
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

}

void draw_sensor_data(int sensor_num, uint8_t radius)
{
	//visualize sensor data using color code
	int x = 40 + (sensor_num * 50);
	int y = 73;

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
