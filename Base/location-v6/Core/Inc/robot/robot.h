#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "robo_states.h"
#include "devs/wheeler.h"
#include "devs/position_sensor.h"

extern ForestA gPointA;
extern ForestH gPointH;

class Robot
{
public:
        Robot() {
                state_ = &gPointA;
                // state_ = &gPointH;
                pos_ = 0;
                is_first_reading_ = true;
                first_heading_ = 0;
        }
        Robot(Robot &&) = default;
        Robot(const Robot &) = default;
        Robot &operator=(Robot &&) = default;
        Robot &operator=(const Robot &) = default;
        ~Robot() { }

        void set_State(Robo_States *state) { state_ = state; }
        Robo_States *get_State() { return state_; }    
        void set_PositionSensor(PositionSensor *pos) { pos_ = pos; }
        PositionSensor *get_PositionSensor() { return pos_; }
        void run(uint32_t dt_millis);
        void goto_absolute_distance(float dx, float dy, float dth, uint32_t dt_milis);

private:
        Robo_States *state_;
        PositionSensor *pos_;
        float velocity_;
        float velocity[3];
        float angle_of_attack_;
        float omegas_[4];
        bool is_first_reading_;
        float first_heading_;
        
        void move(Vec3<float> p, uint32_t dt_millis);
};

#endif // !_ROBOT_H_
