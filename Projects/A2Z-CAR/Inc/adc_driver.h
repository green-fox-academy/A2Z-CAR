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
uint8_t adc_values[9];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void adc_init();
void adc_12b_init();
int8_t calibrate();
int8_t get_bias();
uint16_t adc_rpm_measure();

#endif /* __ADC_DRIVER_H */
