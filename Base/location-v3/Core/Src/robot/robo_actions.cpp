/*
 * robo_actions.cpp
 * 
 * Created : 11/26/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "robot/robo_init.h"
#include "utils/smoothie.h"

extern Vec3<float> gOmega_Bias;

static Vec3<float> calc_Orientation(Vec3<float> accel, Vec3<float> gyro, Vec3<float> mag, uint32_t dt_millis);
static Vec3<float> calc_Position(Vec3<float> lidar, Vec3<float> enc, uint32_t dt_millis);

static Exp_Smooth gAccelAlpha35(0.35);
static Exp_Smooth gGyroAlpha35(0.35);
static Exp_Smooth gMagAlpha35(0.35);
static Exp_Smooth gLidarAlpha35(0.35);
static Smoothie gFirst_Pass;

uint32_t gLast_Lidar_Data = 0;


void read_State(Vec3<float> &euler_angles, Vec3<float> &position, uint32_t dt_millis)
{
        Vec3<float> accel;
        Vec3<float> gyro;
        Vec3<float> mag;
        
        Vec3<float> lidar;
        Vec3<float> enc;

        // Read Data from all sensors at once
        MPU6050_Read_NormAxes(&Body_IMU);
        HMC5883_Read(&Body_HMC);

        uint32_t dist = 0;
        if (!LIDAR_IsEmpty()) {
                dist = LIDAR_Read();
                // printf("%ld\n", dist);
        }
        // Lidar is 220 mm behind robot boundary so we will consider any
        // data less than 150 mm to be error;
        if (dist > 150) {
                gLast_Lidar_Data = dist;
        }
        lidar.setX((float)gLast_Lidar_Data);

        int32_t count = Encoder_get_DeltaCount(&gXEncoder);
        // We will not filter encoder data
        enc.setX((float)count);
        // printf("%ld\n", count);
        
        gFirst_Pass.set_Algorithm(&gAccelAlpha35);
        accel = gFirst_Pass.smooth(Body_IMU.norm_a_axis);
        gFirst_Pass.set_Algorithm(&gGyroAlpha35);
        gyro = gFirst_Pass.smooth(Body_IMU.norm_g_axis - gOmega_Bias);
        gFirst_Pass.set_Algorithm(&gMagAlpha35);
        mag = gFirst_Pass.smooth(Body_HMC.raw_axis);
        
        gFirst_Pass.set_Algorithm(&gLidarAlpha35);
        lidar = gFirst_Pass.smooth(lidar);

        euler_angles = calc_Orientation(accel, gyro, mag, dt_millis);
        position = calc_Position(lidar, enc, dt_millis).mult_EW(0.1);
}

static Vec3<float> calc_Orientation(Vec3<float> accel, Vec3<float> gyro, Vec3<float> mag, uint32_t dt_millis)
{
        // We will consider the rotation in order: psi-theta-phi
        float ax = accel.getX();
        float ay = accel.getY();
        float az = accel.getZ();
        
        float gx = gyro.getX();
        float gy = gyro.getY();
        float gz = gyro.getZ();

        float bx = mag.getX();
        float by = mag.getY();
        float bz = mag.getZ();
        
        float roll = atan2f(ay, az) * 57.3;

        Second_Pass.set_Algorithm(&gRoll_Filter);
        roll = Second_Pass.filter(roll, gx, dt_millis);

        float sin_roll = sin(roll / 57.3);
        float cos_roll = cos(roll / 57.3);
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

static Vec3<float> calc_Position(Vec3<float> lidar, Vec3<float> enc, uint32_t dt_millis)
{
        float lx = lidar.getX();
        float ex = enc.getX();

        Vec3<float> dist;

        Second_Pass.set_Algorithm(&gXDist_Filter);
        float x_dist = Second_Pass.filter(lx, ex, dt_millis);

        dist.setX(x_dist);

        return dist;
}
