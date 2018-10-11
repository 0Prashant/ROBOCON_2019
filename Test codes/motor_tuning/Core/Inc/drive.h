#ifndef _drive_H_
#define _drive_H_

#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "motor.h"

#define HUMAN_RESPONSE 0.07

const float Speed_Multiplier = 1.5;
const float time = 1;

const int Coupling_Matrix[4][3] = {
					{  -1  ,  1  ,  1  },
					{   1  ,  1  ,  1  }, 
					{  -1  ,  1  , -1  }, 
					{   1  ,  1  , -1  }
				  };

char get_uart_data(void);
void calculate_robot_velocity(void);				  

#endif //	_drive_H_
				  
				  