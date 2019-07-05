/* USER CODE BEGIN Header */
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
  * COPYRIGHT(c) 2019 STMicroelectronics
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void _Error_Handler(const char *file, size_t line);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ZONE_BLUE_Pin GPIO_PIN_4
#define ZONE_BLUE_GPIO_Port GPIOE
#define ZONE_INIT_Pin GPIO_PIN_14
#define ZONE_INIT_GPIO_Port GPIOC
#define USER_BUTTON_Pin GPIO_PIN_0
#define USER_BUTTON_GPIO_Port GPIOA
#define ZONE_45_Pin GPIO_PIN_4
#define ZONE_45_GPIO_Port GPIOA
#define LEG1_IR_Pin GPIO_PIN_14
#define LEG1_IR_GPIO_Port GPIOE
#define STEERING_IR_Pin GPIO_PIN_13
#define STEERING_IR_GPIO_Port GPIOB
#define LEG0_IR_Pin GPIO_PIN_15
#define LEG0_IR_GPIO_Port GPIOB
#define PROXIMITY_UP_Pin GPIO_PIN_8
#define PROXIMITY_UP_GPIO_Port GPIOD
#define GEREGE_SWITCH_Pin GPIO_PIN_9
#define GEREGE_SWITCH_GPIO_Port GPIOD
#define PROXIMITY_FRONT_Pin GPIO_PIN_10
#define PROXIMITY_FRONT_GPIO_Port GPIOD
#define PROXIMITY_BACK_Pin GPIO_PIN_11
#define PROXIMITY_BACK_GPIO_Port GPIOD
#define GreenLED_Pin GPIO_PIN_12
#define GreenLED_GPIO_Port GPIOD
#define OrangeLED_Pin GPIO_PIN_13
#define OrangeLED_GPIO_Port GPIOD
#define RedLED_Pin GPIO_PIN_14
#define RedLED_GPIO_Port GPIOD
#define BlueLED_Pin GPIO_PIN_15
#define BlueLED_GPIO_Port GPIOD
#define MOTOR0_IN1_Pin GPIO_PIN_8
#define MOTOR0_IN1_GPIO_Port GPIOA
#define MOTOR0_IN2_Pin GPIO_PIN_10
#define MOTOR0_IN2_GPIO_Port GPIOA
#define MOTOR1_IN2_Pin GPIO_PIN_15
#define MOTOR1_IN2_GPIO_Port GPIOA
#define ZONE_RED_Pin GPIO_PIN_10
#define ZONE_RED_GPIO_Port GPIOC
#define MOTOR1_IN1_Pin GPIO_PIN_11
#define MOTOR1_IN1_GPIO_Port GPIOC
#define ZONE_SAND_Pin GPIO_PIN_12
#define ZONE_SAND_GPIO_Port GPIOC
#define MOTOR2_IN2_Pin GPIO_PIN_0
#define MOTOR2_IN2_GPIO_Port GPIOD
#define ZONE_MOUNTAIN_Pin GPIO_PIN_1
#define ZONE_MOUNTAIN_GPIO_Port GPIOD
#define MOTOR2_IN1_Pin GPIO_PIN_2
#define MOTOR2_IN1_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
