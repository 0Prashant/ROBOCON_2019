#ifndef _THINKER_H_
#define _THINKER_H_

#include "stm32f4xx_hal.h"


void Thinker_Init(void);
void get_Thinker(float arr[4]);
bool Thinker_isEmpty();

void Thinker_UART_RxCpltCallback();

#endif //!_THINKER_H_
