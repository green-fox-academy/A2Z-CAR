/**
  ******************************************************************************
  * @file    FreeRTOS\FreeRTOS_LowPower_LPTIM\Src\main.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    21-April-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright � 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal_tim.h"
#include "cmsis_os.h"
/* Private typedef -----------------------------------------------------------*/
TIM_HandleTypeDef pwm_handle;
TIM_OC_InitTypeDef pwm_oc_init;
GPIO_InitTypeDef GPIO_InitDef;
UART_HandleTypeDef uart_handle;
ADC_HandleTypeDef adc_handle;
ADC_ChannelConfTypeDef adc_ch_conf;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
#define LED_TOGGLE_DELAY         (1000)
TIM_HandleTypeDef pwm_handle;
TIM_OC_InitTypeDef pwm_oc_init;

/* Private function prototypes -----------------------------------------------*/
static void ToggleLedThread(const void *argument);
static void GPIO_ConfigAN(void);
static void SystemClock_Config(void);

static void StartThread(void const * argument);
static void servo_control_thread(void const * argument);

//void uart_init();
void pwm_init();
void pwm_set_duty(float duty);
void set_servo_angle(int8_t angle);

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
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
	//ADC12_IN14
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
	//ADC12_IN13
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
	//ADC123_IN4
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
	//ADC123_IN3
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
	//ADC123_IN2
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
	//ADC123_IN1
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
	//pa4 ADC12_IN9
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
	//pa4 ADC12_IN9
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
	//pa4 ADC12_IN9
	adc_handle.Instance = ADC1;
	adc_ch_conf.Channel = ADC_CHANNEL_6;
}
void d10_adc_init()
{ //pa2 ADC12_IN7
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
{  //pa5 ADC12_IN10
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

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
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
	printf("adcinit: %d\n",status);
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
	//printf("adc: %d ",*values);
	values++;
	a1_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);
	//printf("adc: %d ",*values);
	values++;
	a2_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);
	//printf("adc: %d ",*values);
	values++;
	a3_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);
	//printf("adc: %d ",*values);
	values++;
	a4_adc_init();
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
	d10_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);

	values++;
	d1_adc_init();
	HAL_ADC_Init(&adc_handle);
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
	*values = adc_measure_avg(10);
	//printf("adc: %d ",*values);
	printf("\n ");

}
int8_t get_error()
{
	uint8_t adc_values[9];
	get_adc_values(&adc_values);
	for(int i = 0; i < 9; i++) {
		printf("%d ",adc_values[i]);
	}
	uint8_t limit = 40;
	if (adc_values[2] > limit) {
		return 0;
	} else if (adc_values[1] > limit) {
		return 1;
	} else if (adc_values[3] > limit) {
		return -1;
	} else if (adc_values[4] > limit) {
		return -2;
	} else if (adc_values[0] > limit) {
		return 2;
	} else {
		return 3;
	}

}
void uart_init()
{
	uart_handle.Instance = DISCOVERY_COM1;
	uart_handle.Init.BaudRate = 115200;
	uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
	uart_handle.Init.StopBits = UART_STOPBITS_1;
	uart_handle.Init.Parity = UART_PARITY_NONE;
	uart_handle.Init.Mode = UART_MODE_TX_RX;
	uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;
	uart_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	uart_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	BSP_COM_Init(COM1, &uart_handle);

	/* Output without printf, using HAL function*/
	char msg[] = "UART HAL Example\r\n";
	HAL_UART_Transmit(&uart_handle, msg, strlen(msg), 100);

	/* Output a message using printf function */
	printf("UART Printf Example: retarget the C library printf function to the UART\r\n");
	printf("** Test finished successfully. ** \r\n");
}
void stop() {
	BSP_LED_On(LED2);
}
void set_servo()
{
	int8_t error = get_error();
	printf("error:%d\n",error);
	if (error != 3) {
		set_servo_angle(error * 10);
		printf("servo: %d\n", error * 10);
		//BSP_LED_Off(LED2);
	} else {
		//set_servo_to(0);
		printf("stop\n");
		stop();
	}
}
int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	/* Configure the system clock to 80 MHz */
	SystemClock_Config();
	uart_init();

	/* Init thread */
	osThreadDef(Start, StartThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	osThreadCreate (osThread(Start), NULL);

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	for (;;);
}


/**
  * @brief  Start Thread
  * @param  argument not used
  * @retval None
  */
static void StartThread(void const * argument)
{
	/* Initialize LED */
	BSP_LED_Init(LED2);

	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

//	uart_init();

	pwm_init();
	a0_adc_init();
	adc_init();
	osThreadDef(servo, servo_control_thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE*2);
	osThreadCreate(osThread(servo), NULL);

	osThreadDef(Thread, ToggleLedThread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE);
	osThreadCreate(osThread(Thread), NULL);

	while (1) {
	/* Delete the Init Thread */
	osThreadTerminate(NULL);
	}
}


