/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define int_leg4_Pin GPIO_PIN_3
#define int_leg4_GPIO_Port GPIOE
#define int_leg4_EXTI_IRQn EXTI3_IRQn
#define extra_2_Pin GPIO_PIN_5
#define extra_2_GPIO_Port GPIOE
#define extra_2_EXTI_IRQn EXTI9_5_IRQn
#define extra_3_Pin GPIO_PIN_13
#define extra_3_GPIO_Port GPIOC
#define extra_3_EXTI_IRQn EXTI15_10_IRQn
#define extra_4_Pin GPIO_PIN_14
#define extra_4_GPIO_Port GPIOC
#define extra_4_EXTI_IRQn EXTI15_10_IRQn
#define extra_5_Pin GPIO_PIN_15
#define extra_5_GPIO_Port GPIOC
#define extra_5_EXTI_IRQn EXTI15_10_IRQn
#define pneu_leg1_Pin GPIO_PIN_0
#define pneu_leg1_GPIO_Port GPIOC
#define pneu_leg2_Pin GPIO_PIN_1
#define pneu_leg2_GPIO_Port GPIOC
#define pneu_leg3_Pin GPIO_PIN_2
#define pneu_leg3_GPIO_Port GPIOC
#define pneu_leg4_Pin GPIO_PIN_3
#define pneu_leg4_GPIO_Port GPIOC
#define ch2_leg2_Pin GPIO_PIN_1
#define ch2_leg2_GPIO_Port GPIOA
#define ch1_leg2_Pin GPIO_PIN_5
#define ch1_leg2_GPIO_Port GPIOA
#define ch1_leg3_Pin GPIO_PIN_6
#define ch1_leg3_GPIO_Port GPIOA
#define ch2_leg3_Pin GPIO_PIN_7
#define ch2_leg3_GPIO_Port GPIOA
#define extra_6_Pin GPIO_PIN_0
#define extra_6_GPIO_Port GPIOB
#define extra_7_Pin GPIO_PIN_2
#define extra_7_GPIO_Port GPIOB
#define extra_8_Pin GPIO_PIN_8
#define extra_8_GPIO_Port GPIOE
#define ch1_leg1_Pin GPIO_PIN_9
#define ch1_leg1_GPIO_Port GPIOE
#define ch2_leg1_Pin GPIO_PIN_11
#define ch2_leg1_GPIO_Port GPIOE
#define extra_9_Pin GPIO_PIN_12
#define extra_9_GPIO_Port GPIOE
#define ch1_leg4_Pin GPIO_PIN_12
#define ch1_leg4_GPIO_Port GPIOD
#define ch2_leg4_Pin GPIO_PIN_13
#define ch2_leg4_GPIO_Port GPIOD
#define pwm_leg1_Pin GPIO_PIN_6
#define pwm_leg1_GPIO_Port GPIOC
#define pwm_leg2_Pin GPIO_PIN_7
#define pwm_leg2_GPIO_Port GPIOC
#define pwm_leg3_Pin GPIO_PIN_8
#define pwm_leg3_GPIO_Port GPIOC
#define pwm_leg4_Pin GPIO_PIN_9
#define pwm_leg4_GPIO_Port GPIOC
#define dir1_leg1_Pin GPIO_PIN_8
#define dir1_leg1_GPIO_Port GPIOA
#define dir2_leg1_Pin GPIO_PIN_15
#define dir2_leg1_GPIO_Port GPIOA
#define dir1_leg2_Pin GPIO_PIN_11
#define dir1_leg2_GPIO_Port GPIOC
#define dir2_leg2_Pin GPIO_PIN_3
#define dir2_leg2_GPIO_Port GPIOD
#define dir1_leg3_Pin GPIO_PIN_4
#define dir1_leg3_GPIO_Port GPIOD
#define dir2_leg3_Pin GPIO_PIN_5
#define dir2_leg3_GPIO_Port GPIOD
#define dir1_leg4_Pin GPIO_PIN_7
#define dir1_leg4_GPIO_Port GPIOD
#define dir2_leg4_Pin GPIO_PIN_5
#define dir2_leg4_GPIO_Port GPIOB
#define int_leg1_Pin GPIO_PIN_7
#define int_leg1_GPIO_Port GPIOB
#define int_leg1_EXTI_IRQn EXTI9_5_IRQn
#define int_leg2_Pin GPIO_PIN_9
#define int_leg2_GPIO_Port GPIOB
#define int_leg2_EXTI_IRQn EXTI9_5_IRQn
#define int_leg3_Pin GPIO_PIN_1
#define int_leg3_GPIO_Port GPIOE
#define int_leg3_EXTI_IRQn EXTI1_IRQn

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
 #define USE_FULL_ASSERT    1U 

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
