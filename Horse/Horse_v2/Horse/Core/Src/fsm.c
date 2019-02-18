/*27-1-2019*/
// 238.34

#include "fsm.h"
#include "fsm_tasks.h"

void startFSM(struct fsmStr *sfsm, enum FSM_States *phase)
{
        switch (*phase)
        {
        case HOME_POSITION:
                homePositionChores(sfsm);
                if (checkHomePosition())
                {
                        printf("PHASE_1\n");
                        stateReset(sfsm, phase);

                        setPneumatic(&pneu_arr[3], RESET_PNEU);
                        setPneumatic(&pneu_arr[0], SET_PNEU);
                        // *phase = PHASE_1;
                }
                break;

        case PHASE_1:

                phaseOneChores(sfsm);
                if (checkAngle(0))
                {
                        printf("PHASE_2\n");

                        setPneumatic(&pneu_arr[0], RESET_PNEU);
                        setPneumatic(&pneu_arr[1], SET_PNEU);

			*phase = PHASE_2;
                        stateReset(sfsm, phase);
                        motor_arr[0].encoder->last_count = motor_arr[0].encoder->htim->Instance->CNT;

                }
                break;

        case PHASE_2:

                phaseTwoChores(sfsm);
                if (checkAngle(1))
                {
                        printf("PHASE_3\n");

                        setPneumatic(&pneu_arr[1], RESET_PNEU);
                        setPneumatic(&pneu_arr[2], SET_PNEU);
                        
                        *phase = PHASE_3;
                        stateReset(sfsm, phase);
                        motor_arr[1].encoder->last_count = motor_arr[1].encoder->htim->Instance->CNT;
                }
                break;

        case PHASE_3:

                phaseThreeChores(sfsm);
                if (checkAngle(2))
                {
                        printf("PHASE_4\n");

                        setPneumatic(&pneu_arr[2], RESET_PNEU);
                        setPneumatic(&pneu_arr[3], SET_PNEU);
                        *phase = PHASE_4;
                        stateReset(sfsm, phase);
                        motor_arr[2].encoder->last_count = motor_arr[2].encoder->htim->Instance->CNT;
                }
                break;

        case PHASE_4:

                phaseFourChores(sfsm);
                if (checkAngle(3))
                {
                        printf("PHASE_1\n");

                        setPneumatic(&pneu_arr[3], RESET_PNEU);
                        setPneumatic(&pneu_arr[0], SET_PNEU);
                        *phase = PHASE_1;
                        stateReset(sfsm, phase);
                        
                        motor_arr[3].encoder->last_count = motor_arr[3].encoder->htim->Instance->CNT;
                }
                break;
        }
}
