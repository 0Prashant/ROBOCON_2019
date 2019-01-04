#ifndef _HORSE_H_
#define _HORSE_H_

#include "motionProfile.h"
#include "robo_init.h"
#include "leg.h"

#define HORSE_DELAY 1000
#define SAMPLE_TIME 10

struct Leg_index
{
  int index ;
  int raise_flag ;      
};

void horse_calibrate(void);
void goto_homePosition(struct Leg_str *leg);
void raiseLeg(struct Leg_str *leg); 
//void lowerLeg(struct Leg_str *leg);
void horse_Gallop(void);

#endif /*_HORSE_H_*/

