#ifndef _BLNC_MOTOR
#define  _BLNC_MOTOR

#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "interrupt.h"

enum class State_ID {HOME, WS1, WS2, WS3, WS4};
class State
{
public:
	State() { }
	State(State &&) = default;
	State(const State &) = default;
	State &operator=(State &&) = default;
	State &operator=(const State &) = default;
	~State() { }

	State_ID get_ID() { return State_ID::HOME; }
	State_ID get_NextID() { return State_ID::HOME; }

private:	
};
struct Blnc
{
	struct motorStr motor;
	struct interruptStr interrupt[4];
	struct pidStr pid;
};

void balance_init(void);
void check_N_run(void);

#endif // !_BLNC_MOTOR

