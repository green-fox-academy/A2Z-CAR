/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_DRIVER_H
#define __ADC_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
ADC_HandleTypeDef adc_handle;
ADC_ChannelConfTypeDef adc_ch_conf;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void gpio_init();
void adc_init();
int8_t get_bias();
void set_servo();
int8_t calibrate();

//void calibrate(uint8_t *calibration);

uint8_t max_bg[9];
uint8_t adc_values[9];
uint8_t multiplier_percent[9];
int8_t former_bias, bias;

#endif /* __ADC_DRIVER_H */
