/*28-1-2019*/
/*Functions used in FSM*/

#ifndef _FSM_TASKS_H_
#define _FSM_TASKS_H_

#include "defines.h"
#include "stm32f4xx_hal.h"
#include "feed_forward.h"
#include "robo_init.h"
#include "motor.h"
#include "fsm.h"
#include "pid.h"
 
extern struct fsmStr fsm;
void homePositionChores(struct fsmStr* fsm); 
void phaseOneChores(struct fsmStr* fsm);
void phaseTwoChores(struct fsmStr* fsm);
void phaseThreeChores(struct fsmStr* fsm);
void phaseFourChores(struct fsmStr* fsm);

void resetPID(void);
void stateReset(struct fsmStr *fsm,const enum FSM_States *state);
void setHomePosition(void);
void resetAngles(void);
void updateOmegas(void);
void check_N_Stop(void);
void omegaReset(void);
void feedForwardRestart(void);
void resetIntState(struct fsmStr *fsm, const enum FSM_States *state);

#endif /*fsm_tasks.h*/
