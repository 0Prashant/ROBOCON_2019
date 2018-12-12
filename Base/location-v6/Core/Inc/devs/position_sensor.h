/*
 * position_sensor.h
 * 
 * Created : 11/29/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _POSITION_SENSOR_H_
#define _POSITION_SENSOR_H_

#include "utils/vec3.h"
#include "sensor.h"

#define MAX_POSITION_SENSORS    (4)

class PositionSensor
{
public:
        PositionSensor() {
                sensor_count = 0;
        }
        PositionSensor(PositionSensor &&) = default;
        PositionSensor(const PositionSensor &) = default;
        PositionSensor &operator=(PositionSensor &&) = default;
        PositionSensor &operator=(const PositionSensor &) = default;
        ~PositionSensor() { }

        // Add sensor so that it can be used for calculating position
        void add_Sensor(Sensor *sen) {
                for (uint8_t i = 0; i < sensor_count; ++i) {
                        if (*p_sensors_[i] == *sen) {
                                return;
                        }
                }
                p_sensors_[sensor_count++] = sen;
        }
        // Remove any sensor from the next calculation
        void remove_Sensor(Sensor *sen) {
                for (uint8_t i = 0; i < sensor_count; ++i) {
                        if (*p_sensors_[i] == *sen) {
                                --sensor_count;
                                p_sensors_[i] = p_sensors_[sensor_count];
                                break;
                        }
                }
        }

        // Reads the position of the robot from the initial fences
        // using the available sensors
        Vec3<float> read_Position(uint32_t dt_millis);

private:
        Sensor *p_sensors_[MAX_POSITION_SENSORS];
        uint8_t sensor_count;
};

#endif // !_POSITION_SENSOR_H_
