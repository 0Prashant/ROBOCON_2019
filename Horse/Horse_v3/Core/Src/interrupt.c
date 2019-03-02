/*1-27-2019*/

#include "interrupt.h"
#include "robo_init.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
        if(GPIO_PIN == blnc_motor_chAPin)
        {
                if(HAL_GPIO_ReadPin(blnc_motor_chBPort, blnc_motor_chBPin) == 0)
                {
                        balance_motor.encoder->count--;
                }
                else
                {
                        balance_motor.encoder->count++;
                }
        }
//         if (GPIO_PIN == int_arr[0].int_pin)
//         {
//                 printf("Motor_1 \n");
//                 if (int_arr[0].intState == STATE_B)
//                 {
//                         int_arr[0].intState = STATE_A;
//                 }
//                 else if (int_arr[0].intState == STATE_HOME) {
//                         int_arr[0].fhome_pos = 1;
//                 }
//         }

//         if (GPIO_PIN == int_arr[1].int_pin)
//         {
//                 printf("Motor_2 \n");
//                 if (int_arr[1].intState == STATE_B)
//                 {
//                         int_arr[1].intState = STATE_A;
//                 }
//                 else if (int_arr[1].intState == STATE_HOME) {
//                         int_arr[1].fhome_pos = 1;
//                 }
//         }

//         if (GPIO_PIN == int_arr[2].int_pin)
//         {
//                 printf("Motor_3 \n");
//                 if (int_arr[2].intState == STATE_B)
//                 {
//                         int_arr[2].intState = STATE_A;
//                 }
//                 else if (int_arr[2].intState == STATE_HOME) {
//                         int_arr[2].fhome_pos = 1;
//                 }
//         }

//         if (GPIO_PIN == int_arr[3].int_pin)
//         {
//                 printf("Motor_4 \n");
//                 if (int_arr[3].intState == STATE_B)
//                 {
//                         int_arr[3].intState = STATE_A;
//                 }
//                 else if (int_arr[3].intState == STATE_HOME) {
//                         int_arr[3].fhome_pos = 1;
//                 }
//         }

//     if (GPIO_PIN == blnc_int_arr[0].int_pin)
//     {
//         if (blnc_int_arr[0].intState == STATE_B)
//         {
//                 int_arr[0].intState = STATE_A;
//         }    
//         printf("A\n"); 
//     }
    
//     if (GPIO_PIN == blnc_int_arr[1].int_pin)
//     {   
//         if (blnc_int_arr[1].intState == STATE_B)
//         {
//                 int_arr[1].intState = STATE_A;
//         }
//         printf("B\n");
//     }
    
//     if (GPIO_PIN == blnc_int_arr[2].int_pin)
//     {   
//         if (blnc_int_arr[2].intState == STATE_B)
//         {
//                 int_arr[2].intState = STATE_A;
//         }    
//         blnc_int_arr[2].fhome_pos = 1;
//         printf("C\n");
//     }
    
//     if (GPIO_PIN == blnc_int_arr[3].int_pin)
//     {
//         if (blnc_int_arr[3].intState == STATE_B)
//         {
//                 int_arr[3].intState = STATE_A;
//         }    
//         //printf("D\n");
//     }
     
}
