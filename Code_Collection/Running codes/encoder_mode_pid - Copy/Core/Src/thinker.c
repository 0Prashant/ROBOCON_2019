#include "thinker.h"
#include "queue.h"
#include "usart.h"

#define NUM_OMEGAS_RECEIVED	(8)
#define MAX_POSSIBLE_OMEGA	(40)

static uint8_t gThinker_Data;
static uint8_t gStart_Byte_Received = 0;
static uint8_t gNum_Data_Received = 0;
static int16_t gOmega;

static struct Queue8 gThinker_Bytes;
static struct Queue16 gThinker_Omegas[4];

void Thinker_Init(void)
{
	HAL_UART_Receive_DMA(&huart3, &gThinker_Data, 1);
	Queue8_Init(&gThinker_Bytes);
	for (uint8_t i = 0; i < 4; ++i) {
		Queue16_Init(&gThinker_Omegas[i]);
	}
}

void get_Thinker(float arr[4])
{
	for (uint8_t i= 0; i < 4; ++i) {
		arr[i] = Queue16_lookup(&gThinker_Omegas[i]);
		arr[i] *= (MAX_POSSIBLE_OMEGA / 32768.0);
	}
}

uint8_t Thinker_isEmpty()
{
    if (Queue16_length(&gThinker_Omegas[0])) {
        return 0;
    }
    return 1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 if(huart->Instance == USART3)
	 {
		if(!gStart_Byte_Received)
		{
			if (gThinker_Data == 0xA5) {
				gStart_Byte_Received = 1;
			}
		}
		else
		{
			Queue8_insert(&gThinker_Bytes, gThinker_Data);
			++gNum_Data_Received;
			if (gNum_Data_Received == NUM_OMEGAS_RECEIVED) {
				gNum_Data_Received = 0;
				gStart_Byte_Received = 0;
				// Omega calculation using little-endian
				for (uint8_t i = 0; i < 4; ++i) {
					gOmega = (uint16_t)(Queue8_lookup(&gThinker_Bytes) << 8);
					gOmega += (uint16_t)(Queue8_lookup(&gThinker_Bytes));
					Queue16_insert(&gThinker_Omegas[i], gOmega);
				}
			}
		}
	 }
}
