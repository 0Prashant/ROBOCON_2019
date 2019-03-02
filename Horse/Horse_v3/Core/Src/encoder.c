/* 22-1-2019 */

#include <stdlib.h>
#include <math.h>
#include "tim.h"
#include "encoder.h"
#include "defines.h"
#include "motor.h"
extern struct motorStr balance_motor;

int16_t getCounts(struct encoderStr *encoder)
{
        return encoder->htim->Instance->CNT;
}

float calculateOmega(struct encoderStr *encoder)
{
        /* w = 2 * pi * f
    *   = 2 * pi * counts_per_st / ( PPR * dt ) 
    */
        
        if(encoder == balance_motor.encoder)
        {
                int16_t counts_per_st = encoder->count - encoder->last_count;
                encoder->last_count = encoder->count;
                return 2 * PI * counts_per_st / (encoder->ppr * dt_secs);
        }
        else
        {
                int16_t counts_per_st = encoder->htim->Instance->CNT - encoder->last_count;
                encoder->last_count = encoder->htim->Instance->CNT;
                return 2 * PI * counts_per_st / (encoder->ppr * dt_secs);                   
        }
}

float calculateAngle(struct encoderStr *encoder)
{
        float angle;

        // angle = (360 / PPR) * no_of_counts
        float mult_factor = 360.0 / encoder->ppr;
        int16_t inst_count = encoder->htim->Instance->CNT;
        int16_t counts = inst_count - encoder->ref_count;
        angle = mult_factor * counts;

        // Normalize: 360 - (-360)
        while (fabs(angle) > 360)
        {
                if ((fabs(angle) - (angle * (-1))) < 0.00001)
                        angle += 360;
                else
                {
                        angle -= 360;
                }
        }

        return angle;
}

void resetEncoderAngle(struct encoderStr *encoder)
{
        encoder->ref_count = encoder->htim->Instance->CNT;
}
