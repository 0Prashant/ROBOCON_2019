#ifndef _ROBO_STATES_H_
#define _ROBO_STATES_H_

#include "devs/sensor.h"
#include "devs/position_sensor.h"
#include "utils/vec3.h"
#include <math.h>

#define DEFAULT_ROBOT_VELOCITY          (800)

// Forward Declarations Of Robot and Points on GameField
class Robot;

class Encoder;
class Lidar;

extern Encoder gXEncoder;
extern Encoder gYEncoder;
extern Lidar gXLidar;

class Robo_States
{
public:
        Robo_States() { }
        Robo_States(Robo_States &&) = default;

        Robo_States(const Robo_States &) = default;
        Robo_States &operator=(Robo_States &&) = default;
        Robo_States &operator=(const Robo_States &) = default;
        virtual ~Robo_States() { }

        virtual void setNextState(Robot *rob) = 0;
        virtual float calc_RoboVelocity() = 0;
        virtual float calc_AngleOfAttack(Vec3<float> pos) = 0;
        virtual bool nextStateReached(Vec3<float> pos) = 0;

        virtual float getX() = 0;
        virtual float getY() = 0;
        virtual void set_Sensors(Robot *rob) = 0;
        
        float tolerance_;       // Probable Space radius in mm
protected:
        float angle_of_attack_;
        float x1_, y1_;
        float v_;
        Robo_States *next_state_;

};

class ForestA : public Robo_States
{
public:
        ForestA();
        ForestA(ForestA &&) = default;
        ForestA(const ForestA &) = default;
        ForestA &operator=(ForestA &&) = default;
        ForestA &operator=(const ForestA &) = default;
        ~ForestA() { }
        
        void setNextState(Robot *rob);
        void set_Sensors(Robot *rob);
        
        // float calc_RoboVelocity() {
        //         return dist_kp_ * DEFAULT_ROBOT_VELOCITY;
        // }
        float calc_RoboVelocity() {
                if (dist_kp_ < 0.8) {
                        dist_kp_ += 0.01;
                }
                else {
                        dist_kp_ = 1;
                }
                return DEFAULT_ROBOT_VELOCITY * dist_kp_;
        }
        float calc_AngleOfAttack(Vec3<float> pos) {
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                return atan2f(dx, dy);
        }
        bool nextStateReached(Vec3<float> pos) {
                float tol_sq = next_state_->tolerance_ * next_state_->tolerance_;
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                // printf("dx = %d, dy = %d\n", (int16_t)(dx), (int16_t)(dy));
                return ((dx*dx + dy*dy) < tol_sq);
        }

        float getX() { return x1_; }
        float getY() { return y1_; }

private:
        float dist_kp_;
};

class ForestB : public Robo_States
{
public:
        ForestB();
        ForestB(ForestB &&) = default;
        ForestB(const ForestB &) = default;
        ForestB &operator=(ForestB &&) = default;
        ForestB &operator=(const ForestB &) = default;
        ~ForestB() { }
        
        void setNextState(Robot *rob);
        void set_Sensors(Robot *rob);
        
        float calc_RoboVelocity() {
                return DEFAULT_ROBOT_VELOCITY;
        }
        float calc_AngleOfAttack(Vec3<float> pos) {
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                return atan2f(dx, dy);
        }
        bool nextStateReached(Vec3<float> pos) {
                float tol_sq = next_state_->tolerance_ * next_state_->tolerance_;
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                // printf("dx = %d, dy = %d\n", (int16_t)(dx), (int16_t)(dy));
                return ((dx*dx + dy*dy) < tol_sq);
        }

        float getX() { return x1_; }
        float getY() { return y1_; }
private:
        
};

class ForestC : public Robo_States
{
public:
        ForestC();
        ForestC(ForestC &&) = default;
        ForestC(const ForestC &) = default;
        ForestC &operator=(ForestC &&) = default;
        ForestC &operator=(const ForestC &) = default;
        ~ForestC() { }
        
        void setNextState(Robot *rob);
        void set_Sensors(Robot *rob);
        
        float calc_RoboVelocity() {
                return DEFAULT_ROBOT_VELOCITY;
        }
        float calc_AngleOfAttack(Vec3<float> pos) {
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                return atan2f(dx, dy);
        }
        bool nextStateReached(Vec3<float> pos) {
                float tol_sq = next_state_->tolerance_ * next_state_->tolerance_;
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                // printf("dx = %d, dy = %d\n", (int16_t)(dx), (int16_t)(dy));
                return ((dx*dx + dy*dy) < tol_sq);
        }

        float getX() { return x1_; }
        float getY() { return y1_; }
private:
        
};

class ForestD : public Robo_States
{
public:
        ForestD();
        ForestD(ForestD &&) = default;
        ForestD(const ForestD &) = default;
        ForestD &operator=(ForestD &&) = default;
        ForestD &operator=(const ForestD &) = default;
        ~ForestD() { }
        
