
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

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

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

#define MAX_QUEUE8_SIZE (32)
#define MAX_QUEUE16_SIZE (32)

struct Queue8
{
	uint8_t queue_[MAX_QUEUE8_SIZE];
	uint32_t tail_;
	uint32_t head_;
	uint32_t curr_size_;
};

struct Queue16
{
	int16_t queue_[MAX_QUEUE16_SIZE];
	uint32_t tail_;
	uint32_t head_;
	uint32_t curr_size_;
};

void Queue8_Init(struct Queue8 *q)
{
	q->head_ = 0;
	q->tail_ = 0;
	q->curr_size_ = 0;
}

void Queue8_insert(struct Queue8 *q,uint8_t data)
{
	++q->curr_size_;
	q->queue_[q->head_++] = data; 
	q->head_ %= MAX_QUEUE8_SIZE;
}

uint8_t Queue8_lookup(struct Queue8 *q)
{
	if(q->curr_size_)
	{
		--q->curr_size_;
		uint8_t data = q->queue_[q->tail_++];
		q->tail_ %= MAX_QUEUE8_SIZE;
		return data;
	}
	return 0;
}


uint32_t Queue8_length(struct Queue8 *q)
{
	return q->curr_size_;
}

void Queue16_Init(struct Queue16 *q)
{
	q->head_ = 0;
	q->tail_ = 0;
	q->curr_size_ = 0;
}

void Queue16_insert(struct Queue16 *q,int16_t data)
{
	++q->curr_size_;
	q->queue_[q->head_++] = data; 
	q->head_ %= MAX_QUEUE16_SIZE;
}

int16_t Queue16_lookup(struct Queue16 *q)
{
	if(q->curr_size_)
	{
		--q->curr_size_;
		int16_t data = q->queue_[q->tail_++];
		q->tail_ %= MAX_QUEUE16_SIZE;
		return data;
	}
	return 0;
}

uint32_t Queue16_length(struct Queue16 *q)
{
	return q->curr_size_;
}

#define NUM_OMEGAS_RECEIVED	(8)
#define MAX_POSSIBLE_OMEGA	(50)

static uint8_t gThinker_Data;
static uint8_t gStart_Byte_Received = 0;
static uint8_t gNum_Data_Received = 0;
static int16_t gOmega;

static struct Queue8 gThinker_Bytes;
static struct Queue16 gThinker_Omegas[4];

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
					gOmega = (uint16_t)(Queue8_lookup(&gThinker_Bytes) << 8);
					gOmega += (uint16_t)(Queue8_lookup(&gThinker_Bytes));
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
  Thinker_Init();
  HAL_Delay(100);
  printf("Hello World!!\n");

  float curr_omegas[4];
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
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
