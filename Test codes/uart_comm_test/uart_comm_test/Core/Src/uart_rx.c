#include "uart_rx.h"
void Queue8_Init(struct Queue8 *q)
{
	q->head_ = 0;
	q->tail_ = 0;
	q->curr_size_ = 0;
}

void Queue8_insert(struct Queue8 *q,uint8_t data)
{
	++q->curr_size_;
	q->queue_[q->head_++] = data; 
	q->head_ %= MAX_QUEUE8_SIZE;
}

uint8_t Queue8_lookup(struct Queue8 *q)
{
	if(q->curr_size_)
	{
		--q->curr_size_;
		uint8_t data = q->queue_[q->tail_++];
		q->tail_ %= MAX_QUEUE8_SIZE;
		return data;
	}
	return 0;
}


uint32_t Queue8_length(struct Queue8 *q)
{
	return q->curr_size_;
}

void Queue16_Init(struct Queue16 *q)
{
	q->head_ = 0;
	q->tail_ = 0;
	q->curr_size_ = 0;
}

void Queue16_insert(struct Queue16 *q,int16_t data)
{
	++q->curr_size_;
	q->queue_[q->head_++] = data; 
	q->head_ %= MAX_QUEUE16_SIZE;
}

int16_t Queue16_lookup(struct Queue16 *q)
{
	if(q->curr_size_)
	{
		--q->curr_size_;
		uint8_t data = q->queue_[q->tail_++];
		q->tail_ %= MAX_QUEUE16_SIZE;
		return data;
	}
	return 0;
}


uint32_t Queue16_length(struct Queue16 *q)
{
	return q->curr_size_;
}

