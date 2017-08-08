/* Includes ------------------------------------------------------------------*/
#include "led_matrix.h"



/* Private typedef -----------------------------------------------------------*/

typedef struct {
	int32_t x;
	int32_t y;
} coordinate_t;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

TS_StateTypeDef ts_state;
coordinate_t coordinate1;
coordinate_t coordinate2;
coordinate_t touch_areas[LED_MATRIX_ROWS][LED_MATRIX_COLS];

uint8_t it_was_click = 0;

// Each LED state is stored in this 2D array
GPIO_PinState led_matrix_state[LED_MATRIX_ROWS][LED_MATRIX_COLS] = {
		{GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
		{GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
		{GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
		{GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
		{GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
		{GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
		{GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET}
};


// Mutex definition
osMutexDef(LED_MATRIX_MUTEX_DEF);

// Mutex global variable
osMutexId led_matrix_mutex_id;

/* Private function prototypes -----------------------------------------------*/
void led_matrix_set(uint8_t row, uint8_t col, uint8_t state);
void draw_background();
void define_touch_areas();
void GPIOs_init();
void clear_led_panel();

/* Private functions ---------------------------------------------------------*/

// TODO:
// Write this function!
void led_matrix_set(uint8_t row, uint8_t col, uint8_t state)
{
	// TODO:
	// Wait for the mutex
	osMutexWait(led_matrix_mutex_id, osWaitForever);

	// TODO:
	// Change the LED in the selected row and col to the specified state
	// Use the led_matrix_state 2D array variable!
	led_matrix_state[row][col] = state;

	// TODO:
	// Release the mutex
	osMutexRelease(led_matrix_mutex_id);

}

// TODO:
// Write this function!
void led_matrix_update_thread(void const *argument)
{
	GPIOs_init();

	// TODO:
	// Create a mutex

	led_matrix_mutex_id = osMutexCreate(osMutex(LED_MATRIX_MUTEX_DEF));
	// Use the LED_MATRIX_MUTEX_DEF

	LCD_UsrLog("led_matrix - initialized\n");
	while (!is_ip_ok());
	// Infinite loop
	while (1) {

		if ((BSP_PB_GetState(BUTTON_KEY)) == GPIO_PIN_SET) {
			draw_background();
			clear_led_panel();
		}


		// TODO:
		// Implement the led matrix updater functionality

		// Step 1:
		// Iterate through every row

		for (int i = 0; i < LED_MATRIX_ROWS; i++) {

			// Step 2:
			// Wait for the mutex
			osMutexWait(led_matrix_mutex_id, osWaitForever);
			if (i == 0) {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); // ROW 1
			} else if (i == 1) {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); // ROW 2
			} else if (i == 2) {
				HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET); // ROW 3
			} else if (i == 3) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // ROW 4
			} else if (i == 4) {
				HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET); // ROW 5
			} else if (i == 5) {
				HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_RESET); // ROW 6
			} else if (i == 6) {
				HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET); // ROW 7
			}
			// Step 3:
			// Turn on the column
			// Step 4:
			// Turn on the leds in that column

			for (int z = 0; z < LED_MATRIX_COLS; z++) {
				if (z == 0) {
					HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, led_matrix_state[i][z]); //COL 1
				} else if (z == 1) {
					HAL_GPIO_WritePin(GPIOI, GPIO_PIN_2, led_matrix_state[i][z]); //COL 2
				} else if (z == 2) {
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, led_matrix_state[i][z]); //COL 3
				} else if (z == 3) {
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, led_matrix_state[i][z]); //COL 4
				} else if (z == 4) {
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, led_matrix_state[i][z]); //COL 5
				}
			}

			// Step 5:
			// Release the mutex
			osMutexRelease(led_matrix_mutex_id);

			// Step 6:
			// Delay
			osDelay(10);
			// Step 7:
			// Turn off the col

			if (i == 0) {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // ROW 1
			} else if (i == 1) {
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // ROW 2
			} else if (i == 2) {
				HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET); // ROW 3
			} else if (i == 3) {
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // ROW 4
			} else if (i == 4) {
				HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET); // ROW 5
			} else if (i == 5) {
				HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET); // ROW 6
			} else if (i == 6) {
				HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET); // ROW 7
			}
		}
	}

	// Terminating thread
	while (1) {
		LCD_ErrLog("led_matrix - terminating...\n");
		osThreadTerminate(NULL);
	}
}

