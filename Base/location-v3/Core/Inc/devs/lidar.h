/*
 * lidar.h
 * 
 * Created : 11/9/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _LIDAR_H_
#define _LIDAR_H_

void LIDAR_Init();
uint16_t LIDAR_Read();
void LIDAR_UART3_RxCpltCallback();
bool LIDAR_IsEmpty();

#endif // !_LIDAR_H_