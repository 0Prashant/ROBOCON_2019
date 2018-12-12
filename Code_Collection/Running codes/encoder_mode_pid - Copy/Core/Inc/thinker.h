#ifndef _THINKER_H_
#define _THINKER_H_

#include "stm32f4xx_hal.h"


void Thinker_Init(void);
void get_Thinker(float arr[4]);
uint8_t Thinker_isEmpty();

#endif //!_THINKER_H_
