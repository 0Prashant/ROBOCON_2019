#ifndef _UART_TX_H_
#define _UART_TX_H_

#define START_BYTE 0XA5

#include "stm32f4xx_hal.h"

void transmit_Data(UART_HandleTypeDef* uartHandle, uint8_t *data_array, int no_of_bytes);

#endif
