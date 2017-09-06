/*
 * proximity_driver.c
 *
 *  Created on: 2017. aug. 15.
 *      Author: AND
 */
#include "proximity_driver.h"
#include "cmsis_os.h"

TIM_HandleTypeDef proxim_timer_handle;
uint32_t proxim1_cntr = 0;
uint32_t proxim2_cntr = 0;
int8_t proxim_flag = 0;
uint32_t cm_cntr = 0;

int8_t proximity_sensor1_trigger_init();
int8_t proximity_sensor2_trigger_init();
int8_t proximity_exti_init();
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
static void EXTI3_IRQHandler_Config(void);
int8_t proximity_timer_init();
int8_t led_feedback_init();
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void proximity1_send_trigger();
void proximity2_send_trigger();
uint32_t read_proximity_data();
uint8_t process_proximity(uint32_t distance);


int8_t proximity_driver_init()
{
	if (proximity_sensor1_trigger_init() != 0) {
		return -1;
	}

	if (proximity_sensor2_trigger_init() != 0) {
			return -1;
	}

	if (proximity_exti_init() != 0) {
		return -1;
	}

	if (proximity_timer_init() != 0) {
			return -1;
		}

	if (led_feedback_init() != 0) {
		return -1;
	}

	return 0;
}

int8_t proximity_sensor1_trigger_init()
{
	//init D3 (PB0) as trigger for proximity sensors
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_0;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_Init);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
#ifdef DEBUG_MODE
	printf("Proxim trigger1 init done.\n");
#endif
	return 0;
}

int8_t proximity_sensor2_trigger_init()
{
	//init D2 (PD14) as trigger for proximity sensors
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_14;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOD, &GPIO_Init);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
#ifdef DEBUG_MODE
	printf("Proxim trigger2 init done.\n");
#endif
	return 0;
}

void proximity1_send_trigger()
{
	//init trigger pin to D3 (PB0)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	osDelay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	//printf("Proxim trigger sent.\n");
	osDelay(1);
}

void proximity2_send_trigger()
{
	//init trigger pin to D2 (PD14)
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	osDelay(1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	//printf("Proxim trigger sent.\n");
	osDelay(1);
}

int8_t proximity_exti_init()
{
	//init D4 (PA3) EXTI mode
	EXTI3_IRQHandler_Config();
#ifdef DEBUG_MODE
	printf("Proxim sensor init done.\n");
#endif
	return 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (proxim_flag == 0) {
		HAL_NVIC_DisableIRQ(TIM4_IRQn);
		proxim_flag = 1;
#ifdef DEBUG_MODE
		printf("up   ");
#endif
	} else if (proxim_flag == 1) {
		HAL_NVIC_EnableIRQ(TIM4_IRQn);
		proxim_flag = 0;
#ifdef DEBUG_MODE
		printf("down\n");
#endif
	}
}

static void EXTI3_IRQHandler_Config(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	/* Configure D4 -- PA3 pin as input floating */
	/* Enable GPIOA clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable and set EXTI lines 10 to 15 Interrupt to the lowest priority */
	HAL_NVIC_SetPriority(EXTI3_IRQn, 3, 0);
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
#ifdef DEBUG_MODE
	printf("TIM4 init done.\n");
#endif
	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	cm_cntr++;
}

int8_t led_feedback_init()
{
	//Initialize D11 and D12 (PA6 and PA7) as LED output
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);

	return 0;
}

uint32_t read_proximity_data()
{
	uint8_t measure_failed = 0;
	uint32_t sum = 0;

	for (int i = 0; i < 10; i++){

		cm_cntr = 0;
		proxim1_cntr = 0;
		proxim_flag = 1;
		proximity1_send_trigger();

		while (proxim_flag == 0){
#ifdef DEBUG_MODE
			printf("interrupt 1.\n");
#endif
			osDelay(1);
		}
		proxim1_cntr = cm_cntr;
#ifdef DEBUG_MODE
		printf("proxim1_cntr: %lu", proxim1_cntr);
#endif
		cm_cntr = 0;
		proxim2_cntr = 0;
		proxim_flag = 1;
		proximity2_send_trigger();

		while (proxim_flag == 0){
#ifdef DEBUG_MODE
			printf("interrupt 2.\n");
#endif
			osDelay(1);
		}
		proxim2_cntr = cm_cntr;
#ifdef DEBUG_MODE
		printf("proxim2_cntr: %lu - \n", proxim2_cntr);
#endif
		if ((proxim1_cntr > 600) || (proxim2_cntr > 600)){
			//measure failure
			measure_failed++;

		} else if ((proxim1_cntr < 600) && (proxim2_cntr < 600)){
			sum = sum + (proxim1_cntr + proxim2_cntr);

		}
	}

	distance = sum / (2 * (10 - measure_failed));
#ifdef DEBUG_MODE
	printf("distance: %lu, failure: %d\n\n", distance, measure_failed);
#endif
	return distance;
}

uint8_t process_proximity(uint32_t distance)
{
	object_flag = 0;
	if (distance < 30) {
		//stop_drive();
		object_flag = 40;

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); 	//green led
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);		//red led

	} else if ((distance < 50) && (distance > 29)) {
		//stop_drive();
		object_flag = 40;
#ifdef DEBUG_MODE
		printf("Stop signal sent.\n");
#endif
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); 	//green led
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);					//red led

	} else if ((distance < 100)  && (distance > 49)) {
		//decelerate();
		object_flag = 20;
#ifdef DEBUG_MODE
		printf("Decelerate signal sent.\n");
#endif
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7); 	//green led
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);	//red led

	} else {
		object_flag = 10;
		//go();
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);		//green led
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);	//red led
	}

	return 0;
}

int8_t proximity_control_thread()
{
	while (1){

		uint32_t measured_distance = read_proximity_data();
		process_proximity(measured_distance);

	}

	terminate_thread();


	return 0;
}
