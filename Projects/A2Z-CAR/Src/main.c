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
  * <h2><center>&copy; Copyright © 2017 STMicroelectronics International N.V. 
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
#include "test2.h"
#include "adc_driver.h"
#include "stm32l4xx_hal_tim.h"
#include "cmsis_os.h"

/* Private typedef -----------------------------------------------------------*/
TIM_HandleTypeDef pwm_handle;
TIM_OC_InitTypeDef pwm_oc_init;
GPIO_InitTypeDef GPIO_InitDef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
#define LED_TOGGLE_DELAY         (1000)

#define SSID     "A66 Guest"
#define PASSWORD "Hello123"

#define WIFI_WRITE_TIMEOUT 10000
#define WIFI_READ_TIMEOUT  10000
#define CONNECTION_TRIAL_MAX          10

uint8_t RemoteIP[] = {10, 27, 99, 110};
uint8_t RxData [500];
char* modulename;
uint8_t TxData[] = "Hello big brother board!";
uint16_t RxLen;
uint8_t  MAC_Addr[6];
uint8_t  IP_Addr[4];
uint16_t Datalen;
int32_t Socket = -1;

TIM_HandleTypeDef pwm_handle;
TIM_OC_InitTypeDef pwm_oc_init;

/* Private function prototypes -----------------------------------------------*/
//static void ToggleLedThread(const void *argument);
static void GPIO_ConfigAN(void);
static void SystemClock_Config(void);

static void StartThread(void const * argument);
static void servo_control_thread(void const * argument);
static void wifi_send_thread(void const * argument);

void system_init();
void wifi_init();
void pwm_init();
void pwm_set_duty(float duty);
void set_servo_angle(int8_t angle);
void stop();
void set_servo();

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock to 80 MHz */
	SystemClock_Config();

	system_init();

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
	osThreadDef(servo, servo_control_thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE);
	osThreadCreate(osThread(servo), NULL);

	osThreadDef(wifi, wifi_send_thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE);
	osThreadCreate(osThread(wifi), NULL);

//	osThreadDef(Thread, ToggleLedThread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE);
//	osThreadCreate(osThread(Thread), NULL);

	while (1) {
	/* Delete the init thread */
	osThreadTerminate(NULL);
	}
}


static void servo_control_thread(void const * argument)
{
//	while(1) {
//		for (int8_t i = -45; i < 46; i++) {
//			set_servo_angle(i);
//			osDelay(10);
//		}
//		for (int8_t i = 45; i > -46; i--) {
//			set_servo_angle(i);
//			osDelay(10);
//		}
//	}

	set_servo();

	while (1) {
		/* Delete the thread */
		osThreadTerminate(NULL);
	}
}


static void wifi_send_thread(void const * argument)
{
	uint8_t adc_values[9] = {0, 25, 50, 75, 100, 125, 150, 200, 255};

//	while (1) {
//		if (BSP_PB_GetState(BUTTON_USER) == 0) {
//			printf("button pushed\n");
//			if(Socket != -1) {
//				if(WIFI_SendData(Socket, TxData, sizeof(TxData), &Datalen, WIFI_WRITE_TIMEOUT) != WIFI_STATUS_OK) {
//					printf("> ERROR : Failed to send Data.\n");
//			    } else {
//					printf("Message \"%s\" sent\n", TxData);
//			    }
//			}
//		}
//	}

	while(1) {
		if(Socket != -1) {
			if(WIFI_SendData(Socket, adc_values, sizeof(&adc_values), &Datalen, WIFI_WRITE_TIMEOUT) != WIFI_STATUS_OK) {
				printf("> ERROR : Failed to send Data.\n");
			} else {
				printf("Data sent\n");
			}
			for (int i=0; i < 9; i++) {
				adc_values[i] = 255 - adc_values[i];
			}
		}
		osDelay(1000);
	}

	while (1) {
		/* Delete the thread */
		osThreadTerminate(NULL);
	}
}


void system_init()
{
	BSP_LED_Init(LED2);

	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

	uart_init();

	/* Output without printf, using HAL function*/
	char msg[] = "UART HAL Example\r\n";
	HAL_UART_Transmit(&uart_handle, msg, strlen(msg), 100);

	/* Output a message using printf function */
	printf("UART Printf Example: retarget the C library printf function to the UART\r\n");
	printf("** Test finished successfully. ** \r\n");

	wifi_init();

	pwm_init();

	a0_adc_init();
	adc_init();
}


void wifi_init()
{
	uint16_t Trials = CONNECTION_TRIAL_MAX;

	/*Initialize  WIFI module */
	if(WIFI_Init() ==  WIFI_STATUS_OK) {
	    printf("> WIFI Module Initialized.\n");
	    if(WIFI_GetMAC_Address(MAC_Addr) == WIFI_STATUS_OK) {
	        printf("> es-wifi module MAC Address : %X:%X:%X:%X:%X:%X\n",
	               MAC_Addr[0],
	               MAC_Addr[1],
	               MAC_Addr[2],
	               MAC_Addr[3],
	               MAC_Addr[4],
	               MAC_Addr[5]);
	    } else {
	    	printf("> ERROR : CANNOT get MAC address\n");
	        BSP_LED_On(LED2);
	    }

	    if( WIFI_Connect(SSID, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK) {
	    	printf("> es-wifi module connected \n");
	    	if(WIFI_GetIP_Address(IP_Addr) == WIFI_STATUS_OK) {
	    		printf("> es-wifi module got IP Address : %d.%d.%d.%d\n",
	    		               IP_Addr[0],
	    		               IP_Addr[1],
	    		               IP_Addr[2],
	    		               IP_Addr[3]);

				printf("> Trying to connect to Server: %d.%d.%d.%d:8002 ...\n",
					   RemoteIP[0],
					   RemoteIP[1],
					   RemoteIP[2],
					   RemoteIP[3]);
		        while (Trials--) {
		        	if( WIFI_OpenClientConnection(0, WIFI_TCP_PROTOCOL, "TCP_CLIENT", RemoteIP, 8002, 0) == WIFI_STATUS_OK) {
						printf("> TCP Connection opened successfully.\n");
						Socket = 0;
		        	}
		        }
		        if(!Trials) {
		            printf("> ERROR : Cannot open Connection\n");
		            BSP_LED_On(LED2);
		        }
			} else {
		        printf("> ERROR : es-wifi module CANNOT get IP address\n");
		        BSP_LED_On(LED2);
		    }
		} else {
	        printf("> ERROR : es-wifi module NOT connected\n");
	        BSP_LED_On(LED2);
	    }
	} else {
	    printf("> ERROR : WIFI Module cannot be initialized.\n");
	    BSP_LED_On(LED2);
	}
}


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
	uint32_t pulse = pwm_handle.Init.Period * (duty / 100.0);
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
	float duty = 7.5 + ((5.0 / 90.0) * (float)angle);
	pwm_set_duty(duty);
}


void stop()
{
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



///**
//  * @brief Toggle Thread function
//  * @param  argument: Not used
//  * @retval None
//  */
//static void ToggleLedThread(const void *argument)
//{
//	for (;;) {
//		BSP_LED_On(LED2);
//		osDelay(LED_TOGGLE_DELAY);
//		BSP_LED_Off(LED2);
//		osDelay(LED_TOGGLE_DELAY);
//	}
//
//	while (1) {
//		/* Delete the Init Thread */
//		osThreadTerminate(NULL);
//	}
//}


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