        void setNextState(Robot *rob);
        void set_Sensors(Robot *rob);
        
        float calc_RoboVelocity() {
                return DEFAULT_ROBOT_VELOCITY;
        }
        float calc_AngleOfAttack(Vec3<float> pos) {
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                return atan2f(dx, dy);
        }
        bool nextStateReached(Vec3<float> pos) {
                float tol_sq = next_state_->tolerance_ * next_state_->tolerance_;
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                // printf("dx = %d, dy = %d\n", (int16_t)(dx), (int16_t)(dy));
                return ((dx*dx + dy*dy) < tol_sq);
        }

        float getX() { return x1_; }
        float getY() { return y1_; }

private:
        
};

class ForestE : public Robo_States
{
public:
        ForestE();
        ForestE(ForestE &&) = default;
        ForestE(const ForestE &) = default;
        ForestE &operator=(ForestE &&) = default;
        ForestE &operator=(const ForestE &) = default;
        ~ForestE() { }
        
        void setNextState(Robot *rob);
        void set_Sensors(Robot *rob);
        
        float calc_RoboVelocity() {
                return DEFAULT_ROBOT_VELOCITY;
        }
        float calc_AngleOfAttack(Vec3<float> pos) {
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                return atan2f(dx, dy);
        }
        bool nextStateReached(Vec3<float> pos) {
                float tol_sq = next_state_->tolerance_ * next_state_->tolerance_;
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                // printf("dx = %d, dy = %d\n", (int16_t)(dx), (int16_t)(dy));
                return ((dx*dx + dy*dy) < tol_sq);
        }

        float getX() { return x1_; }
        float getY() { return y1_; }

private:
        float dist_kp_;
};

class ForestF : public Robo_States
{
public:
        ForestF();
        ForestF(ForestF &&) = default;
        ForestF(const ForestF &) = default;
        ForestF &operator=(ForestF &&) = default;
        ForestF &operator=(const ForestF &) = default;
        ~ForestF() { }
        
        void setNextState(Robot *rob);
        void set_Sensors(Robot *rob);
        
        float calc_RoboVelocity() {
                return DEFAULT_ROBOT_VELOCITY;
        }
        float calc_AngleOfAttack(Vec3<float> pos) {
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                return atan2f(dx, dy);
        }
        bool nextStateReached(Vec3<float> pos) {
                float tol_sq = next_state_->tolerance_ * next_state_->tolerance_;
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                // printf("dx = %d, dy = %d\n", (int16_t)(dx), (int16_t)(dy));
                return ((dx*dx + dy*dy) < tol_sq);
        }
        
        // float calc_RoboVelocity() {
        //         return 0;
        // }

        // float calc_AngleOfAttack(Vec3<float> pos) {
        //         float dx = next_state_->getX() - pos.getX();
        //         float dy = next_state_->getY() - pos.getY();
        //         return atan2f(dx, dy);
        // }
        // bool nextStateReached(Vec3<float> pos) {
        //         return false;
        // }

        float getX() { return x1_; }
        float getY() { return y1_; }

private:
        
};

class ForestG : public Robo_States
{
public:
        ForestG();
        ForestG(ForestG &&) = default;
        ForestG(const ForestG &) = default;
        ForestG &operator=(ForestG &&) = default;
        ForestG &operator=(const ForestG &) = default;
        ~ForestG() { }
        
        void setNextState(Robot *rob);
        void set_Sensors(Robot *rob);
        
        float calc_RoboVelocity() {
                return DEFAULT_ROBOT_VELOCITY;
        }
        float calc_AngleOfAttack(Vec3<float> pos) {
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                return atan2f(dx, dy);
        }
        bool nextStateReached(Vec3<float> pos) {
                float tol_sq = next_state_->tolerance_ * next_state_->tolerance_;
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                // printf("dx = %d, dy = %d\n", (int16_t)(dx), (int16_t)(dy));
                return ((dx*dx + dy*dy) < tol_sq);
        }

        float getX() { return x1_; }
        float getY() { return y1_; }

private:
        
};

class ForestH : public Robo_States
{
public:
        ForestH();
        ForestH(ForestH &&) = default;
        ForestH(const ForestH &) = default;
        ForestH &operator=(ForestH &&) = default;
        ForestH &operator=(const ForestH &) = default;
        ~ForestH() { }
        
        void setNextState(Robot *rob);
        void set_Sensors(Robot *rob);
        
        float calc_RoboVelocity() {
                return DEFAULT_ROBOT_VELOCITY;
        }
        float calc_AngleOfAttack(Vec3<float> pos) {
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                return atan2f(dx, dy);
        }
        bool nextStateReached(Vec3<float> pos) {
                float tol_sq = next_state_->tolerance_ * next_state_->tolerance_;
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                // printf("dx = %d, dy = %d\n", (int16_t)(dx), (int16_t)(dy));
                return ((dx*dx + dy*dy) < tol_sq);
        }

