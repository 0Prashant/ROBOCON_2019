#include "robot/robo_states.h"
#include "robot/robot.h"
#include "devs/encoder.h"
#include "devs/lidar.h"


extern ForestA gPointA;
extern ForestB gPointB;
extern ForestC gPointC;
extern ForestD gPointD;
extern ForestE gPointE;
extern ForestF gPointF;
extern ForestG gPointG;
extern ForestH gPointH;
extern ForestI gPointI;
extern ForestJ gPointJ;
extern StopRobot gStop;

ForestA::ForestA()
{
        x1_ = 1000;
        y1_ = 1000;
        tolerance_ = 100;
        next_state_ = &gPointB;
        dist_kp_ = 0.33;
}

ForestB::ForestB()
{
        x1_ = 1900;
        y1_ = 1500;
        tolerance_ = 200;
        next_state_ = &gPointC;
}

ForestC::ForestC()
{
        x1_ = 1900;
        y1_ = 2600;
        tolerance_ = 300;
        next_state_ = &gPointD;
}

ForestD::ForestD()
{
        // // For speed : 500
        // x1_ = 600;
        // y1_ = 3500;
        // tolerance_ = 200;
        // next_state_ = &gPointE;
        
        // For speed : 1000
        x1_ = 700;
        y1_ = 2800;
        tolerance_ = 200;
        next_state_ = &gPointE;
}

ForestE::ForestE()
{
        // // For speed : 500
        // x1_ = 500;
        // y1_ = 4000;
        // tolerance_ = 200;
        // next_state_ = &gPointF;
        
        // For speed : 1000
        x1_ = 700;
        y1_ = 4100;
        tolerance_ = 200;
        next_state_ = &gPointF;
        dist_kp_ = 0.8;
}

ForestF::ForestF()
{
        x1_ = 1800;
        y1_ = 4500;
        tolerance_ = 200;
        next_state_ = &gPointG;
}

ForestG::ForestG()
{
        x1_ = 1800;
        y1_ = 5600;
        tolerance_ = 200;
        next_state_ = &gPointH;
}

ForestH::ForestH()
{
        x1_ = 1250;
        y1_ = 6200;
        tolerance_ = 100;
        next_state_ = &gPointI;
}

ForestI::ForestI()
{
        x1_ = 1300;
        y1_ = 8700;
        tolerance_ = 150;
        next_state_ = &gPointI;
        dist_kp_ = 1;
}

ForestJ::ForestJ()
{
        x1_ = 4500;
        y1_ = 9400;
        tolerance_ = 400;
        next_state_ = &gPointJ;
}

StopRobot::StopRobot()
{
        x1_ = 4200;
        y1_ = 8900;
        tolerance_ = 100;
        next_state_ = &gStop;
        dist_kp_ = 0.9;
}

void ForestA::setNextState(Robot *rob)
{
        next_state_->set_Sensors(rob);
        rob->set_State(next_state_);
        printf("ForestB Zone reached!!\n");
}

void ForestB::setNextState(Robot *rob)
{
        next_state_->set_Sensors(rob);
        rob->set_State(next_state_);
        printf("ForestC Zone reached!!\n");
}

void ForestC::setNextState(Robot *rob)
{
        next_state_->set_Sensors(rob);
        rob->set_State(next_state_);
        printf("ForestD Zone reached!!\n");
}

void ForestD::setNextState(Robot *rob)
{
        next_state_->set_Sensors(rob);
        rob->set_State(next_state_);
        printf("ForestE Zone reached!!\n");
}

void ForestE::setNextState(Robot *rob)
{
        next_state_->set_Sensors(rob);
        rob->set_State(next_state_);
        printf("ForestF Zone reached!!\n");
}

void ForestF::setNextState(Robot *rob)
{
        next_state_->set_Sensors(rob);
        rob->set_State(next_state_);
        printf("ForestG Zone reached!!\n");
}

