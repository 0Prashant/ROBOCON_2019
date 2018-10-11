#ifndef _motor_H_
#define _motor_H_

#include "stm32f4xx_hal.h"
#include "tim.h"
#include "defines.h"

enum Direction
{
        FRONT,
	BACK,
	STOP
};


class motor
{
	public:
		uint8_t id;
		float radius;
		float velcoity;
		float omega;
		GPIO_TypeDef *in1_port;
		uint16_t in1_pin;
		GPIO_TypeDef *in2_port;
		uint16_t in2_pin;		
		TIM_HandleTypeDef *htim;
		uint32_t channel;
	
		void set_Direction(enum Direction d);
		void set_DutyCycle(uint16_t dutyCycle);
		void calculate_omega(void);
		void set_Omega(float omega);
		
};

motor motor[4];
void initialize_all_motor(void);
float float_abs(float x);


#endif // _motor_H_
