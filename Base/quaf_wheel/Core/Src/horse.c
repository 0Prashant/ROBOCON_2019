#include "horse.h"

 struct Trap_struct  profile_arr[4];
 struct Leg_index    present_leg;


void horse_calibrate()
{
    /* set home position */
}

void goto_homePosition(struct Leg_str *leg)
{
    /* use motion profile generator*/
}

void raiseLeg(struct Leg_str *leg)
{
    set_PneuPosition(leg, ACTUATE);
    HAL_Delay(HORSE_DELAY); 

    HAL_Delay(HORSE_DELAY);
    goto_homePosition(leg);
}
void lowerLeg(struct Leg_str *leg)
{
    set_PneuPosition(leg, DEACTUATE);
    HAL_Delay(HORSE_DELAY); 
}

void horse_Gallop()
{
  /*gallop in cycle*/
  int leg_index;
  leg_index = present_leg.index; /* leg that is raised */ 
  int i;                         /* index for remaining legs */ 
 
  if( present_leg.raise_flag == 1 )
  {
      leg_arr[leg_index].omega = motionProfile_Generate(&profile_arr[i], leg_arr[leg_index].curr_angle );
  }      
  
  else
  {
        while(leg_index < 4)
      {   
        raiseLeg(&leg_arr[leg_index]);
        i = leg_index + 1;
        
        /* Calculate omega for three legs once the one is raised*/
        for (int j = 0; j < 3 ; j++) 
        { 
            leg_arr[i].omega = motionProfile_Generate(&profile_arr[i], leg_arr[i].curr_angle );
            if (profile_arr[i].status == 1) 
            {
                present_leg.index ++; 
            }      
            i = ( i + 1 ) % 4;
        }
      }    
  }
  lowerLeg(&leg_arr[leg_index]);  
  leg_index++;      
} 

