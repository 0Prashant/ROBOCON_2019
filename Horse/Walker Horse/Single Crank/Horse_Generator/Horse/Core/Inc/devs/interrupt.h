#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include "main.h"

extern leg leg[1];
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
        switch (GPIO_Pin) {
                case leg[0].limit_switch__[0].int_pin : {
                        leg[0].steps++;
                        leg[0].set_Angle(0);
			leg[0].raised_flag = false;
                } break;

                case leg[0].limit_switch__[1].int_pin : {
                        leg[0].raised_flag = true;
                } break;
        }
}

#endif // !_INTERRUPTS_H_