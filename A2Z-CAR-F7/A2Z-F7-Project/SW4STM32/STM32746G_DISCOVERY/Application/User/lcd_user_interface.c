#include "lcd_user_interface.h"


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

}

void draw_sensor_data(int sensor_num, uint16_t radius)
{
	int x = 40 + (sensor_num * 50);
	int y = 73;

	//LCD_UsrLog("radius, %d\n", radius);
	if (radius <= 50) {
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillCircle(x, y, 3);
	} else if ((radius > 50) && (radius <= 150)){
		BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
		BSP_LCD_FillCircle(x, y, 15);
	} else if (radius > 150) {
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillCircle(x, y, 23);
	}

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

}
