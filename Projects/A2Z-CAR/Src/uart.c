
#include "uart.h"

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

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
}

PUTCHAR_PROTOTYPE	// Retargets the C library printf function to the USART.
{
  HAL_UART_Transmit(&uart_handle, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

