/*
 * thinker.cpp
 * 
 * Created : 11/20/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "devs/thinker.h"

#define NUM_THINKER_BYTES       (6)

static uint8_t gStart_Byte = 0xA5;
static bool gThinker_Start_Byte_Sent = false;
static Queue<int16_t, 32> gThinker_Data[3];
uint8_t gThinker_Bytes[NUM_THINKER_BYTES];

void Thinker_Init()
{
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_TC);
}

void send_ThinkerPack(int16_t pack[3])
{
        // Data are as follows :
        // -> x-position        // Only positive values
        // -> y-position        // "
        // -> heading           // 0 to 360
        for (uint8_t i = 0; i < 3; ++i) {
                gThinker_Data[i].insert(pack[i]);
        }
        if (!gThinker_Start_Byte_Sent) {
                gThinker_Start_Byte_Sent = true;
                HAL_UART_Transmit_IT(&huart2, &gStart_Byte, 1);
        }
}

void Thinker_UART2_TxCpltCallback()
{
        if (gThinker_Start_Byte_Sent) {
                gThinker_Start_Byte_Sent = false;
                int16_t data;
                for (uint8_t i = 0; i < 3; ++i) {
                        data = gThinker_Data[i].lookup();
                        gThinker_Bytes[2*i + 0] = (uint8_t)(data >> 8);
                        gThinker_Bytes[2*i + 1] = (uint8_t)(data);
                }
                HAL_UART_Transmit_IT(&huart2, gThinker_Bytes, 6);
        }
        else {
                // All thinker buffers contain equal length of data
                if (gThinker_Data[0].length()) {
                        gThinker_Start_Byte_Sent = true;
                        HAL_UART_Transmit_IT(&huart2, &gStart_Byte, 1);
                }
        }
}