void ForestG::setNextState(Robot *rob)
{
        next_state_->set_Sensors(rob);
        rob->set_State(next_state_);
        printf("ForestH Zone reached!!\n");
}

void ForestH::setNextState(Robot *rob)
{
        next_state_->set_Sensors(rob);
        rob->set_State(next_state_);
        printf("ForestI Zone reached!!\n");
}

void ForestI::setNextState(Robot *rob)
{
        next_state_->set_Sensors(rob);
        rob->set_State(next_state_);
        printf("ForestJ Zone reached!!\n");
}

void ForestJ::setNextState(Robot *rob)
{
        next_state_->set_Sensors(rob);
        rob->set_State(next_state_);
        printf("Stop Zone reached!!\n");
}

void StopRobot::setNextState(Robot *rob)
{
        next_state_->set_Sensors(rob);
        rob->set_State(&gStop);
}

void ForestA::set_Sensors(Robot *rob)
{
        rob->get_PositionSensor()->add_Sensor(&gXEncoder);
        rob->get_PositionSensor()->add_Sensor(&gYEncoder);
        rob->get_PositionSensor()->add_Sensor(&gXLidar);
}

void ForestB::set_Sensors(Robot *rob)
{
        rob->get_PositionSensor()->add_Sensor(&gXEncoder);
        rob->get_PositionSensor()->add_Sensor(&gYEncoder);
        rob->get_PositionSensor()->add_Sensor(&gXLidar);
}

void ForestC::set_Sensors(Robot *rob)
{
        rob->get_PositionSensor()->add_Sensor(&gXEncoder);
        rob->get_PositionSensor()->add_Sensor(&gYEncoder);
        rob->get_PositionSensor()->add_Sensor(&gXLidar);
}

void ForestD::set_Sensors(Robot *rob)
{
        rob->get_PositionSensor()->add_Sensor(&gXEncoder);
        rob->get_PositionSensor()->add_Sensor(&gYEncoder);
        rob->get_PositionSensor()->add_Sensor(&gXLidar);
}

void ForestE::set_Sensors(Robot *rob)
{
        rob->get_PositionSensor()->add_Sensor(&gXEncoder);
        rob->get_PositionSensor()->add_Sensor(&gYEncoder);
        rob->get_PositionSensor()->add_Sensor(&gXLidar);
}

void ForestF::set_Sensors(Robot *rob)
{
        rob->get_PositionSensor()->add_Sensor(&gXEncoder);
        rob->get_PositionSensor()->add_Sensor(&gYEncoder);
        rob->get_PositionSensor()->add_Sensor(&gXLidar);
}

void ForestG::set_Sensors(Robot *rob)
{
        rob->get_PositionSensor()->add_Sensor(&gXEncoder);
        rob->get_PositionSensor()->add_Sensor(&gYEncoder);
        rob->get_PositionSensor()->add_Sensor(&gXLidar);
}

void ForestH::set_Sensors(Robot *rob)
{
        rob->get_PositionSensor()->add_Sensor(&gXEncoder);
        rob->get_PositionSensor()->add_Sensor(&gYEncoder);
        rob->get_PositionSensor()->add_Sensor(&gXLidar);
}

void ForestI::set_Sensors(Robot *rob)
{
        rob->get_PositionSensor()->add_Sensor(&gXEncoder);
        rob->get_PositionSensor()->add_Sensor(&gYEncoder);
        rob->get_PositionSensor()->add_Sensor(&gXLidar);
}

void ForestJ::set_Sensors(Robot *rob)
{
        rob->get_PositionSensor()->add_Sensor(&gXEncoder);
        rob->get_PositionSensor()->add_Sensor(&gYEncoder);
        rob->get_PositionSensor()->add_Sensor(&gXLidar);
}

void StopRobot::set_Sensors(Robot *rob)
{
        rob->get_PositionSensor()->add_Sensor(&gXEncoder);
        rob->get_PositionSensor()->add_Sensor(&gYEncoder);
        rob->get_PositionSensor()->add_Sensor(&gXLidar);
}
