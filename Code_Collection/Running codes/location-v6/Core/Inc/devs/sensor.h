/*
 * sensor.h
 * 
 * Created : 11/29/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _SENSOR_H_
#define _SENSOR_H_

enum class SensorName
{
        Gyro,
        Compass,
        XEncoder,
        YEncoder,
        XLidar,
        YLidar
};

class Sensor
{
public:
        Sensor() = default;
        Sensor(Sensor &&) = default;
        Sensor(const Sensor &) = default;
        Sensor &operator=(Sensor &&) = default;
        Sensor &operator=(const Sensor &) = default;
        virtual ~Sensor() { }

        virtual int init() = 0;
        SensorName get_Name() { return name_; }
        virtual float read() = 0;
        virtual bool available() = 0;
        virtual void denit() = 0;

        friend bool operator==(Sensor &sen1, Sensor &sen2) { return (sen1.name_ == sen2.name_); }

protected:
        SensorName name_;
};

#endif // !_SENSOR_H_
