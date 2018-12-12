/*
 * a4988.cpp
 * 
 * Created : 11/14/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "devs/a4988.h"

// void TIM_setFrequency(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t freq)
// {
//         uint32_t f = freq;
//         if (htim->Instance == TIM2) {
//                 f = 84000000 / (htim->Instance->PSC + 1);
                
//                 if (freq) {
//                         __HAL_TIM_SET_AUTORELOAD(htim, (uint32_t)(f / freq));
//                         __HAL_TIM_SET_COMPARE(htim, channel, (uint32_t)((f / freq) / 2));
//                 }
//                 else {
//                         __HAL_TIM_SET_AUTORELOAD(htim, 0);
//                         __HAL_TIM_SET_COMPARE(htim, channel, 10);
//                 }
//         }
// }

void A4988_setDirection(struct A4988 *stepper, uint32_t dir)
{
        if (dir) {
                HAL_GPIO_WritePin(stepper->dir_port, stepper->dir_pin, GPIO_PIN_SET);
        }
        else {
                HAL_GPIO_WritePin(stepper->dir_port, stepper->dir_pin, GPIO_PIN_RESET);
        }
}

void A4988_toggleDirection(struct A4988 *stepper)
{
        HAL_GPIO_TogglePin(stepper->dir_port, stepper->dir_pin);
}

void A4988_step(struct A4988 *stepper)
{
        HAL_GPIO_WritePin(stepper->step_port, stepper->step_pin, GPIO_PIN_RESET);
        asm volatile("nop");
        asm volatile("nop");
        HAL_GPIO_WritePin(stepper->step_port, stepper->step_pin, GPIO_PIN_SET);
        for (uint32_t i = 0; i < 10; ++i) {
                asm volatile("nop");
        }
        HAL_GPIO_WritePin(stepper->step_port, stepper->step_pin, GPIO_PIN_RESET);
        asm volatile("nop");
        asm volatile("nop");
}
