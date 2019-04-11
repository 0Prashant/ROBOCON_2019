/*1-26-2019*/

#ifndef _PRINTF_H_
#define _PRINTF_H_

#include "stm32f4xx_hal.h"

/*Debug via SWO pin*/
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

#endif /*printf.h*/
