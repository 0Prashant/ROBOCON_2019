#ifndef _ENCODER_H_
#define _ENCODER_H_
#include <inttypes.h>
#include <math.h>
#include "defines.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"



struct encoder
{
	int8_t id;
	int16_t count;
	float distancex;
	float distancey;
};

;
void calculate_distance_of_encoder(struct encoder *e);
void calculate_distance_of_robot(void);



#endif  //_ENCODER_H_
