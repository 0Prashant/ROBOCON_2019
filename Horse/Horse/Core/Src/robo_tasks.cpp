/*
 *
 */

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "robo_states.h"
#include "robo_init.h"
#include "fsm_tasks.h"
#include "pneumatic.h"
#include "blnc_motor.h"

extern "C" void StartDefaultTask(void const *argument);
extern "C" void MotorThread(void const *argument);
extern "C" void PneumaticThread(void const *argument);
extern "C" void MovingMassThread(void const *argument);

// This is the state of the robot that only the MotorThread is allowed to
// modify

State gHorse_State = State::get_Instance();

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
                // printf("Hello from Default Thread!!\n");
                osDelay(10000);
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
        roboInit();
        bool switch_pressed = false;

        uint32_t sample_period = 10;
        uint32_t dt = HAL_GetTick();
        uint32_t dt_tmp = HAL_GetTick();
        uint32_t last_run_time = 0;
        osDelay(sample_period);

        // Khangai_Robot.check_Actuators();

        /* Infinite loop */
        for (;;)
        {
                // Since this is the highest priority task, we can be sure that
                // another task won't start when this task is running
                dt_tmp = HAL_GetTick();
                dt = dt_tmp - dt;
                
                // Run Code Here
                if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) && !switch_pressed)
                {
                        switch_pressed = true;
                        gHorse_State.update_State();
                }
                else
                {
                        startFSM(&fsm);
                        updateOmegas();
                }
  
                dt = HAL_GetTick();
                dt_tmp = dt - dt_tmp;
                last_run_time = dt_tmp;

                // Check for timing Issues
                if (last_run_time > sample_period / 2) {
                        // Timing Issue Occured since run time is more than half
                        // of sample time
                }

                // Sleep for remaining time of the sampling period if there is
                // time left
                if (dt_tmp < sample_period) {
                        osDelay(sample_period - dt_tmp);
                }
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
        pneu_Init();

        uint32_t sample_period = 50;
        uint32_t dt = HAL_GetTick();
        uint32_t dt_tmp = HAL_GetTick();
        uint32_t last_run_time = 0;
        osDelay(sample_period);

        /* Infinite loop */
        for (;;)
        {
                // Since this is the highest priority task, we can be sure that
                // another task won't start when this task is running
                dt_tmp = HAL_GetTick();
                dt = dt_tmp - dt;

                // Run Code Here
                pneu_Loop();
  
                dt = HAL_GetTick();
                dt_tmp = dt - dt_tmp;
                last_run_time = dt_tmp;

                // Check for timing Issues
                if (last_run_time > sample_period / 2) {
                        // Timing Issue Occured since run time is more than half
                        // of sample time
                }

                // Sleep for remaining time of the sampling period if there is
                // time left
                if (dt_tmp < sample_period) {
                        osDelay(sample_period - dt_tmp);
                }
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
        balance_Init();

        uint32_t sample_period = 20;
        uint32_t dt = HAL_GetTick();
        uint32_t dt_tmp = HAL_GetTick();
        uint32_t last_run_time = 0;
        osDelay(sample_period);

        /* Infinite loop */
        for (;;)
        {
                // Since this is the highest priority task, we can be sure that
                // another task won't start when this task is running
                dt_tmp = HAL_GetTick();
                dt = dt_tmp - dt;

                // Run Code Here
                balance_Loop();
  
                dt = HAL_GetTick();
                dt_tmp = dt - dt_tmp;
                last_run_time = dt_tmp;

                // Check for timing Issues
                if (last_run_time > sample_period / 2) {
                        // Timing Issue Occured since run time is more than half
                        // of sample time
                }

                // Sleep for remaining time of the sampling period if there is
                // time left
                if (dt_tmp < sample_period) {
                        osDelay(sample_period - dt_tmp);
                }
        }
        /* USER CODE END MovingMassThread */
}

// *** EOF ***