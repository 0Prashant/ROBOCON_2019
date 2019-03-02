/*27-1-2019*/

#ifndef _FSM_H_
#define _FSM_H_

#include "check.h"

struct fsmStr
{
    uint8_t loop_cnt;
};

enum FSM_States
{
    HOME_POSITION,
    PHASE_1,
    PHASE_2,
    PHASE_3,
    PHASE_4,
};    
 

void startFSM(struct fsmStr *fsm);

#endif /*fsm.h*/
