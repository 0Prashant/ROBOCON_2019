/*
 * int_config.cpp
 * 
 * Created : 11/16/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "robot/devs_config.h"

extern bool gSendBufferDataFlag;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
        switch (GPIO_Pin) {
                // This interrupt pin is connected to gXEnc
                case GPIO_PIN_4 : {     // PA4
                        Enc_HandleCount(&gXEnc);
                } break;
                
                // This interrupt pin is connected to gYEnc
                case GPIO_PIN_13 : {    // PE13
                        Enc_HandleCount(&gYEnc);
                } break;

                // This interrupt pin is connected to push button
                case GPIO_PIN_14 : {    // PC14
                        gSendBufferDataFlag = true;
                }
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
