#include "lcd_user_interface.h"


void draw_background()
{
	int x = 15;
	int y = 48;
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

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
