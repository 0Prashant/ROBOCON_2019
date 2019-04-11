#ifndef _PNEUMATIC_H_
#define _PNEUMATIC_H_

#include "stm32f4xx_hal.h"
/*function to initialize four pneumatics*/
void pneu_Init(void);
/*function to read the state of the system and actuate pneumatic accordingly*/
void pneu_Loop(void);

#endif /*pneumatic.h*/