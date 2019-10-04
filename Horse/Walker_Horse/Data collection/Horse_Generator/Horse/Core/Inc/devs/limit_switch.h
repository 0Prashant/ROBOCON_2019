#ifndef _LIMIT_SWITCH_H_
#define _LIMIT_SWITCH_H_

struct limit_switch{
	GPIO_TypeDef *int_port;	
        uint16_t int_pin;
};

#endif // !_LIMIT_SWITCH_H_