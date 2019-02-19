/*22-1-2019*/

#include "robo_init.h"

struct interruptStr blnc_int_arr[4];
struct encoderStr encoder_arr[4];
struct motorStr motor_arr[4];
struct pneumaticStr pneu_arr[4];
struct interruptStr int_arr[4];
struct pidStr pid_arr[4];
struct feedStr ff_arr[4];
struct fsmStr fsm;

struct motorStr balance_motor;

// Motor 1 started working after changing interrupt pin from PE3 to PB9
// Cytron motor encoder pulse comes when pull_up only

void roboInit(void)
{
        pidInit();
        motorInit();
        encoderInit(); // Initialized after HOME_POSITION
        interruptInit();
        pneumaticInit();
        feedForwardInit();
}

void feedForwardInit(void)
{
        for (uint8_t i = 0; i < 4; i++)
        {
                ffInit(&ff_arr[i], 5, 20, dt_secs);
        }
}

void interruptInit(void)
{
        // Motor interrupts
        int_arr[0].int_pin = GPIO_PIN_9;
        int_arr[1].int_pin = GPIO_PIN_13;
        int_arr[2].int_pin = GPIO_PIN_1;
        int_arr[3].int_pin = GPIO_PIN_5;

        // Balance interrupts
        blnc_int_arr[0].int_pin = GPIO_PIN_14;
        blnc_int_arr[1].int_pin = GPIO_PIN_15;
        blnc_int_arr[2].int_pin = GPIO_PIN_7;
        blnc_int_arr[3].int_pin = GPIO_PIN_3;

        for (uint8_t i = 0; i < 4; i++)
        {
                int_arr[i].fhome_pos = 0;
                int_arr[i].fangle_reset = 0;
                int_arr[i].intState = STATE_HOME;
                blnc_int_arr[i].intState = STATE_HOME;
        }
}

void pidInit(void)
{
        for (uint8_t i = 0; i < 4; i++)
        {
                PID_Init(&pid_arr[i]);

                pid_arr[i].max_output = 17;
                pid_arr[i].min_output = -17;

                pid_arr[i].kp = 0.3;
                pid_arr[i].ki = 0.02;

                pid_arr[i].last_error = 0;
                pid_arr[i].output = 0;
                pid_arr[i].previous_error = 0;
                pid_arr[i].last_output = 0;
        }
}

void encoderInit(void)
{
        encoder_arr[0].htim = &htim3;
        encoder_arr[1].htim = &htim4;
        encoder_arr[2].htim = &htim1;
        encoder_arr[3].htim = &htim2;

        for (uint8_t i = 0; i < 4; i++)
        {
                // Gear reduction ratio = 4 and X4_MODE
                encoder_arr[i].ppr = PPR_VIET_MOTOR * 4 * 4;
                encoder_arr[i].ref_count = 0;
                encoder_arr[i].last_count = 0;
                // Start encoder-mode
                HAL_TIM_Encoder_Start(encoder_arr[i].htim, TIM_CHANNEL_ALL);
        }
                balance_motor.encoder->ppr = PPR_CYTRON_MOTOR;
                balance_motor.encoder->ref_count = 0;
                balance_motor.encoder->last_count = 0;
}

void pneumaticInit(void)
{
        pneu_arr[0].pneu_port = GPIOC;
        pneu_arr[0].pneu_pin = GPIO_PIN_1;

        pneu_arr[1].pneu_port = GPIOE;
        pneu_arr[1].pneu_pin = GPIO_PIN_8;

        pneu_arr[2].pneu_port = GPIOB;
        pneu_arr[2].pneu_pin = GPIO_PIN_2;

        pneu_arr[3].pneu_port = GPIOC;
        pneu_arr[3].pneu_pin = GPIO_PIN_3;
}
void motorInit(void)
{
        // Mass balancing motor
        balance_motor.dir1_port = GPIOA;
        balance_motor.dir2_port = GPIOA;
        balance_motor.dir1_pin = GPIO_PIN_4;
        balance_motor.dir2_pin = GPIO_PIN_3;

        balance_motor.htim = &htim5;
        balance_motor.channel = TIM_CHANNEL_3;
        HAL_TIM_Base_Start(balance_motor.htim);
        HAL_TIM_PWM_Start(balance_motor.htim, balance_motor.channel);

        // Motors for gait
        // motor1
        motor_arr[0].dir1_port = GPIOA;
        motor_arr[0].dir1_pin = GPIO_PIN_15;
        motor_arr[0].dir2_port = GPIOA;
        motor_arr[0].dir2_pin = GPIO_PIN_8;
        motor_arr[0].channel = TIM_CHANNEL_1;

        // motor2
        motor_arr[1].dir1_port = GPIOC;
        motor_arr[1].dir1_pin = GPIO_PIN_11;
        motor_arr[1].dir2_port = GPIOD;
        motor_arr[1].dir2_pin = GPIO_PIN_3;
        motor_arr[1].channel = TIM_CHANNEL_2;

        // motor3
        motor_arr[2].dir1_port = GPIOD;
        motor_arr[2].dir1_pin = GPIO_PIN_5;
        motor_arr[2].dir2_port = GPIOD;
        motor_arr[2].dir2_pin = GPIO_PIN_4;
        motor_arr[2].channel = TIM_CHANNEL_3;

        // motor4
        motor_arr[3].dir1_port = GPIOD;
        motor_arr[3].dir1_pin = GPIO_PIN_7;
        motor_arr[3].dir2_port = GPIOB;
        motor_arr[3].dir2_pin = GPIO_PIN_5;
        motor_arr[3].channel = TIM_CHANNEL_4;

        for (uint8_t i = 0; i < 4; i++)
        {
                motor_arr[i].update_omega = 0;
                motor_arr[i].htim = &htim8;
                motor_arr[i].encoder = &encoder_arr[i];

                HAL_TIM_Base_Start(motor_arr[i].htim);
                HAL_TIM_PWM_Start(motor_arr[i].htim, motor_arr[i].channel);
        }
}
void fsmInit(void)
{
        fsm.loop_cnt = 0;
}

   /* /Limit-switches/
    *
    * A = 14  M1 = 3
    * B = 15  M2 = 13
    * C = 7   M3 = 1
    * D = 9   M4 = 5
    *
    * /Encoders/
    *
    * M1 = A6 A7
    * M2 = D12 D13
    * M3 = E9 E11
    * M4 = A1 A5 
    * blnc = D9    
    *
    */
