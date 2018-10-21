#ifndef _HEADERS_H_
#define _HEADERS_H_

#include "stm32f4xx_hal.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

struct __FILE{
	
int handle;
};

FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f)
{
	ITM_SendChar(ch);
	return(ch);
}



#endif  //_HEADERS_H_
