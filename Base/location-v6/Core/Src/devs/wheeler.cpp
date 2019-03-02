#include "devs/wheeler.h"
#include <math.h>
#include "utils/queue.h"

#define WHEEL_RADIUS            (67.5)
#define MAX_POSSIBLE_OMEGA      (40)
#define RAMPING_FACTOR (20)
const float gSpeed_Multiplier = 1;

float gWheel_Velocities[4];
float gWheel_Omegas[4];
const int gCoupling_Matrix[4][3] = {{ 1,  1, 1},
                                    { 1, -1, 1},
                                    {-1, -1, 1},
                                    {-1,  1, 1}};
                                    

#define NUM_WHEELER_BYTES       (8)
#define THETA_NOT               (1.570796)

static uint8_t gStart_Byte = 0xA5;
static bool gWheeler_Start_Byte_Sent = false;
static bool is_first_ramping = false;
static Queue<int16_t, 2> gWheeler_Data[NUM_WHEELER_BYTES / 2];
uint8_t gWheeler_Bytes[NUM_WHEELER_BYTES];
struct Wheel Wheel_arr[4];

void wheel_init()
{
        for (int i = 0; i < 4; i++)
        {
                Wheel_arr[i].id = i;
                Wheel_arr[i].velocity = 0.000000;
		Wheel_arr[i].next_velocity = 0.000000;
        }
}

void Wheeler_Init()
{
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_TC);
}

static float calculate_Omega(float velocity, float radius)
{
        return (velocity) / (radius);
}

void calc_WheelsOmegas(float omegas[4], float v, float theta, float phi, uint32_t dt_millis)
{
        float vel[4];
        theta += THETA_NOT;
        float velocity[3] = {v * cos(theta), v * sin(theta), -3*phi * 1000 / (float)(dt_millis)};

        for (int i = 0; i < 4; ++i) {
                vel[i] = 0;
                for (int j = 0; j < 3; ++j) {
                        vel[i] += velocity[j] * gCoupling_Matrix[i][j];
                }
                omegas[i] = calculate_Omega(vel[i], WHEEL_RADIUS);
        }

}

// void calculate_robot_velocity(float v, float theta, float phi, uint32_t dt_millis)
// {
        // float velocity[3] = {v * cos(theta) * cos(phi), v * sin(theta) * cos(phi), -phi * 1000 / (float)(dt_millis)};

        // for (int i = 0; i < 4; ++i) {
        //         gWheel_Velocities[i] = 0;
        //         for (int j = 0; j < 3; ++j) {
        //                 gWheel_Velocities[i] += velocity[j] * gCoupling_Matrix[i][j];
        //         }
        //         gWheel_Omegas[i] = calculate_Omega(gWheel_Velocities[i]);
        // }
// }
void calculate_robot_velocity(float omegas_[4], float velocity[3])
{
        for (int i = 0; i < 4; i++)
        {
                Wheel_arr[i].next_velocity = 0;
                for (int j = 0; j < 3; j++)
                {
                        Wheel_arr[i].next_velocity += velocity[j] * gCoupling_Matrix[i][j];
                }
			
		if(Wheel_arr[i].next_velocity >ROBOT_VELOCITY)
		{
			for(int j=0;j<4;j++)
				Wheel_arr[j].next_velocity = (ROBOT_VELOCITY * Wheel_arr[j].next_velocity)/ Wheel_arr[i].next_velocity;
		}
		if(Wheel_arr[i].next_velocity < -ROBOT_VELOCITY)
		{
			for(int j=0;j<4;j++)
				Wheel_arr[j].next_velocity = -((ROBOT_VELOCITY * Wheel_arr[j].next_velocity)/ Wheel_arr[i].next_velocity);					
		}
                if(!is_first_ramping)
                {
                        omegas_[i] = initial_ramp(Wheel_arr[i].id);
                }
		else
                {
                        omegas_[i] = ramp(Wheel_arr[i].id);
                }
        }
}

float initial_ramp(uint8_t wheel_no) 
{
	if(Wheel_arr[wheel_no].velocity != Wheel_arr[wheel_no].next_velocity)
	{
		if( (Wheel_arr[wheel_no].velocity + (RAMPING_FACTOR/3)) < Wheel_arr[wheel_no].next_velocity)
			Wheel_arr[wheel_no].velocity += (RAMPING_FACTOR/3);
		else if( (Wheel_arr[wheel_no].velocity - (RAMPING_FACTOR/3)) > Wheel_arr[wheel_no].next_velocity)
			Wheel_arr[wheel_no].velocity -= (RAMPING_FACTOR/3);
		else
		{
			Wheel_arr[wheel_no].velocity= Wheel_arr[wheel_no].next_velocity;
                        is_first_ramping = true;
		}        
	}	
        return ((Wheel_arr[wheel_no].velocity*MAX_OMEGA)/MAX_VELOCITY);
}

float ramp(uint8_t wheel_no) 
{
	if(Wheel_arr[wheel_no].velocity != Wheel_arr[wheel_no].next_velocity)
	{
		if( (Wheel_arr[wheel_no].velocity + RAMPING_FACTOR) < Wheel_arr[wheel_no].next_velocity)
			Wheel_arr[wheel_no].velocity += RAMPING_FACTOR;
		else if( (Wheel_arr[wheel_no].velocity - RAMPING_FACTOR) > Wheel_arr[wheel_no].next_velocity)
			Wheel_arr[wheel_no].velocity -= RAMPING_FACTOR;
		else
		{
			Wheel_arr[wheel_no].velocity= Wheel_arr[wheel_no].next_velocity;
		}        
	}	
        return ((Wheel_arr[wheel_no].velocity*MAX_OMEGA)/MAX_VELOCITY);
}

void send_WheelerPack(float pack[NUM_WHEELER_BYTES / 2])
{
        int16_t data;
        // Data are omegas of four motors
        for (uint8_t i = 0; i < NUM_WHEELER_BYTES / 2; ++i) {
                data = (int16_t)((pack[i] * 32768.0) / (MAX_POSSIBLE_OMEGA));
                gWheeler_Data[i].insert(data);
        }
        if (!gWheeler_Start_Byte_Sent) {
                gWheeler_Start_Byte_Sent = true;
                HAL_UART_Transmit_IT(&huart2, &gStart_Byte, 1);
        }
}

void Wheeler_UART2_TxCpltCallback()
{
        if (gWheeler_Start_Byte_Sent) {
                gWheeler_Start_Byte_Sent = false;
                int16_t data;
                for (uint8_t i = 0; i < NUM_WHEELER_BYTES / 2; ++i) {
                        data = gWheeler_Data[i].lookup();
                        gWheeler_Bytes[2*i + 0] = (uint8_t)(data >> 8);
                        gWheeler_Bytes[2*i + 1] = (uint8_t)(data);
                }
                HAL_UART_Transmit_IT(&huart2, gWheeler_Bytes, NUM_WHEELER_BYTES);
        }
        else {
                // All Wheeler buffers contain equal length of data
                if (gWheeler_Data[0].length()) {
                        gWheeler_Start_Byte_Sent = true;
                        HAL_UART_Transmit_IT(&huart2, &gStart_Byte, 1);
                }
        }
}
