/*
 * int_config.cpp
 * 
 * Created : 11/16/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "robot/devs_config.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
        switch (GPIO_Pin) {
                // This interrupt pin is connected to gXEncoder
                case GPIO_PIN_4 : {     // PA4
                        Encoder_HandleCount(&gXEncoder);
                } break;
                
                // This interrupt pin is connected to gYEncoder
                case GPIO_PIN_13 : {    // PD13
                        Encoder_HandleCount(&gYEncoder);
                } break;
        }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
        if (huart->Instance == USART3) {
                LIDAR_UART3_RxCpltCallback();
        }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
        if (huart->Instance == USART2) {
                Wheeler_UART2_TxCpltCallback();
        }
}
