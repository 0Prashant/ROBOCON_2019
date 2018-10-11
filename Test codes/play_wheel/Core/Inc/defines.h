#ifndef _DEFINES_H_
#define _DEFINES_H_


#define MOTOR_DRIVER_FREQUENCY  (8000)
#define PWM_TIMER_FREQUENCY     (168000000)

#define MAX_RPM         (360)
#define MAX_OMEGA       (0.10472 * MAX_RPM)    // w = 2*pi*f

#endif  // _DEFINES_H_
