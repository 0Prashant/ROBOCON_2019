/*22-1-2019*/

#include "robo_init.h"

struct interruptStr blnc_int_arr[4];
struct encoderStr encoder_arr[4];
struct motorStr motor_arr[4];
struct pneumaticStr pneu_arr[4];
struct interruptStr int_arr[4];
struct pidStr pid_arr[4];
struct feedStr ff_arr[4];
struct fsmStr fsm;

struct motorStr balance_motor;

// Motor 1 started working after changing interrupt pin from PE3 to PB9

void roboInit(void)
{
	balance_init();
}

/* /Limit-switches/
    *
    * A = 14  M1 = 3
    * B = 15  M2 = 13
    * C = 7   M3 = 1
    * D = 9   M4 = 5
    *
    * /Encoders/
    *
    * M1 = A6 A7
    * M2 = D12 D13
    * M3 = E9 E11
    * M4 = A1 A5    
    *
    */
