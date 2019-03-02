/*
 * filter_algorithm.h
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _FILTER_ALGORITHM_H_
#define _FILTER_ALGORITHM_H_

#include "vec3.h"
#include "mat.h"

class Filter_Algorithm
{
public:
        virtual float filter(float low_pass, float rate, uint32_t dt_millis) = 0;
        virtual ~Filter_Algorithm() { }
        
};

// filtered_data = lambda * (theta + rate*dt) + (1 - lambda) * (low_pass_data)
class Complementary_Filter : public Filter_Algorithm
{
public:
        Complementary_Filter() {
                set_Lambda(1);
        }
        Complementary_Filter(float lambda) : Complementary_Filter() {
                set_Lambda(lambda);
        }
        Complementary_Filter(Complementary_Filter &&) = default;
        Complementary_Filter(const Complementary_Filter &) = default;
        Complementary_Filter &operator=(Complementary_Filter &&) = default;
        Complementary_Filter &operator=(const Complementary_Filter &) = default;
        ~Complementary_Filter() { }

        void set_Lambda(float lambda) { lambda_ = lambda; }
        float filter(float low_pass, float rate, uint32_t dt_millis);

private:
        float lambda_;
        float filtered_;

};


class Complementary_Filter2 : public Filter_Algorithm
{
public:
        Complementary_Filter2() {
                setK(0);
                last_acc_sum_ = 0;
        }
        Complementary_Filter2(float k) : Complementary_Filter2() {
                setK(k);
        }
        Complementary_Filter2(Complementary_Filter2 &&) = default;
        Complementary_Filter2(const Complementary_Filter2 &) = default;
        Complementary_Filter2 &operator=(Complementary_Filter2 &&) = default;
        Complementary_Filter2 &operator=(const Complementary_Filter2 &) = default;
        ~Complementary_Filter2() { }

        void setK(float k) { k_ = k; }

        float filter(float low_pass, float rate, uint32_t dt_millis);

private:
        float k_;
        float last_acc_sum_;
        float filtered_;
};

#endif // !_FILTER_ALGORITHM_H_
