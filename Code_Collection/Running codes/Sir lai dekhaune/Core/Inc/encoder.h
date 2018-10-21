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
	float distancex, distancey;
};

void init_encoder(void);
void calculate_encoder_distance(struct encoder *e);
void calculate_robot_distance(void);

#endif  //_ENCODER_H_
