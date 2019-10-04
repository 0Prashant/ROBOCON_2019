#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "robot.h"
#include "robot_functions.h"

extern bool ROBOT_START_FLAG;
extern Robot_States robo_state;
extern float angles[7];
extern leg leg[2];
extern "C" void StartDefaultTask(void const *argument);
extern "C" void StartRobotTask(void const *argument);
extern "C" void StartLoggerTask(void const *argument);
extern "C" void StartCalculationTask(void const *argument);


/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{

	/* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for (;;)
	{
		osDelay(1);
	}
	/* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartRobotTask */
/**
* @brief Function implementing the RobotTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRobotTask */
void StartRobotTask(void const *argument)
{
	/* USER CODE BEGIN StartRobotTask */
	uint8_t sample_time = 10;
	uint32_t dt = HAL_GetTick();
	initialize_position();
	// getup_n_run();
	ROBOT_START_FLAG = false;
	robo_state = HOME;
	// zone_select();
	
	for(int i=0; i<7; i++){
		angles[i] *= -1;
	}
	/* Infinite loop */
	for (;;)
	{
		dt = HAL_GetTick();
		play();		
		dt = HAL_GetTick() - dt;
		osDelay(sample_time - dt);
	}
	/* USER CODE END StartRobotTask */
}

/* USER CODE BEGIN Header_StartLoggerTask */
/**
* @brief Function implementing the LoggerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLoggerTask */
void StartLoggerTask(void const *argument)
{
	/* USER CODE BEGIN StartLoggerTask */
	/* Infinite loop */
	uint8_t sample_time = 50;
	uint32_t dt = HAL_GetTick();
	for (;;)
	{
		dt = HAL_GetTick();

		dt = HAL_GetTick() - dt;
		osDelay(sample_time - dt);
	}
	/* USER CODE END StartLoggerTask */
}

/* USER CODE BEGIN Header_StartCalculationTask */
/**
* @brief Function implementing the CalculationTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCalculationTask */
void StartCalculationTask(void const *argument)
{
	/* USER CODE BEGIN StartCalculationTask */
	/* Infinite loop */
	uint8_t sample_time = 10;
	uint32_t dt = HAL_GetTick();
	for (;;)
	{
		dt = HAL_GetTick();
		calculate_datas();
		dt = HAL_GetTick() - dt;
		osDelay(sample_time - dt);
	}
	/* USER CODE END StartCalculationTask */
}