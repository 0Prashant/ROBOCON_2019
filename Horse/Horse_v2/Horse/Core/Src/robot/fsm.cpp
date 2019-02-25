/*27-1-2019*/
// 238.34

#include "fsm.h"
#include "fsm_tasks.h"
#include "robo_states.h"

extern State gHorse_State;
extern bool gReady_To_Go;

void startFSM(struct fsmStr *sfsm)
{
        switch (gHorse_State.get_ID())
        {
        case State_ID::HOME:
                homePositionChores(sfsm);
                if (checkHomePosition())
                {
                        printf("PHASE_1\n");
                        stateReset();
                }
                break;

	case State_ID::WS1:
		if (gReady_To_Go)
		{
			phaseOneChores(sfsm);
			if (checkAngle(0))
			{
				printf("PHASE_2\n");
				gReady_To_Go = false;
				gHorse_State.update_State();
				stateReset();

				motor_arr[0].encoder->last_count = motor_arr[0].encoder->htim->Instance->CNT;

				//HAL_Delay(300);
			}
		}
		break;

	case State_ID::WS2:
		if (gReady_To_Go)
		{
			phaseTwoChores(sfsm);
			if (checkAngle(1))
			{
				printf("PHASE_3\n");
				gReady_To_Go = false;

				gHorse_State.update_State();
				stateReset();

				motor_arr[1].encoder->last_count = motor_arr[1].encoder->htim->Instance->CNT;

				//HAL_Delay(300);
			
			}
		}
		break;

	case State_ID::WS3:
		if (gReady_To_Go)
		{
			phaseThreeChores(sfsm);
			if (checkAngle(2))
			{
				printf("PHASE_4\n");
				gReady_To_Go = false;

				gHorse_State.update_State();
				stateReset();

				motor_arr[2].encoder->last_count = motor_arr[2].encoder->htim->Instance->CNT;
			
				//HAL_Delay(300);
			
			}
			break;
		}
	case State_ID::WS4:
		if (gReady_To_Go)
		{
			phaseFourChores(sfsm);
			if (checkAngle(3))
			{
				printf("PHASE_1\n");
				gReady_To_Go = false;

				gHorse_State.update_State();
				stateReset();

				motor_arr[3].encoder->last_count = motor_arr[3].encoder->htim->Instance->CNT;
			
				//HAL_Delay(300);
			
			}
		}
		break;
	}
}
