/*
 * proximity_driver.c
 *
 *  Created on: 2017. aug. 15.
 *      Author: AND
 */
#include "proximity_driver.h"

uint32_t proxim1_cntr = 0;
int8_t proxim1_up = 0;
uint32_t proxim2_cntr = 0;
int8_t proxim2_up = 0;
uint32_t cm_cntr = 0;

int8_t proximity_driver_init()
{
	if (proximity_timer_init() != 0) {
		return -1;
	}

	if (proximity_sensor_trigger_init() != 0) {
		return -1;
	}

	if (proximity1_exti_init() != 0) {
		return -1;
	}

	if (proximity2_exti_init() != 0) {
		return -1;
	}

	return 0;
}

int8_t proximity_sensor_trigger_init()
{
	//init D3 (PB0) as trigger for proximity sensors
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_0;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_Init);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

	printf("Proxim trigger init done.\n");

	return 0;
}

void proximity_send_trigger()
{
	//init trigger pin to D3 (PB0)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	//printf("Proxim trigger sent.\n");
	HAL_Delay(250);

}

int8_t proximity1_exti_init()
{
	//init D2 (PD14) EXTI mode
	EXTI15_10_IRQHandler_Config();

	printf("Proxim sensor1 init done.\n");

	return 0;

}

int8_t proximity2_exti_init()
{
	//init D4 (PA3) EXTI mode
	EXTI3_IRQHandler_Config();

	printf("Proxim sensor2 init done.\n");

	return 0;
}

int8_t proximity_control_thread()
{

	while (1){
		cm_cntr = 0;
		proxim1_cntr = 0;
		proxim1_up = 0;
		proximity_send_trigger();
		while (proxim1_up == 1);
		proxim1_cntr = cm_cntr;
		printf("proxim1_cntr: %lu\n", proxim1_cntr);

		cm_cntr = 0;
		proxim2_cntr = 0;
		proxim2_up = 0;
		proximity_send_trigger();
		while (proxim2_up == 1);
		proxim2_cntr = cm_cntr;
		printf("proxim2_cntr: %lu\n\n", proxim2_cntr);

	}
	return 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if ((GPIO_Pin == GPIO_PIN_14) && (proxim1_up == 0)) {
	  proxim1_up = 1;
	  HAL_TIM_Base_Start_IT(&proxim_timer_handle);
	  //printf("D2-PD14 up\n");

  } else if ((GPIO_Pin == GPIO_PIN_14) && (proxim1_up == 1)) {
	  proxim1_up = 0;
	  HAL_TIM_Base_Stop_IT(&proxim_timer_handle);
	  //printf("D2-PD14 down\n");

  } else if ((GPIO_Pin == GPIO_PIN_3) && (proxim2_up == 0)) {
	  proxim2_up = 1;
	  HAL_TIM_Base_Start_IT(&proxim_timer_handle);
	  //printf("D4-PA3 up\n");

  } else if ((GPIO_Pin == GPIO_PIN_3) && (proxim2_up == 1)) {
	  proxim2_up = 0;
	  HAL_TIM_Base_Stop_IT(&proxim_timer_handle);
	  //printf("D4-PA3 down\n");
  }
}

static void EXTI15_10_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  /* Configure D2 -- PD14 pin as input floating */
  /* Enable GPIOD clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_14;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Enable and set EXTI lines 10 to 15 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

static void EXTI3_IRQHandler_Config(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	  /* Configure D4 -- PA3 pin as input floating */
  /* Enable GPIOA clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_3;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable and set EXTI lines 10 to 15 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}

int8_t proximity_timer_init()
{
	//init TIM4_CH3 10 kHz x 0,58 = 1 periode / 1 cm
	proxim_timer_handle.Instance = TIM4;
	proxim_timer_handle.State = HAL_TIM_STATE_RESET;
	proxim_timer_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_3;
	proxim_timer_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	proxim_timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	proxim_timer_handle.Init.Period = 928;
	proxim_timer_handle.Init.Prescaler = 4;
	HAL_TIM_Base_Init(&proxim_timer_handle);
	HAL_TIM_Base_Start_IT(&proxim_timer_handle);

	printf("TIM4 init done.\n");

	//init D15 (PB8) TIM4 CH3
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_8;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_Init);

	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	cm_cntr++;
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);

}


