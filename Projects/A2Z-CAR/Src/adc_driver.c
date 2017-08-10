#include "adc_driver.h"

void a0_adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_5;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);
	adc_handle.Instance = ADC1;
	adc_ch_conf.Channel = ADC_CHANNEL_14;
}

void a1_adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_4;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);
	adc_handle.Instance = ADC1;
	adc_ch_conf.Channel = ADC_CHANNEL_13;
}

void a2_adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_3;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);
	adc_handle.Instance = ADC2;
	adc_ch_conf.Channel = ADC_CHANNEL_4;
}

void a3_adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_2;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);
	adc_handle.Instance = ADC2;
	adc_ch_conf.Channel = ADC_CHANNEL_3;
}

void a4_adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_1;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);
	adc_handle.Instance = ADC3;
	adc_ch_conf.Channel = ADC_CHANNEL_2;
}

void a5_adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_0;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	HAL_GPIO_Init(GPIOC, &GPIO_Init);
	adc_handle.Instance = ADC3;
	adc_ch_conf.Channel = ADC_CHANNEL_1;
}

void d7_adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_4;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);
	adc_handle.Instance = ADC1;
	adc_ch_conf.Channel = ADC_CHANNEL_9;
}

void d1_adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_0;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);
	adc_handle.Instance = ADC1;
	adc_ch_conf.Channel = ADC_CHANNEL_5;
}

void d0_adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_1;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);
	adc_handle.Instance = ADC1;
	adc_ch_conf.Channel = ADC_CHANNEL_6;
}

void d10_adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_2;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);

	adc_handle.Instance = ADC1;
	adc_ch_conf.Channel = ADC_CHANNEL_7;
}

void d13_adc_init()
{
	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_5;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);

	adc_handle.Instance = ADC1;
	adc_ch_conf.Channel = ADC_CHANNEL_10;
}

uint16_t adc_measure()
{
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
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
	a0_adc_init();
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
	HAL_StatusTypeDef status = HAL_ADC_Init(&adc_handle);
	//printf("adcinit: %d\n",status);
	adc_ch_conf.Offset = 0;
	adc_ch_conf.Rank = 1;
	adc_ch_conf.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	return;
}

void get_adc_values(uint8_t *adc_values)
{
	uint8_t *values;
	values = adc_values;
	a0_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);
	values++;

	a1_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);
	values++;

	a2_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);
	values++;

	a3_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);
	values++;

	a4_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);
	values++;

	a5_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);
	values++;

	d7_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);

	values++;
	d1_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);

	values++;
	d0_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);

}

int8_t get_error()
{

	get_adc_values(adc_values);
	for(int i = 0; i < 9; i++) {
		//printf("%d ",adc_values[i]);
	}
	//printf("\n ");
	uint8_t limit = 90;
	if (adc_values[4] > limit) {
		return 0;
	} else if (adc_values[3] > limit) {
		return 1;
	} else if (adc_values[5] > limit) {
		return -1;
	} else if (adc_values[2] > limit) {
		return -2;
	} else if (adc_values[6] > limit) {
		return 2;
	} else if (adc_values[1] > limit) {
		return -3;
	} else if (adc_values[7] > limit) {
		return 3;
	} else if (adc_values[0] > limit) {
		return -4;
	} else if (adc_values[8] > limit) {
		return 4;
	} else {
		return 5;
	}
}


/*
 d1  pa0 ADC12_IN5
 d0  pa1 ADC12_IN6
 d10 pa2 ADC12_IN7
     pa3 ADC12_IN8
 d7  pa4 ADC12_IN9
 d13 pa5 ADC12_IN10
     pa6 ADC12_IN11
     pa7 ADC12_IN12
     pb0 ADC12_IN15
     pb1 ADC12_IN16
 * */

