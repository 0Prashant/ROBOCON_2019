#ifndef _PID_H_
#define _PID_H_

#include "pid_algorithms.h"

class PID
{
public:
        PID() { }
        //PID(PID_Algorithm *algo) { set_Algorithm(algo); }
        PID(PID &&) = default;
        PID(const PID &) = default;
        PID &operator=(PID &&) = default;
        PID &operator=(const PID &) = default;
        ~PID() { }

        //void set_Algorithm(PID_Algorithm *algo) { algo_ = algo; }
        PID_Algorithm * get_Algorithm() { return algo_; }
	void set_PID_constants(float kp, float ki, float kd){
		algo_->set_P(kp);
		algo_->set_I(ki);
		algo_->set_D(kd);
	}
	void set_PID_limits(float upper_limit, float lower_limit){
		algo_->set_Limits(upper_limit, lower_limit);
	}
        float compute_PID(float err, uint32_t dt_millis) {
                return algo_->compute(err, dt_millis);
        }

private:
        Discrete_PID *algo_;
};


#endif // !_PID_H_
