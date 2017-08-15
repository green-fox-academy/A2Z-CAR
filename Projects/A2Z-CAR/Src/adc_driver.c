#include "adc_driver.h"

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

uint16_t adc_current_measure()										// 3.3 V = 4095
{																	// 2.5 V (max) = 3100 = 25 A
	HAL_ADC_Start(&adc_12b_handle);									// 1.25 V = 1550 = 0 A
	HAL_ADC_PollForConversion(&adc_12b_handle, HAL_MAX_DELAY);		// 1.3 V = 1612 = 1 A
	return HAL_ADC_GetValue(&adc_12b_handle);						// a change of 1 A means a step of 62
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

void get_adc_values(uint8_t *adc_values)
{
	uint8_t *values;
	values = adc_values;
	a0_adc_init();
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);
	values++;

	a1_adc_init();
	*values = adc_measure_avg(10);
	values++;

	a2_adc_init();
	*values = adc_measure_avg(10);
	values++;

	a3_adc_init();
	*values = adc_measure_avg(10);
	values++;

	a4_adc_init();
	*values = adc_measure_avg(10);
	values++;

	a5_adc_init();
	*values = adc_measure_avg(10);
	values++;

	d7_adc_init();
	*values = adc_measure_avg(10);
	values++;

	d1_adc_init();
	*values = adc_measure_avg(10);
	values++;

	d0_adc_init();
	*values = adc_measure_avg(10);
}

uint8_t areneighbours(uint8_t a, uint8_t b)
{
	if ((a - 1 == b ) || (a + 1 == b )) {
		return 1;
	}
	return 0;
}

int8_t get_bias()
{
	get_adc_values(adc_values);
	for(int i = 0; i < 9; i++) {
		//printf("%d ",adc_values[i]);
	}
	//printf("\n");
	uint8_t limit = 40;
	uint8_t first = 0, second = 0;
	for (int i = 0; i < 9; i++) {
		if (adc_values[i] > adc_values[first]) {
			first = i;
		}
	}
	for (int i = 0; i < 9; i++) {
		if (i != first && adc_values[i] > adc_values[second]) {
			second = i;
		}
	}
	int8_t bias = 0;
	if (adc_values[first] > limit) {
		if ((adc_values[second] > (adc_values[first] *9  / 10)) && areneighbours(first, second)) {
			bias = (2 * first + second);
		} else {
			bias = first * 3;
		}
		bias = -1 * ((4 * 3) - bias) ;
		return bias;
	}
	return 20;
}
