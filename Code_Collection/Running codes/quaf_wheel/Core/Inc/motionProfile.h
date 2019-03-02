#ifndef _MOTIONPROFILE_H_
#define _MOTIONPROFILE_H_

#define pi (3.141592)
/* Trapezoidal motion profile generator */

enum Status
{
    BUSY,
    COMPLETE
};

struct Trap_struct
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

void  motionProfile_resetParams(struct Trap_struct *t); // reset the index for next generation (if any)
void  motionProfile_Init(struct Trap_struct *t, float _max_omega, float _max_angle, float _percent_div, float _dt);
float motionProfile_Generate(struct Trap_struct *st ,float curr_angle); /* t = current time in milliseconds */
void motionProfile_setAngle(struct Trap_struct *st, float _max_angle);
void motionProfile_restart(struct Trap_struct *t);
void motionProfile_resetAngle(struct Trap_struct *t, int mode_flag);
#endif // _GENERATE_PROFILE_H
