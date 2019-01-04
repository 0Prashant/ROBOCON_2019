#include "devs/thinker.h"

extern bool gSendDataFlagReceived;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
        switch (GPIO_Pin) {
                case GPIO_PIN_14 : {    // PC14
                        gSendDataFlagReceived = true;
                } break;
        }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 if(huart->Instance == USART3)
	 {
                 Thinker_UART_RxCpltCallback();
	 }
}
