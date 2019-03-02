/*
 * encoder.h
 * 
 * Created : 11/16/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "periphs/gpio.h"
#include "sensor.h"

struct Enc
{
        GPIO_TypeDef *chB_port;
        uint16_t chB_pin;
        int32_t count;
        float radius;
};

void Enc_HandleCount(struct Enc *enc);


class Encoder : public Sensor
{
public:
        Encoder() = default;
        Encoder(Encoder &&) = default;
        Encoder(const Encoder &) = default;
        Encoder &operator=(Encoder &&) = default;
        Encoder &operator=(const Encoder &) = default;
        ~Encoder() { }
        
        Encoder(struct Enc *enc, SensorName name) {
                enc_ = enc;
                name_ = name;
        }
        
        int init() override;
        float read() override;
        bool available() override;
        void denit() override;

private:
        Enc *enc_;
};

#endif // !_ENCODER_H_
