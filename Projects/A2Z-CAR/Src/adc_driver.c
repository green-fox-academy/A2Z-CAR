#include "adc_driver.h"

ADC_HandleTypeDef adc_handle;
ADC_HandleTypeDef adc_12b_handle;
ADC_ChannelConfTypeDef adc_ch_conf;
uint8_t group;
uint8_t adc_values[9], max_bg[9];
int8_t bias;
//void calibrate(uint8_t *calibration);
void get_adc_values(uint8_t *adc_values);

void gpio_init()
{
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_5;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);

	GPIO_Init.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);

	GPIO_Init.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);

	GPIO_Init.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);

	GPIO_Init.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);

	GPIO_Init.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);

	GPIO_Init.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);

	GPIO_Init.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);

	GPIO_Init.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);

	GPIO_Init.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);

	GPIO_Init.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);
}

void a0_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_14;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void a1_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_13;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void a2_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_4;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void a3_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_3;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void a4_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_2;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void a5_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_1;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void d7_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_9;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void d1_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_5;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void d0_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_6;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void d10_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_7;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

void d13_adc_init()
{
	adc_ch_conf.Channel = ADC_CHANNEL_10;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

uint16_t adc_measure()
{
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

uint16_t adc_rpm_measure()
{
	HAL_ADC_Start(&adc_12b_handle);
	HAL_ADC_PollForConversion(&adc_12b_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_12b_handle);
}


uint16_t adc_measure_avg(uint8_t num)
{
	uint32_t avg = 0, avg2 = 0;
	uint16_t values[num];
	for (int i = 0; i< num; i++) {
		values[i] = adc_measure();
		avg += values[i];
	}
	avg /= num;
	int j = 0;
	for (int i = 0; i< 10; i++) {
		if (values[i] > avg * 0.7 && values[i] < avg * 1.3) {
			avg2 += values[i];
			j++;
		}
	}
	avg2 /= j;
	return avg2;
}

void adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	gpio_init();

	adc_handle.State = HAL_ADC_STATE_RESET;
	adc_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2; //
	adc_handle.Init.Resolution = ADC_RESOLUTION_8B;
	adc_handle.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	adc_handle.Init.DMAContinuousRequests = DISABLE;
	adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc_handle.Init.ContinuousConvMode = DISABLE;
	adc_handle.Init.DiscontinuousConvMode = DISABLE;
	adc_handle.Init.ScanConvMode = DISABLE;
	adc_handle.Init.NbrOfConversion = 1;
	adc_handle.Instance = ADC1;
	HAL_ADC_Init(&adc_handle);

	adc_ch_conf.Offset = 0;
	adc_ch_conf.Rank = 1;
	adc_ch_conf.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;

}

void adc_12b_init()		// ADC2 channel 7 on pin D10 (PA2)
{
	adc_12b_handle.State = HAL_ADC_STATE_RESET;
	adc_12b_handle.Instance = ADC2;
	adc_12b_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	adc_12b_handle.Init.Resolution = ADC_RESOLUTION_12B;
	adc_12b_handle.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	adc_12b_handle.Init.DMAContinuousRequests = DISABLE;
	adc_12b_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc_12b_handle.Init.ContinuousConvMode = DISABLE;
	adc_12b_handle.Init.DiscontinuousConvMode = DISABLE;
	adc_12b_handle.Init.ScanConvMode = DISABLE;
	HAL_ADC_Init(&adc_12b_handle);

	adc_ch_conf.Channel = ADC_CHANNEL_7;
	adc_ch_conf.Offset = 0;
	adc_ch_conf.Rank = 1;
	adc_ch_conf.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	HAL_ADC_ConfigChannel(&adc_12b_handle, &adc_ch_conf);
}

void calibrate()
{
	uint8_t values[9];
	for (int j = 0; j< 9; j++) {
		max_bg[j] = 0;
	}
	for (int i = 0; i< 100; i++) {
		get_adc_values(values);
		for (int j = 0; j< 9; j++) {
			if (values[j] > max_bg[j]) {
				max_bg[j] = values[j];
			}
		}
		HAL_Delay(20);
	}

}
void get_adc_values(uint8_t *adc_values)
{
	uint8_t *values;
	values = adc_values;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	group = 0;

	a0_adc_init();
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure();
	values+=2;

	a2_adc_init();
	*values = adc_measure();
	values+=2;

	a4_adc_init();
	*values = adc_measure();
	values+=2;


	d7_adc_init();
	*values = adc_measure();
	values+=2;

	d0_adc_init();
	*values = adc_measure();

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	group = 1;
	values = adc_values + 1;

	a1_adc_init();
	*values = adc_measure();
	values+=2;

	a3_adc_init();
	*values = adc_measure();
	values+=2;

	a5_adc_init();
	*values = adc_measure();
	values+=2;

	d1_adc_init();
	*values = adc_measure();
}

uint8_t areneighbours(uint8_t a, uint8_t b) {
	if ((a - 1 == b ) || (a + 1 == b )) {
		return 1;
	}
	return 0;
}
int8_t get_bias()
{
	uint8_t detail = 9;
	if (bias <= (detail * 4)) {
		former_bias = bias;
	}
	uint8_t bg_color, color = 1;
	if (color == 1) {
		bg_color = 0;
	} else {
		bg_color = 1;
	}
	get_adc_values(adc_values);
	//printf("adc:\n");
	for (int i = 0; i < 9; i++) {
		//printf(" %4d",adc_values[i]);
	}
	//printf("\n ");
	//uint8_t adc_values[] = {10,100,10,100,10,100,10,100, 10}; //test
	uint8_t contrast[9];
	for (int i = 0; i < 9; i++) {
		contrast[i] = adc_values[i] > (max_bg[i] + 10);
		printf(" %4d",contrast[i]);
	}
	printf("\n");
	uint8_t pos = 0;
	int8_t center[4] = {-1, -1, -1, -1}, width = 0;
	uint8_t is_line = 0;
	if (contrast[0] == color) {
		is_line = 1;
	}
	for (int i = 1; i < 9; i++) {
		if ((contrast[i - 1] == bg_color && contrast[i] == color )) {
			//line left edge
			center[pos] = i * detail;
			width = 0;
			is_line = 1;
		} else if (is_line && contrast[i - 1] == color && contrast[i] == bg_color) {
			//line right edge
			center[pos] -=  detail * width/2;
			pos++;
			width = 0;
			is_line = 0;
		} else if (is_line) { //on a line
			width++;
			center[pos] += detail;
		}
	}

	uint8_t linepos = 0;

	bias = center[linepos] - (detail * 4);
	if (center[linepos] == -1) {
		bias = 100;
	}
	//printf("bias: %d\n", bias );

	return  bias;




	return 100;
}
