#include "robot/play.h"
#include "utils/pid_algorithms.h"
#include "utils/pid.h"
#include <math.h>

const float max_omega[2] = { 8.5, 7};

void update(float set_points[4])
{
        // Only update to new value if the buffer actually contains data else
        // keep the previous data.
        if (!Thinker_isEmpty()) {
                get_Thinker(set_points);
                for (uint8_t i = 0; i < 4; ++i) {
                        printf("%ld  ", (int32_t)(set_points[i]*1000));
                }
                printf("\n");
        }
}

/* *** Tasks To Be Done By tune function ***
 TODO 1: Measure omega of all wheels
 TODO 2: Compute error
 TODO 3: Compute PID using available PID algorithm
 TODO 4: set Omega of each wheel
 TODO 5: Repeat from step 1
 */

Discrete_PID gDisc_PID[4];
PID gPID[4];

float gI_Factor = 20;
float gP_Factor = 1;

void pid_Init()
{
        gDisc_PID[0].set_PID(1.70005, gI_Factor*1.2808, 0);
        gDisc_PID[0].set_Limits(12, -12);
        gDisc_PID[1].set_PID(1.6575, gI_Factor*0.78623, 0);
        gDisc_PID[1].set_Limits(12, -12);
        gDisc_PID[2].set_PID(0.7336, gI_Factor*10.05, 0);
        gDisc_PID[2].set_Limits(24, -24);
        gDisc_PID[3].set_PID(0.49, gI_Factor*7.615, 0);
        gDisc_PID[3].set_Limits(24, -24);

        gPID[0].set_Algorithm(&gDisc_PID[0]);
        gPID[1].set_Algorithm(&gDisc_PID[1]);
        gPID[2].set_Algorithm(&gDisc_PID[2]);
        gPID[3].set_Algorithm(&gDisc_PID[3]);
}

void tune(float set_points[4], uint32_t dt_millis)
{
        float omega[4];
        float error[4];
        float voltage[4];
        float new_omega[4];

        for (uint8_t i = 0; i < 2; ++i) {
                omega[i] = gWheels[i].get_Omega(dt_millis);
                error[i] = set_points[i] - omega[i];
                voltage[i] = gPID[i].compute_PID(error[i], dt_millis);
                new_omega[i] = voltage[i] * max_omega[i] / 12.0;
                gWheels[i].set_Omega(new_omega[i]);
                printf("(%ld, %ld)  ", (int32_t)(new_omega[i]*1000), (int32_t)(omega[i]*1000));
        }
        
        // omega[0] = gWheels[0].get_Omega(dt_millis);
        // error[0] = set_points[0] - omega[0];
        // voltage[0] = gPID[0].compute_PID(error[0], dt_millis);
        // new_omega[0] = voltage[0] * MAX_OMEGA / 24.0;
        // gWheels[0].set_Omega(new_omega[0]);
        // printf("(%ld, %ld)  ", (int32_t)(new_omega[0]), (int32_t)(omega[0]));
        printf("\n");

        // gWheels[0].update();
        for (uint8_t i = 0; i < 4; ++i) {
                gWheels[i].update();
        }
}

void ramp_down(uint32_t dt_millis)
{
        float omega[4];
        float error[4];
        float voltage[4];
        float new_omega[4];
        for (uint8_t j = 0; j < 10; ++j) {
                
                for (uint8_t i = 0; i < 4; ++i) {
                        omega[i] = gWheels[i].get_Omega(dt_millis);
                        if (fabsf(omega[i]) < 5) {
                                gWheels[i].set_Omega(0);
                        }else {
                                error[i] = -(omega[i] / 2.0);
                                voltage[i] = gPID[i].compute_PID(error[i], dt_millis);
                                new_omega[i] = voltage[i] * MAX_OMEGA / 12.0;
                                gWheels[i].set_Omega(new_omega[i]);
                        }
                }

                for (uint8_t i = 0; i < 4; ++i) {
                        gWheels[i].update();
                }
                HAL_Delay(10);
        }
}