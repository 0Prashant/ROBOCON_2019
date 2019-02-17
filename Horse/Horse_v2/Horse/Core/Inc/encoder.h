/*22-1-2019*/

#include "stm32f4xx_hal.h"

#ifndef _ENCODER_H_
#define _ENCODER_H_

struct encoderStr
{
   uint16_t ppr;
       
   // For calculation of omega and angle
   int16_t ref_count;
   int16_t last_count;
    
   TIM_HandleTypeDef *htim;       
};

void  resetEncoderAngle(struct encoderStr *encoder);
float calculateOmega(struct encoderStr *encoder);
float calculateAngle(struct encoderStr *encoder);
int16_t getCounts(struct encoderStr *encoder);

uint8_t PPR_Multiplier(struct encoderStr *encoder);

#endif /*encoder.h*/
