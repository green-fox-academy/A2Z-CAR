/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_DRIVER_H
#define __ADC_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
ADC_HandleTypeDef adc_handle;
ADC_HandleTypeDef adc_12b_handle;
ADC_ChannelConfTypeDef adc_ch_conf;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void adc_init();
void adc_12b_init();
int8_t get_bias();
void set_servo();
uint16_t adc_12b_measure();

uint8_t adc_values[9];

#endif /* __ADC_DRIVER_H */
