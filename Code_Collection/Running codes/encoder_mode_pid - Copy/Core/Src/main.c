
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
#include "dma.h"

/* USER CODE BEGIN Includes */
#include "robo_init.h"
#include "robot.h"
#include "pid.h"
#include "filter.h"
#include "thinker.h"


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */\

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

void get_Counts(int16_t cps[4])
{
    cps[0] = TIM2->CNT; // motor 1 
    TIM2->CNT = 0;

    cps[1] = TIM3->CNT; // motor 2
    TIM3->CNT = 0;

    cps[2] = TIM1->CNT; // motor 3 
    TIM1->CNT = 0;

    cps[3] = TIM4->CNT; // motor 4 
    TIM4->CNT = 0;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *'
  * @retval None
  */

int main(void)
{
  /* USER CODE BEGIN 1 */
	
	uint32_t update_tick = HAL_GetTick();
	uint32_t break_tick  = HAL_GetTick();
	
	int16_t counts_per_st[4] = {0,0,0,0};
	float omega[4] = {0,0,0,0};
	float new_omega[4] = {0,0,0,0};
	uint32_t sample_time = 10L;
	
	
	
	float sp = 5;
	float set_point[] = {sp,sp,sp,sp};
	uint16_t dt = 10;
	float p=0.019;
	float i=1.2;
	float d=0;
	struct Str_pid pid[4];

	
	// flags
	int break_flag = 0;
	
	// variables for serial communications
	uint8_t rx_input;
	int tx_length;
	char tx_str_omega[15];
	
	
	
								
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM8_Init();
  MX_USART3_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	
	
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
	
	wheel_init();
	Thinker_Init();
    for (uint8_t i = 0; i < 4; ++i) {
        init_PID(&pid[i]);
    }
    
	HAL_TIM_Base_Start(&htim8);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
	
		
	//set_DutyCycle(&Wheel_arr[0],65535);
	//set_DutyCycle(&Wheel_arr[1],65535);
	//set_DutyCycle(&Wheel_arr[2],65535);
	//set_DutyCycle(&Wheel_arr[3],65535);
	//
	//set_Direction(&Wheel_arr[0],DIR_AHEAD);
	//set_Direction(&Wheel_arr[1],DIR_BACK);
	//set_Direction(&Wheel_arr[2],DIR_BACK);
	//set_Direction(&Wheel_arr[3],DIR_AHEAD);
        
			
		set_OutputLimit(&pid[0],12,-12);
		set_OutputLimit(&pid[1],12,-12);
		set_OutputLimit(&pid[2],12,-12);
		set_OutputLimit(&pid[3],12,-12);
        
					
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
		
    #define OMEGA_UPDATE_TIME (40)
    
	uint32_t omega_update_tick = HAL_GetTick();
	
	while(1)
	{
		set_Gains(&pid[0],p, i, d );
		set_Gains(&pid[1],p, i, d );	
		set_Gains(&pid[2],p, i, d );	
		set_Gains(&pid[3],p, i, d );
        /* 
		if ((HAL_GetTick() - omega_update_tick) > OMEGA_UPDATE_TIME){
           omega_update_tick = HAL_GetTick();
           if (!(Thinker_isEmpty()))
               get_Thinker(set_point);
       }	
		*/
		if((HAL_GetTick()- update_tick) > sample_time)
		{
            update_tick = HAL_GetTick();
            get_Counts(counts_per_st);
            
            for (uint8_t i = 0; i < 4; ++i) {
                omega[i] =  6.29325 * counts_per_st[i]/sample_time;					
                new_omega[i] = pid_Compute(&pid[i],set_point[i],omega[i],dt);
            }

            for (uint8_t i = 0;i< 4; ++i) {
                set_Omega(&Wheel_arr[i],new_omega[i]);
                printf("%f ", omega[i]);
            }
            printf("\n");
		}
	}

	
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */

	}


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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
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
