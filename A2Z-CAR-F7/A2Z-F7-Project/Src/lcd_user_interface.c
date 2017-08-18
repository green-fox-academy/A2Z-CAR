#include "lcd_user_interface.h"
#include "socket_client.h"

TS_StateTypeDef ts_state;

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
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
	BSP_LCD_FillRect(400, 140, 80, 50);
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillRect(400, 200, 80, 50);


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

void detect_start_stop_thread(void const * argument)
{
	while (1) {
		BSP_TS_GetState(&ts_state);
		if (ts_state.touchDetected) {
			if ((ts_state.touchX[0] > 400) && (ts_state.touchY[0] > 140) && (ts_state.touchY[0] < 190)) {
				move = 1;
				LCD_UsrLog ((char *)"Start command detected\n");

			} else if ((ts_state.touchX[0] > 400) && (ts_state.touchY[0] > 200) && (ts_state.touchY[0] < 250)) {
				move = 0;
				LCD_UsrLog ((char *)"Stop command detected\n");
			}
			osThreadDef(client, socket_client_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
			osThreadCreate (osThread(client), NULL);
			osDelay(2000);
		} else {
			osDelay(10);
		}
	}

	while (1)
		osThreadTerminate(NULL);
}

