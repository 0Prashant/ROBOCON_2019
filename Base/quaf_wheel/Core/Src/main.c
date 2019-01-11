
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "robo_init.h"
#include "pid.h"
#include <stdlib.h>
#include "motionProfile.h"
#include "leg.h"

#define CALIBRATE_SPEED (-4.0) /* In omega */
#define HOME_POS_OMEGA (-4.0) 
#define MAX_POSSIBLE_ANGLE (140)

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
       
    // global variables
    int16_t ref_cnt[4]   = {0, 0, 0, 0};          // last count when interrupt occured
    int16_t curr_cnt[4]  = {0, 0, 0, 0};          // current count for angle calculation 
    uint16_t last_cnt[4] = {0, 0, 0, 0};
    
    float inst_angle[4]     = {0, 0, 0, 0};       // angle at the instant
    float update_omega[4]   = {0, 0, 0, 0};
 
    uint32_t dt_ms = 9L;
    float dt_secs  = 0.009;
    
    float all_set_points;
    
    struct Str_pid  pid[4];
    //global flags
    uint8_t _CALIBRATE_FLAG = 1;
    uint8_t _AND = 1;
    uint8_t _ENCODER_FLAG = 0;
    uint8_t _PHASE = 0;
    uint16_t _CHECK_VARIABLE = 111;
    uint8_t _PHASE_CHANGE_FLAG = 0;
    uint8_t _RAISE_MOTOR_INDEX = 0;
    uint8_t _START_LOOP_FLAG = 0;
    uint8_t _NEXT_MOTOR_FLAG = 0;
    

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


 void get_Angles()
{
    for(uint8_t i = 0; i < 4; i++)
    {
        float temp_angle = 0.0901803 * (abs(curr_cnt[i]) - abs(ref_cnt[i]));
        while(temp_angle >= 360)
        {
            temp_angle = temp_angle - 360 ;
        }
        inst_angle[i] = temp_angle;
    }
}

void raise_Leg(struct Leg_str *leg)
{
    HAL_GPIO_WritePin(leg->pneu_port, leg->pneu_pin,GPIO_PIN_SET);
}


void lower_Leg(struct Leg_str *leg)
{
    HAL_GPIO_WritePin(leg->pneu_port, leg->pneu_pin,GPIO_PIN_RESET);
}



void set_HomePosition()
{
    _CALIBRATE_FLAG = 0;
    for(uint8_t i = 0; i < 4; i++)
    {
        set_Omega(&leg_arr[i], CALIBRATE_SPEED);
       
    }
}

void update_Omegas(float arr[4])
{
  
    for(uint8_t i = 0; i < 4; i++)
    {   
        set_Omega(&leg_arr[i], arr[i]);
       
    }
}


void test_Motors(uint16_t dtyCycle, enum motor_Direction d)
{
    for(uint8_t i = 0; i < 4; i++)
    {
        set_DutyCycle(&leg_arr[i], dtyCycle);
        set_MotorDirection(&leg_arr[i], d);
    }
}

void encoder_Init()
{
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}

uint8_t check_Angle(float angle)
{   
    uint16_t check_variable = 0;
    for(uint8_t i=0; i<4; i++)
    {
        if( (profile_arr[i].status == COMPLETE) && (i != _RAISE_MOTOR_INDEX ))
        {
            check_variable = check_variable * 10 + 1 ; 
        }
    }
    if (check_variable == _CHECK_VARIABLE) return 1;
    else return 0;
}

void calculate_setPoints()
{
    
     
    for(int i=0; i<4; i++)
    {
        int16_t counts_per_st = curr_cnt[i] - last_cnt[i];
        last_cnt[i]      = curr_cnt[i];
    
        float omega      = 0.71922 * counts_per_st / dt_ms;
        if((i == _RAISE_MOTOR_INDEX) && !_START_LOOP_FLAG)
        {
            raise_Leg(&leg_arr[_RAISE_MOTOR_INDEX]);
			
		    int temp_angle = i * (-60);		
            motionProfile_setAngle(&profile_arr[i], temp_angle);
            
            float set_point  = motionProfile_Generate(&profile_arr[i],inst_angle[i]);
            update_omega[i]  = pid_Compute(&pid[i], set_point, omega, dt_ms);
           
            if((profile_arr[i].status == COMPLETE) && (inst_angle[i] >= temp_angle))
            {   
                _START_LOOP_FLAG = 1; 
                update_omega[_RAISE_MOTOR_INDEX] = 0;
              
            }       
            
        }    
       
       if((i != _RAISE_MOTOR_INDEX) && _START_LOOP_FLAG )
       {
           motionProfile_setAngle(&profile_arr[i],60);
           float set_point  = motionProfile_Generate(&profile_arr[i],inst_angle[i]);               
           update_omega[i]  = pid_Compute(&pid[i], set_point, omega, dt_ms);
       }   
    }       
}

