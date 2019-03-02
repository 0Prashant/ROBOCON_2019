/*
 * lidar.h
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _LIDAR_H_
#define _LIDAR_H_

#include "sensor.h"

void LIDAR_Init();
uint16_t LIDAR_Read();
void LIDAR_UART3_RxCpltCallback();
bool LIDAR_IsEmpty();

class Lidar : public Sensor
{
public:
        Lidar() = default;
        Lidar(Lidar &&) = default;
        Lidar(const Lidar &) = default;
        Lidar &operator=(Lidar &&) = default;
        Lidar &operator=(const Lidar &) = default;
        ~Lidar() { }

        Lidar(SensorName name) { name_ = name; }
        
        int init() override;
        float read() override;
        bool available() override;
        void denit() override;

private:
        
};

#endif // !_LIDAR_H_