/*23-1-2019*/

#ifndef _PNEUMATIC_H_
#define _PNEUMATIC_H_

#include "stm32f4xx_hal.h"

enum pneumaticState
{
    SET_PNEU,
    RESET_PNEU
};

struct pneumaticStr
{
    GPIO_TypeDef *pneu_port;
    uint16_t pneu_pin;
};

void setPneumatic(struct pneumaticStr *pneu, enum pneumaticState pneu_state);


#endif /*pneumatic.h*/
