#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "gpio.h"
#include "stm32f4xx_hal.h"

enum interruptState
{
        STATE_A,
        STATE_B,
        STATE_HOME
};

struct interruptStr
{
        uint8_t fangle_reset;
        uint8_t fhome_pos;

        GPIO_TypeDef *int_port;
        uint16_t int_pin;

        enum interruptState intState;
};

#endif /*interrupt.h*/
