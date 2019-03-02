#include "uart_tx.h"

uint8_t start_byte= 0X5A;
void transmit_Data(UART_HandleTypeDef* uartHandle, uint8_t *data_array, int no_of_bytes)
{
	//HAL_UART_Transmit_DMA(uartHandle, &start_byte,1);
	HAL_UART_Transmit_DMA(uartHandle, data_array, no_of_bytes);
}

