/*
 * encoder.h
 * 
 * Created : 11/16/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "devs/encoder.h"

void Encoder_HandleCount(struct Encoder *enc)
{
        if (HAL_GPIO_ReadPin(enc->chB_port, enc->chB_pin)) {
                ++enc->count;
        } else {
                --enc->count;
        }
}

int32_t Encoder_get_DeltaCount(struct Encoder *enc)
{
        uint32_t last_count = enc->count;
        enc->count = 0;
        return last_count;
}
