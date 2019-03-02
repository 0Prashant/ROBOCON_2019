
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
#include "dma.h"
#include "usart.h"
#include "gpio.h"


/* USER CODE BEGIN Includes */
#include "uart_tx.h"
#include "uart_rx.h"
/* USER CODE END Includes */
#include "uart_rx.h"
/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
 //uint8_t start_byte_received = 0;
 uint8_t aauta ;
 //struct Queue8 Q;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */


#define NUM_OMEGAS_RECEIVED	(8)
#define MAX_POSSIBLE_OMEGA	(50)

uint8_t gThinker_Data;
uint8_t gStart_Byte_Received = 0;
uint8_t gNum_Data_Received = 0;
int16_t gOmega;

struct Queue8 gThinker_Bytes;
struct Queue16 gThinker_Omegas[4];
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

void Thinker_Init(void)
{
	HAL_UART_Receive_DMA(&huart3, &gThinker_Data, 1);
	Queue8_Init(&gThinker_Bytes);
	for (uint8_t i = 0; i < 4; ++i) {
		Queue16_Init(&gThinker_Omegas[i]);
	}
}

void get_Thinker(float arr[4])
{
	for (uint8_t i= 0; i < 4; ++i) {
		arr[i] = Queue16_lookup(&gThinker_Omegas[i]);
		arr[i] *= (MAX_POSSIBLE_OMEGA / 32768.0);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 if(huart->Instance == USART3)
	 {
		if(!gStart_Byte_Received)
		{
			if (gThinker_Data == 0xA5) {
				gStart_Byte_Received = 1;
			}
		}
		else
		{
			Queue8_insert(&gThinker_Bytes, gThinker_Data);
			++gNum_Data_Received;
			if (gNum_Data_Received == NUM_OMEGAS_RECEIVED) {
				gNum_Data_Received = 0;
				gStart_Byte_Received = 0;
				// Omega calculation using little-endian
				for (uint8_t i = 0; i < 4; ++i) {
					gOmega = (uint8_t)(Queue8_lookup(&gThinker_Bytes) << 8);
					gOmega += (uint8_t)(Queue8_lookup(&gThinker_Bytes));
					Queue16_insert(&gThinker_Omegas[i], gOmega);
				}
			}
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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
//  char data;
  uint16_t omega[4] ={0x5A5A,9998,9997,9996}; //0x0001110101010110  
 // int tx_length;
 // char tx_str_omega[15];
 // char start_byte = 0XA5;
 // char stop_byte = 0X5A;
  
  Thinker_Init();
  float curr_omegas[4];
  HAL_Delay(100);
  printf("hello\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t sample = HAL_GetTick();
  while (1)	  
  {
	  if (HAL_GetTick() - sample > 50) {
		  sample = HAL_GetTick();
		  get_Thinker(curr_omegas);
		  for (uint8_t i = 0; i < 4; ++i) {
			  printf("%f  ", curr_omegas[i]);
		  }
		  printf("\n");
	  }
	//transmit_Data(&huart3, (uint8_t*)&omega[0], 2);
	 // HAL_UART_Receive_DMA(&huart3,(uint8_t*)&aauta, 9);
		
		
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  while(1)
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
