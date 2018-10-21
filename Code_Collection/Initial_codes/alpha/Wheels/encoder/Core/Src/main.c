
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

/* USER CODE BEGIN Includes */
#include "defines.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

#define NUM_STEPS (253)

float deltas[NUM_STEPS][2] = {
    {0, 0},
    {0.1, 0},
    {0.1, 0.00333518672982534},
    {0.1, 0.00668153104781060},
    {0.1, 0.0100503781525921},
    {0.1, 0.0527644853011086},
    {0.1, 0.0577350269189626},
    {0.1, 0.0630488324991280},
    {0.1, 0.0687746384953325},
    {0.1, 0.0750000000000000},
    {0.1, 0.0818387935282501},
    {0.1, 0.0894427190999916},
    {0.1, 0.0980196058819607},
    {0.1, 0.107863874326001},
    {0.1, 0.119410050279768},
    {0.1, 0.133333333333333},
    {0.1, 0.150755672288882},
    {0.1, 0.173719807243076},
    {0.1, 0.206474160483506},
    {0.1, 0.259973473447873},
    {0.1, 0.307547841844389},
    {0.1, 0.307547841844389},
    {0.1, 0.307547841844389},
    {0.1, 0.307547841844389},
    {0.1, 0.377547841844389},
    {0.1, 0.377547841844389},
    {0.1, 0.377547841844389},
    {0.1, 0.377547841844389},
    {0.1, 0.307547841844389},
    {0.1, 0.307547841844389},
    {0.1, 0.307547841844389},
    {0.1, 0.307547841844389},
    {0.1, 0.259973473447873},
    {0.1, 0.206474160483506},
    {0.1, 0.173719807243076},
    {0.1, 0.150755672288882},
    {0.1, 0.133333333333333},
    {0.1, 0.119410050279768},
    {0.1, 0.107863874326001},
    {0.1, 0.0980196058819607},
    {0.1, 0.0894427190999916},
    {0.1, 0.0818387935282501},
    {0.1, 0.0750000000000000},
    {0.1, 0.0687746384953325},
    {0.1, 0.0630488324991280},
    {0.1, 0.0577350269189626},
    {0.1, 0.0527644853011086},
    {0.1, 0.0480822368945951},
    {0.1, 0.0436435780471985},
    {0.1, 0.0394115900145010},
    {0.1, 0.0353553390593274},
    {0.1, 0.0314485451016576},
    {0.1, 0.0276685785546430},
    {0.1, 0.0239956909566871},
    {0.1, 0.0204124145231931},
    {0.1, 0.0169030850945703},
    {0.1, 0.0134534558799262},
    {0.1, 0.0100503781525921},
    {0.1, 0.00668153104781060},
    {0.1, 0.00333518672982534},
    {0.1, -0.00333518672982534},
    {0.1, -0.00668153104781060},
    {0.1, -0.0100503781525921},
    {0.1, -0.0134534558799262},
    {0.1, -0.0169030850945703},
    {0.1, -0.0204124145231931},
    {0.1, -0.0239956909566871},
    {0.1, -0.0276685785546430},
    {0.1, -0.0314485451016576},
    {0.1, -0.0353553390593274},
    {0.1, -0.0394115900145010},
    {0.1, -0.0436435780471985},
    {0.1, -0.0480822368945951},
    {0.1, -0.0527644853011086},
    {0.1, -0.0577350269189626},
    {0.1, -0.0630488324991280},
    {0.1, -0.0687746384953325},
    {0.1, -0.0750000000000000},
    {0.1, -0.0818387935282501},
    {0.1, -0.0894427190999916},
    {0.1, -0.0980196058819607},
    {0.1, -0.107863874326001},
    {0.1, -0.119410050279768},
    {0.1, -0.133333333333333},
    {0.1, -0.150755672288882},
    {0.1, -0.173719807243076},
    {0.1, -0.206474160483506},
    {0.1, -0.259973473447873},
    {0.1, -0.307547841844389},
    {0.1, -0.307547841844389},
    {0.1, -0.307547841844389},
    {0.1, -0.307547841844389},
    {0.1, -0.377547841844389},
    {0.1, -0.377547841844389},
    {0.1, -0.307547841844389},
    {0.1, -0.307547841844389},
    {0.1, -0.307547841844389},
    {0.1, -0.307547841844389},
    {0.1, -0.259973473447873},
    {0.1, -0.206474160483506},
    {0.1, -0.173719807243076},
    {0.1, -0.150755672288882},
    {0.1, -0.133333333333333},
    {0.1, -0.119410050279768},
    {0.1, -0.107863874326001},
    {0.1, -0.0980196058819607},
    {0.1, -0.0894427190999916},
    {0.1, -0.0818387935282501},
    {0.1, -0.0750000000000000},
    {0.1, -0.0687746384953325},
    {0.1, -0.0630488324991280},
    {0.1, -0.0577350269189626},
    {0.1, -0.0527644853011086},
    {0.1, -0.0480822368945951},
    {0.1, -0.0436435780471985},
    {0.1, -0.0394115900145010},
    {0.1, -0.0353553390593274},
    {0.1, -0.0314485451016576},
    {0.1, -0.0276685785546430},
    {0.1, -0.0239956909566871},
    {0.1, -0.0204124145231931},
    {0.1, -0.0169030850945703},
    {0.1, -0.0134534558799262},
    {0.1, -0.0100503781525921},
    {0.075, -0.00668153104781060},
    {0.025, -0.00333518672982534},
    {-0.025, 0.00333518672982534},
    {-0.075, 0.00668153104781060},
    {-0.1, 0.0100503781525921},
    {-0.1, 0.0134534558799262},
    {-0.1, 0.0169030850945703},
    {-0.1, 0.0204124145231931},
    {-0.1, 0.0239956909566871},
    {-0.1, 0.0276685785546430},
    {-0.1, 0.0314485451016576},
    {-0.1, 0.0353553390593274},
    {-0.1, 0.0394115900145010},
    {-0.1, 0.0436435780471985},
    {-0.1, 0.0480822368945951},
    {-0.1, 0.0527644853011086},
    {-0.1, 0.0577350269189626},
    {-0.1, 0.0630488324991280},
    {-0.1, 0.0687746384953325},
    {-0.1, 0.0750000000000000},
    {-0.1, 0.0818387935282501},
    {-0.1, 0.0894427190999916},
    {-0.1, 0.0980196058819607},
    {-0.1, 0.107863874326001},
    {-0.1, 0.119410050279768},
    {-0.1, 0.133333333333333},
    {-0.1, 0.150755672288882},
    {-0.1, 0.173719807243076},
    {-0.1, 0.206474160483506},
    {-0.1, 0.259973473447873},
    {-0.1, 0.307547841844389},
    {-0.1, 0.307547841844389},
    {-0.1, 0.307547841844389},
    {-0.1, 0.307547841844389},
    {-0.1, 0.377547841844389},
    {-0.1, 0.377547841844389},
    {-0.1, 0.377547841844389},
    {-0.1, 0.377547841844389},
    {-0.1, 0.307547841844389},
    {-0.1, 0.307547841844389},
    {-0.1, 0.307547841844389},
    {-0.1, 0.307547841844389},
    {-0.1, 0.259973473447873},
    {-0.1, 0.206474160483506},
    {-0.1, 0.173719807243076},
    {-0.1, 0.150755672288882},
    {-0.1, 0.133333333333333},
    {-0.1, 0.119410050279768},
    {-0.1, 0.107863874326001},
    {-0.1, 0.0980196058819607},
    {-0.1, 0.0894427190999916},
    {-0.1, 0.0818387935282501},
    {-0.1, 0.0750000000000000},
    {-0.1, 0.0687746384953325},
    {-0.1, 0.0630488324991280},
    {-0.1, 0.0577350269189626},
    {-0.1, 0.0527644853011086},
    {-0.1, 0.0480822368945951},
    {-0.1, 0.0436435780471985},
    {-0.1, 0.0394115900145010},
    {-0.1, 0.0353553390593274},
    {-0.1, 0.0314485451016576},
    {-0.1, 0.0276685785546430},
    {-0.1, 0.0239956909566871},
    {-0.1, 0.0204124145231931},
    {-0.1, 0.0169030850945703},
    {-0.1, 0.0134534558799262},
    {-0.1, 0.0100503781525921},
    {-0.1, 0.00668153104781060},
    {-0.1, 0.00333518672982534},
    {-0.1, -0.00333518672982534},
    {-0.1, -0.00668153104781060},
    {-0.1, -0.0100503781525921},
    {-0.1, -0.0134534558799262},
    {-0.1, -0.0169030850945703},
    {-0.1, -0.0204124145231931},
    {-0.1, -0.0239956909566871},
    {-0.1, -0.0276685785546430},
    {-0.1, -0.0314485451016576},
    {-0.1, -0.0353553390593274},
    {-0.1, -0.0394115900145010},
    {-0.1, -0.0436435780471985},
    {-0.1, -0.0480822368945951},
    {-0.1, -0.0527644853011086},
    {-0.1, -0.0577350269189626},
    {-0.1, -0.0630488324991280},
    {-0.1, -0.0687746384953325},
    {-0.1, -0.0750000000000000},
    {-0.1, -0.0818387935282501},
    {-0.1, -0.0894427190999916},
    {-0.1, -0.0980196058819607},
    {-0.1, -0.107863874326001},
    {-0.1, -0.119410050279768},
    {-0.1, -0.133333333333333},
    {-0.1, -0.150755672288882},
    {-0.1, -0.173719807243076},
    {-0.1, -0.206474160483506},
    {-0.1, -0.259973473447873},
    {-0.1, -0.307547841844389},
    {-0.1, -0.307547841844389},
    {-0.1, -0.307547841844389},
    {-0.1, -0.307547841844389},
    {-0.1, -0.377547841844389},
    {-0.1, -0.377547841844389},
    {-0.1, -0.307547841844389},
    {-0.1, -0.307547841844389},
    {-0.1, -0.307547841844389},
    {-0.1, -0.307547841844389},
    {-0.1, -0.259973473447873},
    {-0.1, -0.206474160483506},
    {-0.1, -0.173719807243076},
    {-0.1, -0.150755672288882},
    {-0.1, -0.133333333333333},
    {-0.1, -0.119410050279768},
    {-0.1, -0.107863874326001},
    {-0.1, -0.0980196058819607},
    {-0.1, -0.0894427190999916},
    {-0.1, -0.0818387935282501},
    {-0.1, -0.0750000000000000},
    {-0.1, -0.0687746384953325},
    {-0.1, -0.0630488324991280},
    {-0.1, -0.0577350269189626},
    {-0.1, -0.0527644853011086},
    {-0.1, -0.0480822368945951},
    {-0.1, -0.00668153104781060},
    {0, -0.00333518672982534},
    {0, 0},
    {0, 0}};

