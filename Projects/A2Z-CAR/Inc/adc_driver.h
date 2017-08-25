/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_DRIVER_H
#define __ADC_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
int8_t former_bias;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void adc_init();
void adc_12b_init();
int8_t calibrate();
int8_t get_bias();
uint8_t max_bg[9];
uint8_t adc_values[9];
uint8_t multiplier_percent[9];

#endif /* __ADC_DRIVER_H */
