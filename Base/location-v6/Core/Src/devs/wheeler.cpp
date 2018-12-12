#include "devs/wheeler.h"
#include <math.h>
#include "utils/queue.h"

#define WHEEL_RADIUS            (67.5)
#define MAX_POSSIBLE_OMEGA      (40)

const float gSpeed_Multiplier = 1;

float gWheel_Velocities[4];
float gWheel_Omegas[4];
const int gCoupling_Matrix[4][3] = {{ 1,  1, 1},
                                    { 1, -1, 1},
                                    {-1, -1, 1},
                                    {-1,  1, 1}};
                                    

#define NUM_WHEELER_BYTES       (8)
#define THETA_NOT               (1.570796)

static uint8_t gStart_Byte = 0xA5;
static bool gWheeler_Start_Byte_Sent = false;
static Queue<int16_t, 2> gWheeler_Data[NUM_WHEELER_BYTES / 2];
uint8_t gWheeler_Bytes[NUM_WHEELER_BYTES];

void Wheeler_Init()
{
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_TC);
}

static float calculate_Omega(float velocity, float radius)
{
        return (velocity) / (radius);
}

void calc_WheelsOmegas(float omegas[4], float v, float theta, float phi, uint32_t dt_millis)
{
        float vel[4];
        theta += THETA_NOT;
        float velocity[3] = {v * cos(theta), v * sin(theta), -3*phi * 1000 / (float)(dt_millis)};

        for (int i = 0; i < 4; ++i) {
                vel[i] = 0;
                for (int j = 0; j < 3; ++j) {
                        vel[i] += velocity[j] * gCoupling_Matrix[i][j];
                }
                omegas[i] = calculate_Omega(vel[i], WHEEL_RADIUS);
        }
}

// void calculate_robot_velocity(float v, float theta, float phi, uint32_t dt_millis)
// {
        // float velocity[3] = {v * cos(theta) * cos(phi), v * sin(theta) * cos(phi), -phi * 1000 / (float)(dt_millis)};

        // for (int i = 0; i < 4; ++i) {
        //         gWheel_Velocities[i] = 0;
        //         for (int j = 0; j < 3; ++j) {
        //                 gWheel_Velocities[i] += velocity[j] * gCoupling_Matrix[i][j];
        //         }
        //         gWheel_Omegas[i] = calculate_Omega(gWheel_Velocities[i]);
        // }
// }

void send_WheelerPack(float pack[NUM_WHEELER_BYTES / 2])
{
        int16_t data;
        // Data are omegas of four motors
        for (uint8_t i = 0; i < NUM_WHEELER_BYTES / 2; ++i) {
                data = (int16_t)((pack[i] * 32768.0) / (MAX_POSSIBLE_OMEGA));
                gWheeler_Data[i].insert(data);
        }
        if (!gWheeler_Start_Byte_Sent) {
                gWheeler_Start_Byte_Sent = true;
                HAL_UART_Transmit_IT(&huart2, &gStart_Byte, 1);
        }
}

void Wheeler_UART2_TxCpltCallback()
{
        if (gWheeler_Start_Byte_Sent) {
                gWheeler_Start_Byte_Sent = false;
                int16_t data;
                for (uint8_t i = 0; i < NUM_WHEELER_BYTES / 2; ++i) {
                        data = gWheeler_Data[i].lookup();
                        gWheeler_Bytes[2*i + 0] = (uint8_t)(data >> 8);
                        gWheeler_Bytes[2*i + 1] = (uint8_t)(data);
                }
                HAL_UART_Transmit_IT(&huart2, gWheeler_Bytes, NUM_WHEELER_BYTES);
        }
        else {
                // All Wheeler buffers contain equal length of data
                if (gWheeler_Data[0].length()) {
                        gWheeler_Start_Byte_Sent = true;
                        HAL_UART_Transmit_IT(&huart2, &gStart_Byte, 1);
                }
        }
}
