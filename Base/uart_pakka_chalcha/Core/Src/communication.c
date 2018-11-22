#include "communication.h"

#define NUM_OMEGAS_RECEIVED	(6)
#define MAX_POSSIBLE_OMEGA	(50) 

static uint8_t send_flag = 1;
static uint8_t gNavigator_Data;
static uint8_t gStart_Byte_Received = 0;
static uint8_t gNum_Data_Received = 0;
static int16_t gOmega;

static struct Queue8 gNavigator_Bytes;
static struct Queue16 gNavigator_datas[3];
//static uint16_t gu3_datas[4];

extern struct Wheel Wheel_arr[4];

void Navigator_Init(void)
{
	HAL_UART_Receive_DMA(&huart3, &gNavigator_Data, 1);
	Queue8_Init(&gNavigator_Bytes);
	for (uint8_t i = 0; i < 3; ++i) {
		Queue16_Init(&gNavigator_datas[i]);
	}
}

void get_Navigator(float arr[3])
{
	for (uint8_t i= 0; i < 3; ++i) {
		arr[i] = Queue16_lookup(&gNavigator_datas[i]);
		arr[i] *= (MAX_POSSIBLE_OMEGA / 32768.0);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 if(huart->Instance == USART3)
	 {
		if(!gStart_Byte_Received)
		{
			if (gNavigator_Data == 0xA5) {
				gStart_Byte_Received = 1;
			}
		}
		else
		{
			Queue8_insert(&gNavigator_Bytes, gNavigator_Data);
			++gNum_Data_Received;
			if (gNum_Data_Received == NUM_OMEGAS_RECEIVED) {
				gNum_Data_Received = 0;
				gStart_Byte_Received = 0;
				// Omega calculation using little-endian
				for (uint8_t i = 0; i < 3; ++i) {
					gOmega = (uint16_t)(Queue8_lookup(&gNavigator_Bytes) << 8);
					gOmega += (uint16_t)(Queue8_lookup(&gNavigator_Bytes));
					Queue16_insert(&gNavigator_datas[i], gOmega);
				}
			}
		}
	 }
}

void send_velocity_to_u3(void)
{

	float max_velocity = MAX_VELOCITY * 0.99999999999;
	uint8_t sending_buffer[9] = {0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5};
	int16_t data;
	sending_buffer[0] = 0xA5;
	for(int i=0; i<4; i++)
	{
		//Wheel_arr[i].velocity = 1500;
		data = (int16_t)((32768*Wheel_arr[i].velocity) / max_velocity);
		sending_buffer[(i*2)+1] = (uint8_t)(data>>8);
		sending_buffer[(i*2)+2] = (uint8_t)(data);
		//printf("%f\t %f\t %f\t %f\n",Wheel_arr[0].velocity,Wheel_arr[1].velocity,Wheel_arr[2].velocity,Wheel_arr[3].velocity);
	}
		printf("%s\n", sending_buffer);
		if(send_flag)
		{
			send_flag = 0;
			HAL_UART_Transmit_DMA(&huart3, sending_buffer, 9);
		}
		HAL_Delay(10);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART3)
	{
		send_flag = 1;
	}
}





