/*
 * encoder.h
 * 
 * Created : 11/16/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "devs/encoder.h"

#define PULSE_PER_REVOLUTION    (200)
#define FREEWHEEL_RADIUS        (32)

void Enc_HandleCount(struct Enc *enc)
{
        if (HAL_GPIO_ReadPin(enc->chB_port, enc->chB_pin)) {
                ++enc->count;
        } else {
                --enc->count;
        }
}

static int32_t Enc_get_DeltaCount(struct Enc *enc)
{
        uint32_t last_count = enc->count;
        enc->count = 0;
        return last_count;
}


int Encoder::init()
{
        return 0;
}

float Encoder::read()
{
        int32_t counts = Enc_get_DeltaCount(enc_);
        // return (float)counts;
        float dist = ((float)counts / (float)PULSE_PER_REVOLUTION) * 2 * 3.14159 * (enc_->radius);
        return dist;
}

bool Encoder::available()
{
        return true;
}

void Encoder::denit()
{
        
}
