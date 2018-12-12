#ifndef _WHEELER_H_
#define _WHEELER_H_

#include "periphs/usart.h"

extern float gWheel_Omegas[4];

void Wheeler_Init();
void send_WheelerPack(float pack[4]);
// void calculate_robot_velocity(float v, float theta, float phi, uint32_t dt_millis);
void Wheeler_UART2_TxCpltCallback();
void calc_WheelsOmegas(float omegas[4], float v, float theta, float phi, uint32_t dt_millis);

#endif // !_WHEELER_H_