// This thread is a waterfall type animation
void led_matrix_waterfall_thread(void const *argument)
{
	uint32_t speed = 0;
	osEvent event = osMessageGet(message_q_id, osWaitForever);
	while (!is_ip_ok());
	while (1) {
		 if (event.status == osEventMessage){
			 speed = event.value.v / 32;
		 }

		for (uint8_t r = 0; r < LED_MATRIX_ROWS; r++) {
			for (uint8_t c = 0; c < LED_MATRIX_COLS; c++) {
				led_matrix_set(r, c, 1);
				osDelay(speed);
				led_matrix_set(r, c, 0);
			}
		}
	}

	while (1) {
		LCD_ErrLog("led_matrix_waterfall - terminating...\n");
		osThreadTerminate(NULL);
	}
}

void GPIOs_init()
{
	// TODO:
	// Initialize the pins as outputs and the led_matrix_state 2D array

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();

	GPIO_InitTypeDef row_04;
	row_04.Pin = GPIO_PIN_4;
	row_04.Mode = GPIO_MODE_OUTPUT_PP;
	row_04.Pull = GPIO_NOPULL;
	row_04.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &row_04);

	GPIO_InitTypeDef row_02;
	row_02.Pin = GPIO_PIN_6;
	row_02.Mode = GPIO_MODE_OUTPUT_PP;
	row_02.Pull = GPIO_NOPULL;
	row_02.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOC, &row_02);

	GPIO_InitTypeDef row_01;
	row_01.Pin = GPIO_PIN_7;
	row_01.Mode = GPIO_MODE_OUTPUT_PP;
	row_01.Pull = GPIO_NOPULL;
	row_01.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOC, &row_01);

	GPIO_InitTypeDef row_03;
	row_03.Pin = GPIO_PIN_6;
	row_03.Mode = GPIO_MODE_OUTPUT_PP;
	row_03.Pull = GPIO_NOPULL;
	row_03.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOG, &row_03);

	GPIO_InitTypeDef row_05;
	row_05.Pin = GPIO_PIN_7;
	row_05.Mode = GPIO_MODE_OUTPUT_PP;
	row_05.Pull = GPIO_NOPULL;
	row_05.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOG, &row_05);

	GPIO_InitTypeDef row_07;
	row_07.Pin = GPIO_PIN_6;
	row_07.Mode = GPIO_MODE_OUTPUT_PP;
	row_07.Pull = GPIO_NOPULL;
	row_07.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOH, &row_07);

	GPIO_InitTypeDef row_06;
	row_06.Pin = GPIO_PIN_0;
	row_06.Mode = GPIO_MODE_OUTPUT_PP;
	row_06.Pull = GPIO_NOPULL;
	row_06.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOI, &row_06);

	GPIO_InitTypeDef col_01;
	col_01.Pin = GPIO_PIN_3;
	col_01.Mode = GPIO_MODE_OUTPUT_PP;
	col_01.Pull = GPIO_NOPULL;
	col_01.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOI,  &col_01);

	GPIO_InitTypeDef col_03;
	col_03.Pin = GPIO_PIN_15;
	col_03.Mode = GPIO_MODE_OUTPUT_PP;
	col_03.Pull = GPIO_NOPULL;
	col_03.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA,  &col_03);

	GPIO_InitTypeDef col_04;
	col_04.Pin = GPIO_PIN_8;
	col_04.Mode = GPIO_MODE_OUTPUT_PP;
	col_04.Pull = GPIO_NOPULL;
	col_04.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA,  &col_04);

	GPIO_InitTypeDef col_05;
	col_05.Pin = GPIO_PIN_15;
	col_05.Mode = GPIO_MODE_OUTPUT_PP;
	col_05.Pull = GPIO_NOPULL;
	col_05.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB,  &col_05);

	GPIO_InitTypeDef col_02;
	col_02.Pin = GPIO_PIN_2;
	col_02.Mode = GPIO_MODE_OUTPUT_PP;
	col_02.Pull = GPIO_NOPULL;
	col_02.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOI,  &col_02);

	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET);

	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

}

