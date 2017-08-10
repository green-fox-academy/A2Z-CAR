/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TEST2_H
#define __TEST2_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
UART_HandleTypeDef uart_handle;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void uart_init();

#endif /* __TEST2_H */
