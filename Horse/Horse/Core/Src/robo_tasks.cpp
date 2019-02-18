/*
 *
 */

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

extern "C" void StartDefaultTask(void const *argument);
extern "C" void MotorThread(void const *argument);
extern "C" void PneumaticThread(void const *argument);
extern "C" void MovingMassThread(void const *argument);

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
                printf("Hello from Default Thread!!\n");
                osDelay(100);
        }
        /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_MotorThread */
/**
* @brief Function implementing the Motor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MotorThread */
void MotorThread(void const *argument)
{
        /* USER CODE BEGIN MotorThread */

        /* Infinite loop */
        for (;;)
        {
                printf("Hello from Motor Thread!!\n");
                osDelay(10);
        }
        /* USER CODE END MotorThread */
}

/* USER CODE BEGIN Header_PneumaticThread */
/**
* @brief Function implementing the Pnematic thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PneumaticThread */
void PneumaticThread(void const *argument)
{
        /* USER CODE BEGIN PneumaticThread */

        /* Infinite loop */
        for (;;)
        {
                printf("Hello from Pnematic Thread!!\n");
                osDelay(20);
        }
        /* USER CODE END PneumaticThread */
}

/* USER CODE BEGIN Header_MovingMassThread */
/**
* @brief Function implementing the MovingMass thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MovingMassThread */
void MovingMassThread(void const *argument)
{
        /* USER CODE BEGIN MovingMassThread */

        /* Infinite loop */
        for (;;)
        {
                printf("Hello from MovingMass Thread!!\n");
                osDelay(20);
        }
        /* USER CODE END MovingMassThread */
}

// *** EOF ***