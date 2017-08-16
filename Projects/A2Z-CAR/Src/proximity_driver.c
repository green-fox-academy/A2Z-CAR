/*
 * proximity_driver.c
 *
 *  Created on: 2017. aug. 15.
 *      Author: AND
 */
#include "proximity_driver.h"

typedef struct {
	uint32_t ovf;
	uint32_t prev;
	uint32_t last;
} input_capture_data_t;

input_capture_data_t ic_cntr = {0, 0, 0};
uint32_t ovf_cntr = 0;
float prev_rpm_value = 0;


int8_t proximity_sensor_trigger_init()
{
	//init D2 (PD14) as trigger for proximity sensors
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_14;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOD, &GPIO_Init);

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	printf("Proxim trigger init done.\n");

	return 0;
}

void proximity_send_trigger()
{
	printf("Proxim trigger sending started.\n");
	while(1){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		printf("Proxim trigger sent.\n");
		get_freq();
		HAL_Delay(250);

	}
}

void proximity_ic2_init()
{
	//init D3 (PB0) input capture mode
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_0;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOB, &GPIO_Init);

	__HAL_RCC_TIM1_CLK_ENABLE();
	ic_handle.Instance = TIM1;
	ic_handle.State = HAL_TIM_STATE_RESET;
	ic_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_2;
	ic_handle.Init.RepetitionCounter = 0xFF;
	ic_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	ic_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	ic_handle.Init.Period = 0xFFFF;
	ic_handle.Init.Prescaler = 0;
	HAL_TIM_IC_Init(&ic_handle);

	ic_ic_init.ICFilter = 0;
	ic_ic_init.ICPolarity = TIM_ICPOLARITY_BOTHEDGE;
	ic_ic_init.ICPrescaler = TIM_ICPSC_DIV1;
	ic_ic_init.ICSelection = TIM_ICSELECTION_DIRECTTI;
	HAL_TIM_IC_ConfigChannel(&ic_handle, &ic_ic_init, TIM_CHANNEL_2);

	HAL_TIM_Base_Start_IT(&ic_handle);
	HAL_TIM_IC_Start_IT(&ic_handle, TIM_CHANNEL_2);

}

/*int8_t proximity_ic1_init()
{
	// Init pin D4(PA3) as PWM TIM5 output (alternate timers: TIM2_CH4 TIM5_CH4 TIM15_CH2)
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitDef.Mode = GPIO_MODE_AF_PP;
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_MEDIUM;
	GPIO_InitDef.Pin = GPIO_PIN_3;
	GPIO_InitDef.Alternate = GPIO_AF2_TIM5;
	HAL_GPIO_Init(GPIOA, &GPIO_InitDef);

	// TIM5 init as PWM, 10 kHz
	__HAL_RCC_TIM5_CLK_ENABLE();
	proxi_pwm_handle.Instance = TIM5;
	proxi_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	proxi_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	proxi_pwm_handle.Init.Period = 8000;
	proxi_pwm_handle.Init.Prescaler = 0;
	if (HAL_TIM_PWM_Init(&proxi_pwm_handle) != HAL_OK) {
		return -1;
	}

	proxi_pwm_oc_init.OCFastMode = TIM_OCFAST_DISABLE;
	proxi_pwm_oc_init.OCIdleState = TIM_OCIDLESTATE_RESET;
	proxi_pwm_oc_init.OCMode = TIM_OCMODE_PWM1;
	proxi_pwm_oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	proxi_pwm_oc_init.Pulse = 4000;
	HAL_TIM_PWM_ConfigChannel(&proxi_pwm_handle, &proxi_pwm_oc_init, TIM_CHANNEL_5);

	return 0;

}*/

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	ic_cntr.prev = ic_cntr.last;
	ic_cntr.last = TIM1->CCR1;
	ic_cntr.ovf = ovf_cntr;
	ovf_cntr = 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	ovf_cntr++;
}

float get_freq()
{
	  //TIM1_BRK_TIM15_IRQn         = 24,     /*!< TIM1 Break interrupt and TIM15 global interrupt                   */
	  //TIM1_UP_TIM16_IRQn          = 25,     /*!< TIM1 Update Interrupt and TIM16 global interrupt                  */
	  //TIM1_TRG_COM_TIM17_IRQn     = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM17 global interrupt */
	  //TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                                    */

	HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
	input_capture_data_t snapshot = ic_cntr;
	HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);

	float steps = (float)snapshot.ovf * ic_handle.Init.Period + snapshot.last - snapshot.prev;
	float tim1_clk_freq = (float)SystemCoreClock / 2 / (ic_handle.Init.Prescaler + 1); // Because clock division is 1x, so only sysclock matters
	float tim1_clk_period = 1/ tim1_clk_freq;
	float signal_period = steps * tim1_clk_period;
	float signal_freq = 1 / signal_period;

	/*if (isnan(signal_freq) || isinf(signal_freq))
		return -1;
	else*/
	printf("Steps are: %f\n", steps);
	printf("Signal Periode is: %f\n", signal_period);
	printf("Signal Freq is: %f\n", signal_freq);
		return signal_freq;
}

