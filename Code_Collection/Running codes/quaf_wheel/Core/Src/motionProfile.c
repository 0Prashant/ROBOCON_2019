
#include "motionProfile.h"
#include <stdio.h>

/*define the maximum anzgle in radians*/

/*t1_angle is the up-to which the motor ramps up. After this value, the motor runs at constant velocity, velocity given by omega */
/*t2_angle is the angle up-to which the motor runs at constant velocity. After this value the motor starts to ramp down */


/* t1 is the time up-to which the motor ramps up. After this time, the motor runs at constant velocity */
/* t2 is the time up-to which the motor runs at constant velocity. After this time,the motor starts to ramp down */

void motionProfile_Init(struct Trap_struct *t, float _max_omega, float _max_angle, float _percent_div, float _dt) 
{
	/* Initializations */
    t->sign_flag = 1;
    if(_max_angle < 0)
    {
        _max_angle = -1 * _max_angle ;
        t->sign_flag = -1;
    }
    
    
	t->max_omega  	= _max_omega  ;
	t->max_angle  	= _max_angle  ;
	t->percent_div	= _percent_div ; 
	t->dt = _dt ;
	t->index = 0;
    t->status = BUSY;
    
    if(_max_angle == 0)
        t->status =  COMPLETE;
   

}

void motionProfile_setAngle(struct Trap_struct *st, float _max_angle)
{
        
    st->sign_flag = 1;
   
    if(_max_angle < 0)
    {
        _max_angle = -1 * _max_angle ;
        st->sign_flag = -1;
        
    }
    if(_max_angle == 0)
        st->status =  COMPLETE;

	float max_angle_rad = ( _max_angle * pi ) / 180 ;

    st->max_angle = _max_angle;
	st->T         = max_angle_rad  / st->max_omega ; /*change*/ 
	st->t1        = ( st->percent_div * st->T ) / 100.0 ;
	st->t2 = ( 100 - st->percent_div ) * st->T / 100.0 ;
    
}


float motionProfile_Generate(struct Trap_struct *st ,float curr_angle)
{
	float new_omega ;
	float t = st->index * st->dt ;
    while(st->status == BUSY)
    {
        
        if( (t > 0) && (t < st->t1) )
        {
            new_omega  = ( st->max_omega * t ) / st->t1 ;
        }
        
        else if( (t > st->t1 && (t < st->t2) ))
        {
            new_omega =  st->max_omega ;
        }
        
        else if ( (t > st->t2) && (t < st->T))
        {
            new_omega = st->max_omega * ( t - st->T ) / ( st->t2 - st->T );
        }
        
        if ((t > st->T) || (curr_angle > st->max_angle)) 
        {
            motionProfile_resetParams(st);
            new_omega = 0;
            st->status = COMPLETE ;
           
        }
        
        st->index++ ; 
        return new_omega * st->sign_flag;
    }        
    return 0;    	
}

void motionProfile_restart(struct Trap_struct *t)
{
    t->status = BUSY;
}

void motionProfile_resetParams(struct Trap_struct *t)
{
	t->index = 0; 
        
}

void motionProfile_resetAngle(struct Trap_struct *t, int _max_angle)
{
     t->sign_flag = 1;
    
    if(_max_angle == 0)
        t->status =  COMPLETE;
    
    if(_max_angle < 0)
    {
        _max_angle = -1 * _max_angle ;
        t->sign_flag = -1;
    }      
}


