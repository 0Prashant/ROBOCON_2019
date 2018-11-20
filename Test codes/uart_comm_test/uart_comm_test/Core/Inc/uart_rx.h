#ifndef _UART_RX_H_
#define _UART_RX_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

//#define START_BYTE (0XA5)
#define MAX_QUEUE8_SIZE (32)
#define MAX_QUEUE16_SIZE (32)

struct Queue8
{
	uint8_t queue_[MAX_QUEUE8_SIZE];
	uint32_t tail_;
	uint32_t head_;
	uint32_t curr_size_;
};

void Queue8_Init(struct Queue8 *q);
void Queue8_insert(struct Queue8 *q,uint8_t data);
uint8_t Queue8_lookup(struct Queue8 *q);
uint32_t Queue8_length(struct Queue8 *q);

struct Queue16
{
	int16_t queue_[MAX_QUEUE16_SIZE];
	uint32_t tail_;
	uint32_t head_;
	uint32_t curr_size_;
};

void Queue16_Init(struct Queue16 *q);
void Queue16_insert(struct Queue16 *q,int16_t data);
int16_t Queue16_lookup(struct Queue16 *q);
uint32_t Queue16_length(struct Queue16 *q);

#endif