void reset_Angles()
{
    ref_cnt[0] = TIM1->CNT;
    ref_cnt[1] = TIM2->CNT;
    ref_cnt[2] = TIM3->CNT;
    ref_cnt[3] = TIM4->CNT;
    
}

void restart()
{
    for(uint8_t i=0; i<4; i++)
    {
        motionProfile_resetParams(&profile_arr[i]);
        motionProfile_restart(&profile_arr[i]);
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
	uint32_t break_tick  = HAL_GetTick(); 
    uint32_t omega_update_tick  = HAL_GetTick();

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
  MX_TIM8_Init();
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	
	leg_init();
    
    set_OutputLimit(&pid[0],12,-12);
    set_OutputLimit(&pid[1],12,-12);
    set_OutputLimit(&pid[2],12,-12);
    set_OutputLimit(&pid[3],12,-12);
   
   
    HAL_TIM_Base_Start(&htim8);
    
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);    
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
    
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER 
  CODE BEGIN WHILE */
    
	
    set_Gains(&pid[0], 0.56, 0, 0);
    set_Gains(&pid[1], 0.56, 0, 0);
    set_Gains(&pid[2], 0.56, 0, 0);
    set_Gains(&pid[3], 0.56, 0, 0);
    
    motionProfile_Init(&profile_arr[0], 5, 60, 50, dt_secs);
    motionProfile_Init(&profile_arr[1], 5, 60, 50, dt_secs);
    motionProfile_Init(&profile_arr[2], 5, 60, 50, dt_secs);
    motionProfile_Init(&profile_arr[3], 5, 60, 50, dt_secs);
   
    set_HomePosition();
	
      
    
   while(1) 
	{
       
        	curr_cnt[0]   = TIM1->CNT;
        	curr_cnt[1]   = TIM2->CNT;    
        	curr_cnt[2]   = TIM3->CNT;    
        	curr_cnt[3]   = TIM4->CNT;    
	
        	if(_ENCODER_FLAG)
        	{
        	    encoder_Init();
        	    _ENCODER_FLAG = 0;
	
        	}
	
        	get_Angles();  
       		if((HAL_GetTick()- update_tick) > dt_ms)
        	{
        	    update_tick   = HAL_GetTick();
	
        		if(_CALIBRATE_FLAG)
        		{   
        		     calculate_setPoints();
        		     update_Omegas(update_omega);
        		     _PHASE_CHANGE_FLAG = check_Angle(60);

        		     printf("%f \t %f \t %f \t %f \n", inst_angle[0], inst_angle[1], inst_angle[2], inst_angle[3] );
        		     if(_PHASE_CHANGE_FLAG)
        		     {
						 HAL_Delay(1000);
        		         lower_Leg(&leg_arr[_RAISE_MOTOR_INDEX]);
        		         printf("Phase change flag:%d\n",_PHASE_CHANGE_FLAG);

        		         _START_LOOP_FLAG = 0;
        		         _PHASE_CHANGE_FLAG = 0;

        		         _RAISE_MOTOR_INDEX  = (_RAISE_MOTOR_INDEX  + 1) ;
        		         reset_Angles();
        		         restart();


        		     }

        		}                

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
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
    
    if(!_CALIBRATE_FLAG)
    {
            if (GPIO_PIN == GPIO_PIN_13)    // PC13
    {   
        ref_cnt[0] = TIM1->CNT; // reset count for angle calculation 
        printf("Hello from motor 1 :%d\n",_AND);
        if(!_CALIBRATE_FLAG) set_Omega(&leg_arr[0], 0);
        _AND++;
            
    }
        
    if (GPIO_PIN == GPIO_PIN_3)     // PE3
    {   
        ref_cnt[1] = TIM2->CNT; // reset count for angle calculation   
        printf("Hello from motor 2 :%d\n",_AND);
        if(!_CALIBRATE_FLAG) set_Omega(&leg_arr[1], 0);
        _AND++;
     }
    
    if (GPIO_PIN == GPIO_PIN_5)     // PE5
    {   
        ref_cnt[2] = TIM3->CNT; // reset count for angle calculation
        printf("Hello from motor 3:%d\n",_AND);    
        if(!_CALIBRATE_FLAG) set_Omega(&leg_arr[2], 0);
        _AND++;
    }
    
    if (GPIO_PIN == GPIO_PIN_1)     // PE1
    {   
        ref_cnt[3] = TIM4->CNT; // reset count for angle calculation
        printf("Hello from motor 4:%d \n",_AND);    
        if(!_CALIBRATE_FLAG) set_Omega(&leg_arr[3], 0);
        _AND++;
    }
    if(!_CALIBRATE_FLAG && _AND == 5)
    { 
        _ENCODER_FLAG = 1;
        _CALIBRATE_FLAG = 1;
      
       return;
    }          
    } 
    
   
    
}

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

