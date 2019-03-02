/*
 * kalman.h
 * 
 * Created : 11/13/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _KALMAN_H_
#define _KALMAN_H_

#include "filter_algorithm.h"

class Kalman_Filter;

struct Kalman_Vars
{
public:
        void set_F(const Mat &f) { F_ = f; }
        void set_B(const Mat &b) { B_ = b; }
        void set_H(const Mat &h) { H_ = h; }
        void set_I(const Mat &i) { I_ = i; }
        void set_P(const Mat &p) { P_ = p; }
        void set_Q(const Mat &q) { Q_ = q; }
        void set_R(const Mat &r) { R_ = r; }

        friend class Kalman_Filter;

private:
        Mat F_, B_, H_, I_, P_, Q_, R_;
};

// Trying a Simple Sensor Fusion : Gyroscope and (Accelerometer/Magnetometer)
class Kalman_Filter : public Filter_Algorithm
{
public:
        Kalman_Filter(Kalman_Vars *kv) :
        x_(2,1), u_(1,1), z_(1,1), K_(2,1)
        {
                is_first_ = true;
                kv_ = kv;
        }
        Kalman_Filter(Kalman_Filter &&) = default;
        Kalman_Filter(const Kalman_Filter &) = default;
        Kalman_Filter &operator=(Kalman_Filter &&) = default;
        Kalman_Filter &operator=(const Kalman_Filter &) = default;
        ~Kalman_Filter() { }
        
        float filter(float measured, float control_input, uint32_t dt_millis) {
                // float dt = (float)dt_millis / 1000.0;
                // For first element of vector
                if (is_first_) {
                        is_first_ = false;
                        x_.at(0,0) = measured;
                        // u_.at(0,0) = control_input;
                }
                else {
                        u_.at(0,0) = control_input;
                        z_.at(0,0) = measured;

                        // Predict
                        x_ = (kv_->F_).mult(x_) + (kv_->B_).mult(u_);
                        kv_->P_ = (kv_->F_).mult((kv_->P_).mult((kv_->F_).transpose())) + kv_->Q_;

                        // Update
                        K_ = kv_->P_.mult(kv_->H_.transpose()).mult((kv_->H_.mult(kv_->P_.mult(kv_->H_.transpose())) + kv_->R_).inverse());
                        x_ += K_.mult(z_ - kv_->H_.mult(x_));
                        kv_->P_ = (kv_->I_ - K_.mult(kv_->H_)).mult(kv_->P_);
                }

                return x_.at(0,0);
        }

private:
        bool is_first_;
        Kalman_Vars *kv_;

        Mat x_, u_, z_, K_;
};

#endif // !_KALMAN_H_
