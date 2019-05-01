#include "robo_init.h"

leg leg[1];
steering steering;

static motor_config motor_configurations[3];
static  encoder_config encoder_configurations[2];


static void motor_init();
static void encoder_init();
static void pid_init();
static void limit_switch_init(); 
static void leg_init();
static void steering_init();

void robo_init(){ 
	motor_init();
	encoder_init();	
	pid_init();
	limit_switch_init();
	leg_init();
	steering_init();
}

void leg_init(){
	leg[0].set_config(&motor_configurations[0], &motor_configurations[1], &encoder_configurations[0]);	
	leg[0].set_gravity_compensator_constant(0.85, 0.95);

}	

void steering_init(){
	steering.set_config(&motor_configurations[2], &encoder_configurations[1]);
}

static void pid_init(){

	leg[0].set_PID_constants( 10, 125, 0, -38, 38);		//for crank = 70  // 25.42, 3.082, 2.385	
	steering.set_PID_constants( 2, 4, 0, -1.35, 1.35);			//for crank = 100 // 17.8, 0.3103, 255.2	loaded = 4.5906, 0.06479, 0
}

static void limit_switch_init(){
	leg[0].limit_switch__[0].int_port = GPIOC;
	leg[0].limit_switch__[0].int_pin = GPIO_PIN_0;
	leg[0].limit_switch__[1].int_port = GPIOA;
	leg[0].limit_switch__[1].int_pin = GPIO_PIN_2;

	//steering.limit_switch__[0].int_port = GPIOD;
	//steering.limit_switch__[0].int_pin = GPIO_PIN_4;
}

void motor_init(){
        motor_configurations[0].htim = &htim8;
        motor_configurations[0].in1_port = GPIOD;
        motor_configurations[0].in1_pin = GPIO_PIN_4; 
        motor_configurations[0].in2_port = GPIOD;
        motor_configurations[0].in2_pin = GPIO_PIN_2;
        motor_configurations[0].channel = TIM_CHANNEL_3;
        motor_configurations[0].max_omega = 95/2.5;
        motor_configurations[0].tolerance = 0.2;

        motor_configurations[1].htim = &htim8;
        motor_configurations[1].in1_port = GPIOC;
        motor_configurations[1].in1_pin = GPIO_PIN_11;
        motor_configurations[1].in2_port = GPIOA;
        motor_configurations[1].in2_pin = GPIO_PIN_15;
        motor_configurations[1].channel = TIM_CHANNEL_2;
        motor_configurations[1].max_omega = 95/2.5;
        motor_configurations[1].tolerance = 0.2;
        

	motor_configurations[2].htim = &htim8;
	motor_configurations[2].in1_port = GPIOA;
	motor_configurations[2].in1_pin = GPIO_PIN_10;
	motor_configurations[2].in2_port = GPIOE;
	motor_configurations[2].in2_pin = GPIO_PIN_3;
	motor_configurations[2].channel = TIM_CHANNEL_4;
	motor_configurations[2].max_omega = 7/5.04;
	motor_configurations[2].tolerance = 0.1;

	HAL_TIM_Base_Start(&htim8);
	HAL_TIM_PWM_Start(motor_configurations[0].htim, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(motor_configurations[1].htim, TIM_CHANNEL_2);
        HAL_TIM_PWM_Start(motor_configurations[1].htim, TIM_CHANNEL_4);
}

void encoder_init(){
	encoder_configurations[0].ppr = 200*LEG_TO_ENCODER_RATIO*4;
	encoder_configurations[0].henc = &htim2;
	HAL_TIM_Encoder_Start(encoder_configurations[0].henc, TIM_CHANNEL_ALL);
	encoder_configurations[1].ppr = 200*BEARING_TO_STEERING_RATIO*4;
	encoder_configurations[1].henc = &htim3;
	HAL_TIM_Encoder_Start(encoder_configurations[1].henc, TIM_CHANNEL_ALL);
}
