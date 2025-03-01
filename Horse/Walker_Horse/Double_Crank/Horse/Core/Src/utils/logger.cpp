/*
 * logger.cpp
 *
 * Created : 5/21/2019
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "stm32f4xx_hal.h"
#include "logger.h"
#include "defines.h"

Queue<uint8_t, LOG_BUFFER_SIZE> gLogging_Buffer;
static bool gLogDataSent = true;

void log_data()
{
        if (gLogDataSent && !gLogging_Buffer.is_Empty()) {
                gLogDataSent = false;
                uint8_t data = gLogging_Buffer.lookup();
                HAL_UART_Transmit_IT(&huart2, &data, 1);
                for (uint32_t i = 500; i; --i) {
                        asm volatile("nop");
                }
        }
}

uint8_t gSent_Count = 0;
void Logging_Handle_TxCplt()
{
        gLogDataSent = true;
}

//* Angle_Logging Function
void log_Angle(float psi, float rw)
{
        // Angle Packet is of the following form
        // START_BYTE  ANGLE_PACKET_ID  PSI  NEW_PSI
        // A total of 4 bytes including start byte

        gLogging_Buffer.insert((uint8_t)(START_BYTE));
        // gLogging_Buffer.insert((uint8_t)(ANGLE_PACKET_ID));
        gLogging_Buffer.insert((int8_t)(psi * 20));
        gLogging_Buffer.insert((int8_t)(rw * 25));
}
