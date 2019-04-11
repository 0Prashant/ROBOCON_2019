/*
 * robo_states.cpp
 * 
 * Created : 02/18/2019
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "robo_states.h"

extern Node<State_ID> gHomeState;
extern Node<State_ID> gWalkState1;
extern Node<State_ID> gWalkState2;
extern Node<State_ID> gWalkState3;
extern Node<State_ID> gWalkState4;

Node<State_ID> gHomeState(State_ID::HOME, &gWalkState1);
Node<State_ID> gWalkState1(State_ID::WS1, &gWalkState2);
Node<State_ID> gWalkState2(State_ID::WS2, &gWalkState3);
Node<State_ID> gWalkState3(State_ID::WS3, &gWalkState4);
Node<State_ID> gWalkState4(State_ID::WS4, &gWalkState1);


State& State::get_Instance()
{
        static State sRobo_State;

        sRobo_State.state_node_ = &gHomeState;

        return sRobo_State;
}

void State::update_State()
{
        state_node_ = state_node_->get_Next();
}

State_ID State::get_ID()
{
        return state_node_->get_Element();
}

State_ID State::get_NextID()
{
        return state_node_->get_Next()->get_Element();
}