static void servo_control_thread(void const * argument)
{
	while(1) {
		/*
		for (int8_t i = -45; i < 46; i++) {
			printf("%d\n", i);
			set_servo_angle(i);
			osDelay(100);
		}
		for (int8_t i = 45; i > -46; i--) {
			printf("%d\n", i);
			set_servo_angle(i);
			osDelay(100);
		}
		*/
		set_servo();
	}

	while (1) {
		/* Delete the Init Thread */
		osThreadTerminate(NULL);
	}
}


//void uart_init()
//{
//	uart_handle.Instance = DISCOVERY_COM1;
//	uart_handle.Init.BaudRate = 115200;
//	uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
//	uart_handle.Init.StopBits = UART_STOPBITS_1;
//	uart_handle.Init.Parity = UART_PARITY_NONE;
//	uart_handle.Init.Mode = UART_MODE_TX_RX;
//	uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//	uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;
//	uart_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//	uart_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//
//	BSP_COM_Init(COM1, &uart_handle);
//}


/**
  * @brief  Initializes TIM3 as PWM source
  * @param  None
  * @retval None
  */
void pwm_init()
{
	// Initialize pin D5 (PB4) as PWM (TIM3) output
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitDef.Mode = GPIO_MODE_AF_PP;
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_MEDIUM;
	GPIO_InitDef.Pin = GPIO_PIN_4;
	GPIO_InitDef.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);

	// TIM3 init as PWM
	__HAL_RCC_TIM3_CLK_ENABLE();
	pwm_handle.Instance = TIM3;
//	pwm_handle.State = HAL_TIM_STATE_RESET;
//	pwm_handle.Channel = TIM_CHANNEL_1;
	pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	pwm_handle.Init.Period = 31380;
	pwm_handle.Init.Prescaler = 50;
	pwm_handle.Init.RepetitionCounter = 0;
	HAL_TIM_PWM_Init(&pwm_handle);

	pwm_oc_init.OCFastMode = TIM_OCFAST_DISABLE;
	pwm_oc_init.OCIdleState = TIM_OCIDLESTATE_RESET;
	pwm_oc_init.OCMode = TIM_OCMODE_PWM1;
	pwm_oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;
	pwm_oc_init.Pulse = 2354;
	HAL_TIM_PWM_ConfigChannel(&pwm_handle, &pwm_oc_init, TIM_CHANNEL_1);
}


/**
  * @brief  Sets the duty cycle of TIM3 CH1
  * @param  duty - duty cycle to set (0.0-100.0)
  * @retval None
  */
void pwm_set_duty(float duty)
{
	uint32_t pulse = pwm_handle.Init.Period * (duty / 100);
	pwm_oc_init.Pulse = pulse;
	HAL_TIM_PWM_ConfigChannel(&pwm_handle, &pwm_oc_init, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&pwm_handle, TIM_CHANNEL_1);
}


void set_servo_angle(int8_t angle)
{
	// 5% duty cycle is the leftmost position of the steering, 10% is the rightmost,
	// leftmost is -45 degrees now, rightmost is 45,
	// so 1 degree equals to (5 / 90) % in duty cycle.
	// 7.5 % is 0 degrees
	printf("angle %d \n", angle);
	float duty = 7.5 + (5.0 / 90.0) * (float)angle;
	pwm_set_duty(duty);
}


/**
  * @brief Toggle Thread function
  * @param  argument: Not used
  * @retval None
  */
static void ToggleLedThread(const void *argument)
{
	for (;;) {
		BSP_LED_On(LED2);
		osDelay(LED_TOGGLE_DELAY);
		BSP_LED_Off(LED2);
		osDelay(LED_TOGGLE_DELAY);
	}

	while (1) {
		/* Delete the Init Thread */
		osThreadTerminate(NULL);
	}
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&uart_handle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}


void vMainPreStopProcessing(void)
{
  /* Ensure that MSI is wake-up system clock */ 
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);
  
  GPIO_ConfigAN();
}


void vMainPostStopProcessing(void)
{  
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enable the Internal High Speed oscillator (MSI). */
  __HAL_RCC_MSI_ENABLE();
  
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_SYSCLKSOURCE_PLLCLK);	
  
  /* Update the SystemCoreClock global variable */
  SystemCoreClock =  HAL_RCC_GetSysClockFreq() >> AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  
  BSP_LED_Init(LED2);
}

/**
  * @brief  Configure all GPIO's to AN to reduce the power consumption
  * @param  None
  * @retval None
  */
static void GPIO_ConfigAN(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = GPIO_PIN_All;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
    
    /* Disable GPIOs clock */

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOE_CLK_DISABLE();
    __HAL_RCC_GPIOF_CLK_DISABLE();
    __HAL_RCC_GPIOG_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
