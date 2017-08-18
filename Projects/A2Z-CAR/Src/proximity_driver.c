/*
 * proximity_driver.c
 *
 *  Created on: 2017. aug. 15.
 *      Author: AND
 */
#include "proximity_driver.h"

static void EXTI15_10_IRQHandler_Config(void);


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
	printf("Proxim trigger sending started.\n");

	//init trigger pin to D3 (PB0)
	while(1){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
			printf("Proxim trigger sent.\n");
			HAL_Delay(250);

	}
}

int8_t proximity1_exti_init()
{
	//init D2 (PD14) EXTI mode
	 EXTI15_10_IRQHandler_Config();

	printf("Proxim sensor2 init done.\n");

	return 0;

}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_14)
  {
	  printf("D2 (PD14) EXTI\n");
  }
}

static void EXTI15_10_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* Configure PD.14 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_14;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Enable and set EXTI lines 10 to 15 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

