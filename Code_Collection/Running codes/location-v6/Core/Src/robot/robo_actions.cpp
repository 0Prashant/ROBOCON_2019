/*
 * robo_actions.cpp
 * 
 * Created : 11/26/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "robot/robo_init.h"
#include "utils/smoothie.h"


uint32_t gLast_Lidar_Data = 0;


void read_State(Vec3<float> &euler_angles, Vec3<float> &position, uint32_t dt_millis)
{
        Vec3<float> accel;
        Vec3<float> gyro;
        Vec3<float> mag;
        
        // Vec3<float> lidar;
        // Vec3<float> enc;

        // Read Data from all sensors at once

        // uint32_t dist = 0;
        // if (!LIDAR_IsEmpty()) {
        //         dist = LIDAR_Read();
        //         // printf("%ld\n", dist);
        // }
        // // Lidar is 220 mm behind robot boundary so we will consider any
        // // data less than 150 mm to be error;
        // if (dist > 150) {
        //         gLast_Lidar_Data = dist;
        // }
        // lidar.setX((float)gLast_Lidar_Data);

        // // int32_t count = Enc_get_DeltaCount(&gXEncoder);
        // // We will not filter encoder data
        // // enc.setX((float)count);
        // // printf("%ld\n", count);
        
        // gFirst_Pass.set_Algorithm(&gAccelAlpha35);
        // accel = gFirst_Pass.smooth(Body_IMU.norm_a_axis);
        // gFirst_Pass.set_Algorithm(&gGyroAlpha35);
        // gyro = gFirst_Pass.smooth(Body_IMU.norm_g_axis - gOmega_Bias);
        // gFirst_Pass.set_Algorithm(&gMagAlpha35);
        // mag = gFirst_Pass.smooth(Body_HMC.raw_axis);
        
        // gFirst_Pass.set_Algorithm(&gLidarAlpha35);
        // lidar = gFirst_Pass.smooth(lidar);

        // euler_angles = calc_Orientation(accel, gyro, mag, dt_millis);
        // position = calc_Position(lidar, enc, dt_millis).mult_EW(0.1);
}
