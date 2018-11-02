
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "pid.h"
#include <stdlib.h>

/* USER CODE BEGIN Includes */
#include "robo_init.h"
#include "robot.h"
#include "pid.h"




/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

#define PI 3.14159
int32_t counts[4] = {0,0,0,0};
/* USER CODE BEGIN PV */


/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/


/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

	
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


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
        switch (GPIO_Pin) {
                case GPIO_PIN_1 : {             // PA1
                        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
                        {
                                ++counts[0];
                        }
                        else
                        {
                                --counts[0];
                        }
                } break;

                case GPIO_PIN_7 : {             // PA7
                        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
                        {
                                ++counts[1];
                        }
                        else
                        {
                                --counts[1];
                        }
                } break;

                case GPIO_PIN_11 : {            // PE11
                        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9))
                        {
                                ++counts[2];
                        }
                        else 
                        {
                                --counts[2];
                        }
                } break;

                case GPIO_PIN_13 : {            // PD13
                        if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12))
                        {
                                ++counts[3];
                        }
                        else
                        {
                                --counts[3];
                        }
                } break;

                default : {

                }
        }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
	
{
        /* USER CODE BEGIN 1 */
 	uint32_t update_tick = HAL_GetTick();
	uint32_t sample_time = 15L;
	uint16_t dt = 15;  
		uint32_t break_tick ;
	
		float omega[4] = {0,0,0,0};
		float new_omega[4] = {0,0,0,0};
					

		uint16_t counts_per_st[4] = {0,0,0,0};
		struct Str_pid pid_1;
		struct Str_pid pid_2;
		struct Str_pid pid_3;
		struct Str_pid pid_4;

				
        /* USER CODE END 1 */

        /* MCU Configuration----------------------------------------------------------*/

        /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
        HAL_Init();

        /* USER CODE BEGIN Init */

        /* USER CODE END Init */

        /* Configure the system clock */
        SystemClock_Config();

        /* USER CODE BEGIN SysInit */

        /* USER CODE END SysInit */

        /* Initialize all configured peripherals */
        MX_GPIO_Init();
        MX_ADC1_Init();
        MX_TIM8_Init();
        MX_USART1_UART_Init();
        MX_USART3_UART_Init();
     
        /* USER CODE BEGIN 2 */
				
				wheel_init();
				
				
				set_OutputLimit(&pid_1,120,-120);	
				set_OutputLimit(&pid_2,120,-120);
				set_OutputLimit(&pid_3,120,-120);
				set_OutputLimit(&pid_4,120,-120);
		

				HAL_TIM_Base_Start(&htim8);
				HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
				HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
				HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
				HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
		    uint8_t data[2];
				
        /* USER CODE END 2 */

        /* Infinite loop */
			       
				MX_USART1_UART_Init();
				int break_flag;
				while (1)
				{
        
				/* USER CODE BEGIN WHILE */
				    break_tick = HAL_GetTick(); 
					
				
					if ((HAL_GetTick()- update_tick)> sample_time)
					{  
						
						update_tick = HAL_GetTick();
						
						set_Gains(&pid_1,0.5,0,0 );
						set_Gains(&pid_2,0.5,0,0 );	
						set_Gains(&pid_3,0.5,0,0 );	
						set_Gains(&pid_4,0.5,0,0 );
					
		      
						counts_per_st[0] = abs(counts[0]); 
						counts[0] = 0;
						
						
						counts_per_st[1] = abs(counts[1]); 
						counts[1] = 0;
						
						counts_per_st[2] = abs(counts[2]); 
						counts[2] = 0;
						
						counts_per_st[3] = abs(counts[3]); 
						counts[3] = 0;
					
				
						omega[0] = 25.173 * counts_per_st[0]/dt;					
						new_omega[0] = pid_Compute(&pid_1,15,omega[0],dt);
						
						omega[1] = 25.173 * counts_per_st[1]/dt;					
						new_omega[1] = pid_Compute(&pid_2,15,omega[1],dt);
						
						omega[2] = 25.173 * counts_per_st[2]/dt;					
						new_omega[2] = pid_Compute(&pid_3,15,omega[2],dt);
						
						omega[3] = 25.173 * counts_per_st[3]/dt;					
						new_omega[3] = pid_Compute(&pid_4,15,omega[3],dt);
				   
						set_Omega(&Wheel_arr[0],new_omega[0]);
						set_Omega(&Wheel_arr[1],new_omega[1]);
						set_Omega(&Wheel_arr[2],new_omega[2]);
						set_Omega(&Wheel_arr[3],new_omega[3]);
						
						
						printf(" %f\t %f \t %f \t %f  \n",omega[0],omega[1],omega[2],omega[3]);
					
					
				  }
					if(break_flag == 1)
					{
						break;
					}
			
				
					  
				}				
					/* USER CODE END WHILE */
            /* USER CODE BEGIN 3 */
		set_Omega(&Wheel_arr[0], 0);
		set_Omega(&Wheel_arr[1], 0);
		set_Omega(&Wheel_arr[2], 0);
		set_Omega(&Wheel_arr[3], 0);			
}
		
						
        /* USER CODE END 3 */
	 
			
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
        RCC_OscInitTypeDef RCC_OscInitStruct;
        RCC_ClkInitTypeDef RCC_ClkInitStruct;

        /**Configure the main internal regulator output voltage 
    */
        __HAL_RCC_PWR_CLK_ENABLE();

        __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

        /**Initializes the CPU, AHB and APB busses clocks 
    */
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        RCC_OscInitStruct.HSEState = RCC_HSE_ON;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLM = 8;
        RCC_OscInitStruct.PLL.PLLN = 336;
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
        RCC_OscInitStruct.PLL.PLLQ = 4;
        if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        {
                _Error_Handler(__FILE__, __LINE__);
        }

        /**Initializes the CPU, AHB and APB busses clocks 
    */
        RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

        if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
        {
                _Error_Handler(__FILE__, __LINE__);
        }

        /**Configure the Systick interrupt time 
    */
        HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

        /**Configure the Systick 
    */
        HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

        /* SysTick_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
        /* USER CODE BEGIN Error_Handler_Debug */
        /* User can add his own implementation to report the HAL error return state */
        while (1)
        {
        }
        /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
        /* USER CODE BEGIN 6 */
        /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
        /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
