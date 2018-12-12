#include "robot/robot.h"
#include "robot/devs_config.h"
#include "utils/smoothie.h"
#include "devs/wheeler.h"
#include "robot/robo_init.h"

extern Vec3<float> gOmega_Bias;

ForestA gPointA;
ForestB gPointB;
ForestC gPointC;
ForestD gPointD;
ForestE gPointE;
ForestF gPointF;
ForestG gPointG;
ForestH gPointH;
ForestI gPointI;
ForestJ gPointJ;
StopRobot gStop;

static Exp_Smooth gXAccelAlpha35(0.35);
static Exp_Smooth gZAccelAlpha35(0.35);
static Exp_Smooth gYAccelAlpha35(0.35);

static Exp_Smooth gXGyroAlpha35(0.35);
static Exp_Smooth gYGyroAlpha35(0.35);
static Exp_Smooth gZGyroAlpha35(0.35);

static Exp_Smooth gXMagAlpha35(0.35);
static Exp_Smooth gYMagAlpha35(0.35);
static Exp_Smooth gZMagAlpha35(0.35);


static Vec3<float> read_Orientation(uint32_t dt_millis);

bool gIsFirstTimeStamp = true;
uint32_t gPrintTime;

void Robot::move(Vec3<float> pos, uint32_t dt_millis) {
        // calculate current heading
        Vec3<float> ori = read_Orientation(dt_millis);
        float heading = ori.getZ();

        if (is_first_reading_) {
                is_first_reading_ = false;
                first_heading_ = heading;
                // velocity_ = state_->calc_RoboVelocity();
                // angle_of_attack_ = state_->calc_AngleOfAttack(pos);
        }
        else {
                // printf("Velocity : %ld  ", (int32_t)(velocity_));
                // printf("Angle Of Attack : %ld  ", (int32_t)(angle_of_attack_ * 57.3));
                if (gIsFirstTimeStamp) {
                        gPrintTime = HAL_GetTick();
                        gIsFirstTimeStamp = false;
                }
                if (HAL_GetTick() - gPrintTime < 15000) {
                        printf("%ld, ", (HAL_GetTick()));
                        pos.print();
                        printf(", ");
                        printf("%d, ", (int16_t)(heading - first_heading_));
                        printf("%d, %d\n", (int16_t)(velocity_), (int16_t)(angle_of_attack_));
                } 

                angle_of_attack_ = state_->calc_AngleOfAttack(pos);
                velocity_ = state_->calc_RoboVelocity();
                calc_WheelsOmegas(omegas_, velocity_, angle_of_attack_, (first_heading_ - heading)/57.3, dt_millis);
                send_WheelerPack(omegas_);

        }
}

void Robot::run(uint32_t dt_millis) {
        Vec3<float> pos = pos_->read_Position(dt_millis);
        if (state_->nextStateReached(pos)) {
                state_->setNextState(this);
        }

        move(pos, dt_millis);
}


// ! This function requires major refactoring
static Vec3<float> read_Orientation(uint32_t dt_millis)
{
        MPU6050_Read_NormAxes(&Body_IMU);
        HMC5883_Read(&Body_HMC);

        Vec3<float> accel = Body_IMU.norm_a_axis;
        Vec3<float> gyro = Body_IMU.norm_g_axis - gOmega_Bias;
        Vec3<float> mag = Body_HMC.raw_axis;

        // We will consider the rotation in order: psi-theta-phi
        float ax = gXAccelAlpha35.smooth(accel.getX());
        float ay = gYAccelAlpha35.smooth(accel.getY());
        float az = gZAccelAlpha35.smooth(accel.getZ());
        
        float gx = gXGyroAlpha35.smooth(gyro.getX());
        float gy = gYGyroAlpha35.smooth(gyro.getY());
        float gz = gZGyroAlpha35.smooth(gyro.getZ());

        float bx = gXMagAlpha35.smooth(mag.getX());
        float by = gYMagAlpha35.smooth(mag.getY());
        float bz = gZMagAlpha35.smooth(mag.getZ());
        
        float roll = atan2f(ay, az) * 57.3;

        Second_Pass.set_Algorithm(&gRoll_Filter);
        roll = Second_Pass.filter(roll, gx, dt_millis);

        float sin_roll = sin(roll / 57.3);
        float cos_roll = cos(roll / 57.3);
        // Roll Compensated Pitch
        float pitch = atan2f((-ax), ay*sin_roll + az*cos_roll) * 57.3;

        Second_Pass.set_Algorithm(&gPitch_Filter);
        pitch = Second_Pass.filter(pitch, gy, dt_millis);

        float sin_pitch = sin(pitch / 57.3);
        float cos_pitch = cos(pitch / 57.3);
        // Tilt Compensated Yaw
        float yaw = atan2f((bz*sin_roll - by*cos_roll), (bx*cos_pitch + by*sin_roll*sin_pitch + bz*cos_roll*sin_pitch)) * 57.3;

        Second_Pass.set_Algorithm(&gYaw_Filter);
        yaw = Second_Pass.filter(yaw, gz, dt_millis);

        Vec3<float> angles(roll, pitch, yaw);

        return angles;
}
