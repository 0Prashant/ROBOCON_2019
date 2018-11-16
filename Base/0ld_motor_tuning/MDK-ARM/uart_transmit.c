#include "uart_transmit.h"
#include "usart.h"


void Send_Data_To_Second_Microcontroller(void)
{
	uint8_t Start_Bit = 01;
	char data  = 'a';
	uint16_t Data_Set[4] = {12,4537,1,987};
	 HAL_UART_Transmit(&huart3,(uint8_t*)&data,1,10);
	/*HAL_UART_Transmit_DMA(&huart3,&Start_Bit,1);
	for(int i = 0; i < 4;i++)
	{
		HAL_UART_Transmit_DMA(&huart3,(uint8_t*)&Data_Set[i],2);
	}*/

}
