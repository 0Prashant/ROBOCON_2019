/*27-1-2019*/
// 238.34

#include "fsm.h"
#include "fsm_tasks.h"
#include "robo_states.h"

extern State gHorse_State;

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

                phaseOneChores(sfsm);
                if (checkAngle(0))
                {
                        printf("PHASE_2\n");
                        
                        gHorse_State.update_State();
                        stateReset();

                        motor_arr[0].encoder->last_count = motor_arr[0].encoder->htim->Instance->CNT;
                }
                break;

        case State_ID::WS2:

                phaseTwoChores(sfsm);
                if (checkAngle(1))
                {
                        printf("PHASE_3\n");
                        
                        gHorse_State.update_State();
                        stateReset();

                        motor_arr[1].encoder->last_count = motor_arr[1].encoder->htim->Instance->CNT;
                }
                break;

        case State_ID::WS3:

                phaseThreeChores(sfsm);
                if (checkAngle(2))
                {
                        printf("PHASE_4\n");

                        gHorse_State.update_State();
                        stateReset();

                        motor_arr[2].encoder->last_count = motor_arr[2].encoder->htim->Instance->CNT;
                }
                break;

        case State_ID::WS4:

                phaseFourChores(sfsm);
                if (checkAngle(3))
                {
                        printf("PHASE_1\n");
                        
                        gHorse_State.update_State();
                        stateReset();
                        
                        motor_arr[3].encoder->last_count = motor_arr[3].encoder->htim->Instance->CNT;
                }
                break;
        }
}
