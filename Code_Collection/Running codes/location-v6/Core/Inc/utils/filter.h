/*
 * filter.h
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _FILTER_H_
#define _FILTER_H_

#include "filter_algorithm.h"

class Filter
{
public:
        Filter() { algo_ = 0; }

        Filter(Filter_Algorithm *filt_algo) { set_Algorithm(filt_algo); }

        Filter(Filter &&) = default;
        Filter(const Filter &) = default;
        Filter &operator=(Filter &&) = default;
        Filter &operator=(const Filter &) = default;
        ~Filter() { }

        void set_Algorithm(Filter_Algorithm *filt_algo) { algo_ = filt_algo; }
        float filter(float low_pass, float rate, uint32_t dt_millis) {
                return algo_->filter(low_pass, rate, dt_millis);
        }

private:
        Filter_Algorithm *algo_;
};

#endif // !_FILTER_H_
