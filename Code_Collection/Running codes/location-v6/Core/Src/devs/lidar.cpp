/*
 * lidar.cpp
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "periphs/usart.h"

/*
 * Configuring LIDAR for measuring distance
 * => If the pulse emitted by the LIDAR is n us long, then the distance to
 *      obstacle is n mm
 TODO : Send low on Trigger Pin
 TODO : Wait for high to occur in Monitor Pin
 TODO : Start timer
 TODO : Wait for low to occur in Monitor Pin
 TODO : Stop timer
 TODO : Get Counter Value
 */

#define START_BYTE      (0xA5)
#define NUM_DATA        (2)     // For LIDAR

#include "utils/queue.h"
#include "periphs/usart.h"
#include "devs/lidar.h"

static uint8_t gRx_Data;
static Queue<uint8_t, NUM_DATA> gLidar_Bytes;
static uint8_t gNum_Data_Received = 0;
static Queue<uint16_t, 2> gLidar_Data;
static uint16_t gDistance;

static uint16_t gXLidar_Offset = 186;
// static uint16_t gYLidar_Offset = 0;

static bool gStart_Byte_Received = false;

void LIDAR_Init()
{
        HAL_UART_Receive_DMA(&huart3, &gRx_Data, 1);
}

uint16_t LIDAR_Read()
{
        return gLidar_Data.lookup() + gXLidar_Offset;
}

bool LIDAR_IsEmpty()
{
        return (gLidar_Data.length() == 0);
}

void LIDAR_UART3_RxCpltCallback()
{
        if (!gStart_Byte_Received) {
                if (gRx_Data == START_BYTE) {
                        gStart_Byte_Received = true;
                }
        }
        else {
                gLidar_Bytes.insert(gRx_Data);
                ++gNum_Data_Received;
                // If required number of data is received initiate new data;
                if (gNum_Data_Received == NUM_DATA) {
                        gNum_Data_Received = 0;
                        gStart_Byte_Received = false;
                        // For LIDAR
                        gDistance = gLidar_Bytes.lookup() << 8;
                        gDistance |= gLidar_Bytes.lookup();
                        if (gDistance) {
                                gLidar_Data.insert(gDistance);
                        }
                }
        }
}


int Lidar::init()
{
        LIDAR_Init();
        return 0;
}

float Lidar::read()
{
        float dist = (float)(LIDAR_Read());
        return dist;
}

bool Lidar::available()
{
        return !LIDAR_IsEmpty();
}

void Lidar::denit()
{

}
