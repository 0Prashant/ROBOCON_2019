/*
 * thinker.h
 * 
 * Created : 11/20/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _THINKER_H_
#define _THINKER_H_

#include "utils/queue.h"
#include "periphs/usart.h"


void Thinker_Init();
void send_ThinkerPack(int16_t pack[3]);
void Thinker_UART2_TxCpltCallback();

#endif // !_THINKER_H_
