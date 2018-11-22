#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include "usart.h"
#include "queue.h"
#include "defines.h"
#include "wheel.h"

void Navigator_Init(void);
void get_Navigator(float arr[4]);
void send_velocity_to_u3(void);

#endif //COMMUNICATION_H

