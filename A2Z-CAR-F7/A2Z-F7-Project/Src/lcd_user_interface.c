#include "lcd_user_interface.h"
#include "socket_server.h"

TS_StateTypeDef ts_state;

void draw_background()
{
	//draw basic background for sensor data visualization
	int x = 15;
	int y = 53;

	for (uint8_t j = 0; j < 9; j++) {
		BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);
		BSP_LCD_FillRect(x, y, 49, 49);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_DrawRect(x, y, 50, 50);
		x += 50;
	}

	x = 40;
	y = 78;
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	for (uint8_t j = 0; j < 9; j++) {
		BSP_LCD_DrawCircle(x, y, 25);
		x += 50;
	}

}

void draw_line_sensor_data(int sensor_num, uint8_t radius, uint8_t line_feedback)
{
	//visualize sensor data using color code
	int x = 40 + (sensor_num * 50);
	int y = 78;

	if (radius <= 25) {
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillCircle(x, y, 5);
	} else if ((radius > 25) && (radius <= 50)){
		BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
		BSP_LCD_FillCircle(x, y, 10);
	} else if ((radius > 50) && (radius <= 100)){
		BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
		BSP_LCD_FillCircle(x, y, 15);
	} else if ((radius > 100) && (radius <= 150)){
		BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
		BSP_LCD_FillCircle(x, y, 20);
	} else if (radius > 150) {
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillCircle(x, y, 24);
	}

}

void draw_proximity_sensor_data(uint32_t distance)
{
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	//object distance feedback coordinates (16, 103, 449, 50)
	if (distance > 400) {
		BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
		BSP_LCD_FillRect(16, 103, 449, 50);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)"no object detected", CENTER_MODE);
	} else if ((distance < 400) && (distance > 200)) {
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_FillRect(16, 103, 449, 50);
		BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
		BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)"object detected, distance ok", CENTER_MODE);
	} else if ((distance < 200) && (distance > 100)) {
		BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
		BSP_LCD_FillRect(16, 103, 449, 50);
		BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
		BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)"object detected, distance ok", CENTER_MODE);
	} else if ((distance < 100) && (distance > 50)) {
		BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
		BSP_LCD_FillRect(16, 103, 449, 50);
		BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
		BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)"object detected, SLOW DOWN", CENTER_MODE);
	} else if ((distance < 50) && (distance > 30)) {
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_FillRect(16, 103, 449, 50);
		BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
		BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)"STOP!", CENTER_MODE);
	} else if ((distance < 30) && (distance > 1)) {
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_FillRect(16, 103, 449, 50);
		BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
		BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)"DANGER", CENTER_MODE);
		osDelay(10);
		BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
		BSP_LCD_FillRect(16, 103, 449, 50);
	} else if (distance == 0) {
		BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
		BSP_LCD_FillRect(16, 103, 449, 50);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)"no object detected", CENTER_MODE);
	}
	BSP_LCD_SetFont(&Font12);
}

void draw_buttons() {
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
	BSP_LCD_FillRect(5, 0, 80, 50); //START Button coordinates (5, 5, 80, 50)
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DrawRect(5, 0, 80, 51); //START Button coordinates (5, 5, 80, 50)
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillRect(395, 0, 80, 50); //STOP Button coordinates (395, 5, 80, 50)
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DrawRect(395, 0, 80, 51); //STOP Button coordinates (395, 5, 80, 50)
	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
	BSP_LCD_DisplayStringAt(20, 18, (uint8_t *)"START", LEFT_MODE);
	BSP_LCD_DisplayStringAt(412, 18, (uint8_t *)"STOP", LEFT_MODE);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DrawRect(15, 102, 450, 51); //object distance feedback coordinates (16, 103, 449, 50)
	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
	BSP_LCD_FillRect(16, 103, 449, 50); //object distance feedback coordinates (16, 103, 449, 50)

}

void detect_start_stop_thread(void const * argument)
{
	move = 0;
	prev = 0;
	while (1) {
		BSP_TS_GetState(&ts_state);
		if (ts_state.touchDetected) {
			if (ts_state.touchY[0] < 56) {
				if (ts_state.touchX[0] < 86) {
					//START Button coordinates (5, 5, 80, 50)
					if (prev == 0) {
						move = 1;
						prev = move;
					}
					LCD_UsrLog ((char *)"Go command detected\n");
					BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
					BSP_LCD_FillRect(5, 5, 80, 50);
					osDelay(50);
					draw_buttons();

				} else if (ts_state.touchX[0] < 166) {
					if (prev != 0) {
						move = 3;
						prev = move;
					}
					LCD_UsrLog ((char *)"Accelerate command detected\n");

				} else if (ts_state.touchX[0] > 394) {
					//STOP Button coordinates (395, 5, 80, 50)
	//				move = -1;
	//				LCD_UsrLog ((char *)"Disable command detected\n");
					move = 0;
					prev = move;
					LCD_UsrLog ((char *)"Stop command detected\n");
					BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
					BSP_LCD_FillRect(395, 5, 80, 50);
					osDelay(50);
					draw_buttons();

				} else if (ts_state.touchX[0] > 314) {
					if (prev != 0) {
						move = 2;
						prev = move;
					}
					LCD_UsrLog ((char *)"Decelerate command detected\n");
				}
			}
			osDelay(500);
		} else {
			osDelay(10);
		}
	}

	terminate_thread();
}

