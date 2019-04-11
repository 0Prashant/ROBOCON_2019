/*1-23-2019*/

#include"pneumatic.h"

void setPneumatic(struct pneumaticStr *pneu,enum pneumaticState pneu_state )
{
    if(pneu_state == SET_PNEU)
    {
        HAL_GPIO_WritePin(pneu->pneu_port, pneu->pneu_pin, GPIO_PIN_SET);    
    }
    
    else if(pneu_state == RESET_PNEU)
    {
        HAL_GPIO_WritePin(pneu->pneu_port, pneu->pneu_pin, GPIO_PIN_RESET);    
    }
    
    else
    {
        // error
    }
}
