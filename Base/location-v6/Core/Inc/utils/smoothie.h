/*
 * smoothie.h
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _SMOOTHIE_H_
#define _SMOOTHIE_H_

#include "smoothing_algorithm.h"

class Smoothie
{
public:
        Smoothie() { algo_ = 0; }
        Smoothie(Smoothing_Algorithm *sm_algo);
        Smoothie(Smoothie &&) = default;
        Smoothie(const Smoothie &) = default;
        Smoothie &operator=(Smoothie &&) = default;
        Smoothie &operator=(const Smoothie &) = default;
        ~Smoothie();

        void set_Algorithm(Smoothing_Algorithm *sm_algo) { algo_ = sm_algo; }
        float smooth(const float &input) {
                return algo_->smooth(input);
        }

private:
        Smoothing_Algorithm *algo_;
};

#endif // !_SMOOTHIE_H_
