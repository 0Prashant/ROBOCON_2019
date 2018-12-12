#ifndef _PLAY_H_
#define _PLAY_H_

#include "robo_init.h"

void tune(float set_points[4], uint32_t dt_millis);
void update(float set_points[4]);
void ramp_down(uint32_t dt_millis);

#endif // !_PLAY_H_
