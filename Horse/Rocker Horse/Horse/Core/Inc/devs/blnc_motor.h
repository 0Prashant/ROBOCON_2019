#ifndef _BLNC_MOTOR
#define  _BLNC_MOTOR

#include "motor.h"
#include "interrupt.h"
#include "pid.h"

struct Blnc
{
	struct motorStr motor;
	struct interruptStr interrupt[4];
	struct pidStr pid;
};

void balance_Init(void);
void balance_Loop(void);
float get_BalletOmega();

#endif // !_BLNC_MOTOR
