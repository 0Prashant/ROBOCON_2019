/*
 * robo_states.h
 * 
 * Created : 02/18/2019
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _ROBO_STATES_H_
#define _ROBO_STATES_H_

#include "node.h"

enum class State_ID
{
        HOME,
        WS1,
        WS2,
        WS3,
        WS4
};


class State
{
public:
        State(State &&) = default;
        State(const State &) = default;
        State &operator=(State &&) = default;
        State &operator=(const State &) = default;
        ~State() { }

        static State& get_Instance();
        void update_State();
        State_ID get_ID();
        State_ID get_NextID();

private:
        Node<State_ID> *state_node_;
        State() { }
};

#endif // !_ROBO_STATES_H_
