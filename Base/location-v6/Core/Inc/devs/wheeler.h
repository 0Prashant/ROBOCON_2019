#ifndef _WHEELER_H_
#define _WHEELER_H_

#include "periphs/usart.h"

#define MAX_VELOCITY (2400)
#define ROBOT_VELOCITY (300)
#define MAX_OMEGA (35)

extern float gWheel_Omegas[4];

struct Wheel
{
    uint8_t id;
    double next_velocity;
    double velocity;
};
void wheel_init();

void Wheeler_Init();
void send_WheelerPack(float pack[4]);
// void calculate_robot_velocity(float v, float theta, float phi, uint32_t dt_millis);
void Wheeler_UART2_TxCpltCallback();
void calc_WheelsOmegas(float omegas[4], float v, float theta, float phi, uint32_t dt_millis);
void calculate_robot_velocity(float omegas_[4], float velocity[3]);
float initial_ramp(uint8_t wheel_no);
float ramp(uint8_t wheel_no);

#endif // !_WHEELER_H_