// float deltas[NUM_STEPS][2] = {
//     {0, 0},
//     {0.01, 0},
//     {0.02, 0},
//     {0.03, 0},
//     {0.04, 0},
//     {0.05, 0},
//     {0.06, 0},
//     {0.07, 0},
//     {0.08, 0},
//     {0.09, 0},
//     {0.1, 0},
//     {0.12, 0},
//     {0.14, 0},
//     {0.16, 0},
//     {0.18, 0},
//     {0.2, 0},
//     {0.2, 0},
//     {0.2, 0},
//     {0.2, 0},
//     {0.2, 0},
//     {0.2, 0},
//     {0.2, 0},
//     {0.2, 0},
//     {0.18, 0},
//     {0.16, 0},
//     {0.16, 0},
//     {0.14, 0},
//     {0.12, 0},
//     {0.1, 0},
//     {0.09, 0},
//     {0.08, 0},
//     {0.08, 0},
//     {0.07, 0},
//     {0.06, 0},
//     {0.05, 0},
//     {0.04, 0},
//     {0.03, 0},
//     {0.02, 0},
//     {0.01, 0},
//     {0, 0}

// };

static uint8_t index = 0;
const float time = 0.377;
const uint8_t Speed_Multiplier = 1.9;

uint16_t cnt1, cnt2, tick, diff, tick1;
int32_t pulses;
uint8_t dir_flag = 0;

