/*
 * filter.cpp
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "utils/filter.h"

//* First Order Complementary Filter Implementation
float Complementary_Filter::filter(float low_pass, float rate, uint32_t dt_millis)
{
        float dt = (float)dt_millis / 1000.0;
        filtered_ = lambda_ * (filtered_ + rate*dt) + (1 - lambda_) * low_pass;
        return filtered_;
}

//*
//* Second-Order Complementary FIlter Implementation
float Complementary_Filter2::filter(float low_pass, float rate, uint32_t dt_millis)
{
        float dt = (float)dt_millis / 1000.0;

        float x1 = low_pass - filtered_;
        last_acc_sum_ += x1 * k_ * k_ * dt;
        filtered_ += (rate + x1 * 2 * k_ + last_acc_sum_) * dt;

        return filtered_;
}
//*
