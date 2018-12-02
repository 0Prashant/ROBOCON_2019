/*
 * encoder.h
 * 
 * Created : 11/16/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "periphs/gpio.h"

struct Encoder
{
        GPIO_TypeDef *chB_port;
        uint16_t chB_pin;
        int32_t count;
};

void Encoder_HandleCount(struct Encoder *enc);
int32_t Encoder_get_DeltaCount(struct Encoder *enc);

#endif // !_ENCODER_H_
