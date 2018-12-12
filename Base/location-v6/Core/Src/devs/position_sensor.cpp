/*
 * position_sensor.h
 * 
 * Created : 11/29/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "devs/position_sensor.h"
#include "robot/robo_init.h"
#include "utils/smoothie.h"
#include "robot/robot.h"

// static Vec3<float> calc_Position(Vec3<float> lidar, Vec3<float> enc, uint32_t dt_millis);

static Exp_Smooth gXLidarAlpha35(0.35);
static Exp_Smooth gYLidarAlpha35(0.35);
static Exp_Smooth gXEncoderAlpha35(0.35);
static Exp_Smooth gYEncoderAlpha35(0.35);

extern Robot gKhangaiRobot;
// Starting Y-Position
float gLast_YEncoderValue = 390;
float gLast_XEncoderValue = 186;

// float gLast_lx = 0;

Vec3<float> gLastPosition(gLast_XEncoderValue, gLast_YEncoderValue, 0);

Vec3<float> PositionSensor::read_Position(uint32_t dt_millis)
{
        Vec3<float> lidar, enc;
        float lx(0), ly(0), ex(0), ey(0);
        bool x_lidar_used = false;
        bool y_lidar_used = false;
        bool x_enc_used = false;
        bool y_enc_used = false;
        // Read Data from available sensors and smooth them
        for (uint8_t i = 0; i < sensor_count; ++i) {
                if (p_sensors_[i]->get_Name() == SensorName::XLidar) {
                        lx = p_sensors_[i]->read();
                        lx = gXLidarAlpha35.smooth(lx);
                        lidar.setX(lx);
                        x_lidar_used = true;
                }
                else if (p_sensors_[i]->get_Name() == SensorName::YLidar) {
                        ly = p_sensors_[i]->read();
                        ly = gYLidarAlpha35.smooth(ly);
                        lidar.setY(ly);
                        y_lidar_used = true;
                }
                else if (p_sensors_[i]->get_Name() == SensorName::XEncoder) {
                        ex = (-1)*p_sensors_[i]->read();    // -ve count is observed in considered +ve direction
                        enc.setX(ex);
                        x_enc_used = true;
                }
                else if (p_sensors_[i]->get_Name() == SensorName::YEncoder) {
                        ey = p_sensors_[i]->read();
                        enc.setX(ey);
                        y_enc_used = true;
                }
        }
        if (!(x_lidar_used || x_enc_used)) {
                printf("No Data Available on x-direction!!\n");
        }
        if (!(y_lidar_used || y_enc_used)) {
                printf("No Data Available on y-direction!!\n");
        }

        float x(0), y(0);
        if (!x_lidar_used) {
                x = gLastPosition.getX() + ex;
                lx = gXLidarAlpha35.smooth(x);
        }
        else {
                Second_Pass.set_Algorithm(&gXDist_Filter);
                x = Second_Pass.filter(lx, ex, dt_millis);
        }
        if (!y_lidar_used) {
                y = gLastPosition.getY() + ey;
                ly = gYLidarAlpha35.smooth(y);
        }
        else {
                Second_Pass.set_Algorithm(&gYDist_Filter);
                y = Second_Pass.filter(ly, ey, dt_millis);
        }
        // gLast_lx = lx;
        gLastPosition.set_Values(x,y,0);
        return gLastPosition;
}


// static Vec3<float> calc_Position(Vec3<float> lidar, Vec3<float> enc, uint32_t dt_millis)
// {
//         float lx = lidar.getX();
//         float ex = enc.getX();
//         float ly = lidar.getY();
//         float ey = enc.getY();

//         Vec3<float> dist;

//         Second_Pass.set_Algorithm(&gXDist_Filter);
//         float x_dist = Second_Pass.filter(lx, ex, dt_millis);

//         Second_Pass.set_Algorithm(&gYDist_Filter);
//         float y_dist = Second_Pass.filter(ly, ey, dt_millis);

//         dist.setX(x_dist);
//         dist.setY(y_dist);

//         return dist;
// }