        float getX() { return x1_; }
        float getY() { return y1_; }

private:
        
};

class ForestI : public Robo_States
{
public:
        ForestI();
        ForestI(ForestI &&) = default;
        ForestI(const ForestI &) = default;
        ForestI &operator=(ForestI &&) = default;
        ForestI &operator=(const ForestI &) = default;
        ~ForestI() { }
        
        void setNextState(Robot *rob);
        void set_Sensors(Robot *rob);
        
        float calc_RoboVelocity() {
                return 0;
        }

        float calc_AngleOfAttack(Vec3<float> pos) {
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                return atan2f(dx, dy);
        }
        bool nextStateReached(Vec3<float> pos) {
                return false;
        }
        
        // float calc_RoboVelocity() {
        //         return DEFAULT_ROBOT_VELOCITY * 1.5;
        // }
        // float calc_RoboVelocity() {
        //         // if (dist_kp_ > 0.8) {
        //         //         dist_kp_ -= 0.001;
        //         // }
        //         // else if (dist_kp_ > 0.6) {
        //         //         dist_kp_ -= 0.001;
        //         // }
        //         // else if (dist_kp_ > 0.4) {
        //         //         dist_kp_ -= 0.001;
        //         // }
        //         // // else if (dist_kp_ > 0.2) {
        //         // //         dist_kp_ -= 0.05;
        //         // // }
        //         // else {
        //         //         dist_kp_ = 0.4;
        //         // }
        //         return DEFAULT_ROBOT_VELOCITY;
        // }
        // float calc_AngleOfAttack(Vec3<float> pos) {
        //         float dx = next_state_->getX() - pos.getX();
        //         float dy = next_state_->getY() - pos.getY();
        //         return atan2f(dx, dy);
        // }
        // bool nextStateReached(Vec3<float> pos) {
        //         float tol_sq = next_state_->tolerance_ * next_state_->tolerance_;
        //         float dx = next_state_->getX() - pos.getX();
        //         float dy = next_state_->getY() - pos.getY();
        //         // printf("dx = %d, dy = %d\n", (int16_t)(dx), (int16_t)(dy));
        //         return ((dx*dx + dy*dy) < tol_sq);
        // }

        float getX() { return x1_; }
        float getY() { return y1_; }

private:
        float dist_kp_;
};

class ForestJ : public Robo_States
{
public:
        ForestJ();
        ForestJ(ForestJ &&) = default;
        ForestJ(const ForestJ &) = default;
        ForestJ &operator=(ForestJ &&) = default;
        ForestJ &operator=(const ForestJ &) = default;
        ~ForestJ() { }
        
        void setNextState(Robot *rob);
        void set_Sensors(Robot *rob);
        

        float calc_RoboVelocity() {
                return 0;
        }

        float calc_AngleOfAttack(Vec3<float> pos) {
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                return atan2f(dx, dy);
        }
        bool nextStateReached(Vec3<float> pos) {
                // float tol_sq = next_state_->tolerance_ * next_state_->tolerance_;
                // float dx = next_state_->getX() - pos.getX();
                // float dy = next_state_->getY() - pos.getY();
                // // printf("dx = %d, dy = %d\n", (int16_t)(dx), (int16_t)(dy));
                // return ((dx*dx + dy*dy) < tol_sq);
                return false;
        }

        float getX() { return x1_; }
        float getY() { return y1_; }

private:
        
};

class StopRobot : public Robo_States
{
public:
        StopRobot();
        StopRobot(StopRobot &&) = default;
        StopRobot(const StopRobot &) = default;
        StopRobot &operator=(StopRobot &&) = default;
        StopRobot &operator=(const StopRobot &) = default;
        ~StopRobot() { }
        
        void setNextState(Robot *rob);
        void set_Sensors(Robot *rob);

        float calc_RoboVelocity() {
                return 0;
        }
        // float calc_RoboVelocity() {
        //         if (dist_kp_ > 0.2) {
        //                 dist_kp_ -= 0.01;
        //         }
        //         else {
        //                 dist_kp_ = 0;
        //         }
        //         return DEFAULT_ROBOT_VELOCITY * dist_kp_ * 3;
        // }

        float calc_AngleOfAttack(Vec3<float> pos) {
                float dx = next_state_->getX() - pos.getX();
                float dy = next_state_->getY() - pos.getY();
                return atan2f(dx, dy);
        }
        bool nextStateReached(Vec3<float> pos) {
                return true;
        }

        float getX() { return x1_; }
        float getY() { return y1_; }
private:
        float dist_kp_;
};


#endif // !_ROBO_STATES_H_
