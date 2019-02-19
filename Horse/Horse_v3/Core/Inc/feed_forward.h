/*30-1-2019*/

#ifndef _FEED_FORWARD_H_
#define _FEED_FORWARD_H_

#include "defines.h"
#include <stdio.h>

/* Trapezoidal motion profile generator */
enum Status
{
    ffBUSY,
    ffCOMPLETE
};

struct feedStr
{
    float max_omega;
    float max_angle;
    float percent_div;
    float dt;

    float T;
    float t1;
    float t2;

    float t1_angle;
    float t2_angle;
    float t3_angle;
    
    int total_steps;
    int index;  
    int sign_flag;    
    
    float output_omega ;
    enum Status status;
};

void  ffResetParams(struct feedStr *ff); // reset the index for next generation
void  ffInit(struct feedStr *ff, float _max_omega, float _percent_div, float _dt);
void  ffSetAngle(struct feedStr *ff, float _max_angle);
void  ffRestart(struct feedStr *ff);

float ffGenerate(struct feedStr *ff ,float curr_angle); // t(secs) 

#endif /*feed_forward.h*/
