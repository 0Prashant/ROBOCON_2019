
#ifndef _SMOOTHING_ALGORITHM_H_
#define _SMOOTHING_ALGORITHM_H_

#include "vec3.h"

class Smoothing_Algorithm
{
public:
        virtual Vec3<float> smooth(const Vec3<float> &input) = 0;
        virtual ~Smoothing_Algorithm() { }
};

class Exp_Smooth : public Smoothing_Algorithm
{
public:
        Exp_Smooth();
        Exp_Smooth(float alpha);
        Exp_Smooth(Exp_Smooth &&) = default;
        Exp_Smooth(const Exp_Smooth &) = default;
        Exp_Smooth &operator=(Exp_Smooth &&) = default;
        Exp_Smooth &operator=(const Exp_Smooth &) = default;
        ~Exp_Smooth() { }

        void set_Alpha(float alpha) { alpha_ = alpha; }

        Vec3<float> smooth(const Vec3<float> &input);

private:
        Vec3<float> last_output_;
        float alpha_;
        bool is_first_;
};

#endif // !_SMOOTHING_ALGORITHM_H_
