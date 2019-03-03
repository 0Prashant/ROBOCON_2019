/*
 * robot.cpp
 *
 * Created : 12/31/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "robot.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"


extern State_Vars gStateA_Data;

// We should make sure that the robot ony have one instance and it is properly
// instantiated
Robot& Robot::get_Instance()
{
        static Robot sRobo_Instance;

        // Since all the parts follow singleton pattern, we should create them
        // when the robot is instantiated
        State_Sensor &stsn = State_Sensor::get_Instance();
        sRobo_Instance.sensor_ = &stsn;

        Processor &proc = Processor::get_Instance(sRobo_Instance.sensor_);
        sRobo_Instance.cpu_ = &proc;

        Actuator &act = Actuator::get_Instance();
        sRobo_Instance.base_ = &act;
        
        return sRobo_Instance;
}

int Robot::init(uint32_t dt_millis)
{
        int sensor_status = sensor_->init(dt_millis);
        int base_status = base_->init();
        int cpu_status = cpu_->init(dt_millis);

        int status = (base_status | sensor_status | cpu_status);

        initiated_ = true;
        robot_state_vars_ = &gStateA_Data;

        return status;
}

// This function is called by the RobotThread
void Robot::update(uint32_t dt_millis)
{
        state_ = sensor_->read_State(state_from_base_,robot_state_vars_, dt_millis);
        // state_.print();
        // printf("\n");
        Vec3<float> vels = cpu_->process(state_, robot_state_vars_, dt_millis);

        // vels.print();
        // printf("\n");

        // This is for correcting units and the inverted co-ordinate system
        float vx = -vels.getX() / (float)1000.0;
        float vy = vels.getY()  / (float)1000.0;
        // float rw = vels.getZ()  / (float)1000.0;
        vels.setX(vx);
        vels.setY(vy);
        // vels.setZ(rw);
        // vels.print();
        // printf("\n");
        // Vec3<float> vels(0, -5, 0.5);

        taskENTER_CRITICAL();
        velocities_ = vels;
        taskEXIT_CRITICAL();
}

// This function is called by the MotorThread
void Robot::run(uint32_t dt_millis)
{
        // velocities_.set_Values(0, 0.5, 0);
        // (velocities_.mult_EW(1000)).print();
        // printf("\n");
        state_from_base_ = base_->actuate(velocities_, dt_millis);
}

bool Robot::is_Initiated() const
{
        return initiated_;
}

void Robot::profile_Actuators(Vec3<float> vel, uint32_t dt_millis)
{
        base_->profile(vel, dt_millis);
}

void Robot::check_Actuators()
{
        base_->check();
}