float velocities[NUM_STEPS][4];

float tmp_omega;

enum Dirn
{
        HALT,
        AHEAD,
        BACK
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

uint16_t time_period(uint16_t PWM_frequency);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
int ITM_SendString(char *data, int len)
{
        int i = 0;
        for (; i < len; ++i)
        {
                ITM_SendChar(data[i]);
        }
        return i;
}

int _write(int file, char *data, int len)
{
        int sent = ITM_SendString(data, len);
        return sent;
}

/* SET DUTYCYCLE function */
void set_DutyCycle(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t dutyCycle)
{
        uint16_t mapped_value;
        mapped_value = (time_period(MOTOR_DRIVER_FREQUENCY) * dutyCycle) / 65535;
        __HAL_TIM_SET_COMPARE(htim, Channel, mapped_value);
}

float float_abs(float x)
{
        if (x < 0)
        {
                return (-1 * x);
        }
        return x;
}

typedef struct
{
        uint8_t id;
        float radius;
        float wheel_velcoity;
        float omega;
        enum Dirn direction;
        GPIO_TypeDef *in1_port;
        uint16_t in1_pin;
        GPIO_TypeDef *in2_port;
        uint16_t in2_pin;
} wheel;

wheel w[4];
void wheel_init(void)
{
        int i;
        for (i = 0; i < 4; i++)
        {
                w[i].id = i;
                w[i].radius = 0.067;
        }
        w[0].in1_port = GPIOD;
        w[0].in1_pin = GPIO_PIN_1;
        w[0].in2_port = GPIOD;
        w[0].in2_pin = GPIO_PIN_3;

        w[1].in1_port = GPIOD;
        w[1].in1_pin = GPIO_PIN_5;
        w[1].in2_port = GPIOD;
        w[1].in2_pin = GPIO_PIN_14;

        w[2].in1_port = GPIOD;
        w[2].in1_pin = GPIO_PIN_7;
        w[2].in2_port = GPIOD;
        w[2].in2_pin = GPIO_PIN_15;

        w[3].in1_port = GPIOC;
        w[3].in1_pin = GPIO_PIN_12;
        w[3].in2_port = GPIOC;
        w[3].in2_pin = GPIO_PIN_10;
}

void calculate_robot_velocity()
{
        float x, y;
        x = deltas[index][0] * Speed_Multiplier;
        y = deltas[index][1] * Speed_Multiplier;
        float velocity[3] = {x / time, y / time, 0};
        int coupling_matrix[4][3] = {{-1, 1, 1}, {1, 1, 1}, {-1, 1, -1}, {1, 1, -1}}; //const vanera global banauney

        for (int i = 0; i < 4; i++)
        {
                w[i].wheel_velcoity = 0;
                for (int j = 0; j < 3; j++)
                {
                        w[i].wheel_velcoity += velocity[j] * coupling_matrix[i][j];
                }
                velocities[index][i] = w[i].wheel_velcoity;
        }
        ++index;
}

void calculate_omega()
{
        for (int i = 0; i < 4; ++i)
        {
                w[i].omega = w[i].wheel_velcoity / w[i].radius;
        }
}

void set_Direction(wheel *w)
{
        if (w->direction == AHEAD)
        {
                HAL_GPIO_WritePin(w->in1_port, w->in1_pin, SET);
                HAL_GPIO_WritePin(w->in2_port, w->in2_pin, RESET);
        }
        else if (w->direction == BACK)
        {
                HAL_GPIO_WritePin(w->in1_port, w->in1_pin, RESET);
                HAL_GPIO_WritePin(w->in2_port, w->in2_pin, SET);
        }
        else
        {
                HAL_GPIO_WritePin(w->in1_port, w->in1_pin, RESET);
                HAL_GPIO_WritePin(w->in2_port, w->in2_pin, RESET);
        }
}

void move()
{
        uint16_t dutyCycle[4];
        for (int i = 0; i < NUM_STEPS; ++i)
        {
                calculate_robot_velocity();
                calculate_omega();
                for (int j = 0; j < 4; ++j)
                {
                        tmp_omega = float_abs(w[j].omega);
                        dutyCycle[j] = 2663.115 * tmp_omega;
                        if (!w[j].omega)
                        {
                                w[j].direction = HALT;
                        }
                        else if (w[j].omega > 0)
                        {
                                w[j].direction = AHEAD;
                        }
                        else
                        {
                                w[j].direction = BACK;
                        }
                        set_Direction(&w[j]);
                }
                set_DutyCycle(&htim8, TIM_CHANNEL_1, dutyCycle[0]);
                set_DutyCycle(&htim8, TIM_CHANNEL_2, dutyCycle[1]);
                set_DutyCycle(&htim8, TIM_CHANNEL_3, dutyCycle[2]);
                set_DutyCycle(&htim8, TIM_CHANNEL_4, dutyCycle[3]);
                HAL_Delay(90);
        }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
        if (htim->Instance == TIM14)
        {
                if (w[2].direction != HALT) {
                        if ((++dir_flag) % 2)
                        {
                                w[2].direction = BACK;
                        }
                        else
                        {
                                w[2].direction = AHEAD;
                        }
                        set_Direction(&w[2]);
                }
        }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
        if (GPIO_Pin == GPIO_PIN_11)
        {
                if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9))
                {
                        ++pulses;
                }
                else
                {
                        --pulses;
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
        MX_ADC1_Init();
        MX_TIM3_Init();
        MX_TIM4_Init();
        MX_TIM5_Init();
        MX_TIM8_Init();
        MX_USART1_UART_Init();
        MX_USART3_UART_Init();
        MX_TIM14_Init();
        /* USER CODE BEGIN 2 */
        // sHAL_Delay(1000);
        HAL_TIM_Base_Start(&htim8);
        wheel_init();

        set_DutyCycle(&htim8, TIM_CHANNEL_1, 0);
        set_DutyCycle(&htim8, TIM_CHANNEL_2, 0);
        set_DutyCycle(&htim8, TIM_CHANNEL_3, 0);
        set_DutyCycle(&htim8, TIM_CHANNEL_4, 0);

        HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
        HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);

        w[2].direction = AHEAD;
        set_Direction(&w[2]);
        HAL_TIM_Base_Start_IT(&htim14);
        set_DutyCycle(&htim8, TIM_CHANNEL_3, 6000);
        printf("%d\n", 0);

        tick1 = tick;

        /* USER CODE END 2 */

        /* Infinite loop */
        /* USER CODE BEGIN WHILE */
        while (1)
        {

                /* USER CODE END WHILE */

                /* USER CODE BEGIN 3 */
                if (HAL_GetTick() - tick > 1L) {
                        tick = HAL_GetTick();
                        printf("%ld\n", pulses);
                }

                // if (HAL_GetTick() - tick1 > 4000) {
                //         break;
                // }
        }
        for (uint16_t i = 5000; i > 1000; i -= 1000)
        {

                set_DutyCycle(&htim8, TIM_CHANNEL_3, i);
                HAL_Delay(100);
        }

        w[2].direction = HALT;
        set_DutyCycle(&htim8, TIM_CHANNEL_3, 0);
        set_Direction(&w[2]);
        while (1)
                ;
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