void draw_background()
{
	BSP_LCD_Clear(LCD_COLOR_DARKYELLOW);

	int x = 0;
	int y = 10;
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	for (uint8_t i = 0; i < 5; i++) {

		for (uint8_t j = 0; j < 7; j++) {
			BSP_LCD_FillRect(x, y, 48, 48);
			x += 50;

		}
		x = 0;
		y += 50;

	}
	for (int c = 0; c < LED_MATRIX_COLS; c++) {
		for (int r = 0; r <  LED_MATRIX_ROWS; r++) {
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			BSP_LCD_FillCircle((touch_areas[r][c].x), (touch_areas[r][c].y), 15);

		}
	}
}

void define_touch_areas()
{

	int32_t x = 25;
	int32_t y = 35;
	int r = 0;
	int c = 0;
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	for (uint8_t i = 0; i < LED_MATRIX_COLS; i++) {
		for (uint8_t j = 0; j < LED_MATRIX_ROWS; j++) {
			touch_areas[r][c].x = x;
			touch_areas[r][c].y = y;
			x += 50;
			r++;

		}
		x = 25;
		r = 0;
		y += 50;
		c++;
	}
}

void led_matrix_ts_thread()
{
	while (!is_ip_ok());
	osDelay(50);
	uint8_t first_touch_detected_flag = 0;
	uint8_t possible_click_event = 0;
	define_touch_areas();
	draw_background();


	while (1){
		if ((BSP_PB_GetState(BUTTON_KEY)) == GPIO_PIN_SET) {
			draw_background();
			clear_led_panel();
		}

		osDelay(5);
		BSP_TS_GetState(&ts_state);
		osDelay(5);

		if (ts_state.touchDetected) {
			if (!first_touch_detected_flag) {
				first_touch_detected_flag = 1;
				possible_click_event = 1;
				coordinate2.x = ts_state.touchX[0];
				coordinate2.y = ts_state.touchY[0];
				coordinate1.x = ts_state.touchX[0];
				coordinate1.y = ts_state.touchY[0];
			} else {
				coordinate2.x = ts_state.touchX[0];
				coordinate2.y = ts_state.touchY[0];

				BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
				BSP_LCD_FillCircle(coordinate2.x, coordinate2.y, 5);
				osDelay(5);

				int32_t click_diff_x = ts_state.touchX[0] - coordinate1.x;
				int32_t click_diff_y = ts_state.touchY[0] - coordinate1.y;
				if (abs(click_diff_x) > 3 || abs(click_diff_y) > 5)
					possible_click_event = 0;
			}
			osDelay(5);

		} else {
			first_touch_detected_flag = 0;
			if (possible_click_event) {
				it_was_click = 1;

				turn_on_a_led_thread();
				osDelay(5);
				possible_click_event = 0;
			}
		}
	}
	osDelay(10);

}

void turn_on_a_led_thread()
{
	osDelay(50);

	for (uint8_t c = 0; c < LED_MATRIX_COLS; c++) {
		for (uint8_t r = 0; r <  LED_MATRIX_ROWS; r++) {
			if ((it_was_click = 1) &&
			((abs(coordinate2.x - touch_areas[r][c].x)) < 30) &&
			((abs(coordinate2.y - touch_areas[r][c].y)) < 30)) {
				if ((led_matrix_state[r][c] == GPIO_PIN_RESET)) {
					led_matrix_set(r, c, 1);
					BSP_LCD_SetTextColor(LCD_COLOR_RED);
					BSP_LCD_FillCircle((touch_areas[r][c].x), (touch_areas[r][c].y), 15);
					osDelay(50);

				} else if ((led_matrix_state[r][c] == GPIO_PIN_SET)) {
					led_matrix_set(r, c, 0);
					BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
					BSP_LCD_FillCircle((touch_areas[r][c].x), (touch_areas[r][c].y), 15);
					osDelay(50);
				}
				it_was_click = 0;

			}
		}
	}
}

void clear_led_panel()
{
	for (uint8_t c = 0; c < LED_MATRIX_COLS; c++) {
		for (uint8_t r = 0; r <  LED_MATRIX_ROWS; r++) {
			led_matrix_set(r, c, 0);

		}
	}
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
