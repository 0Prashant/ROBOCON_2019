/*27-1-2019*/

#include "check.h"

int checkAngle(int motor_index)
{
    int temp_check_variable = 1;
    for (uint8_t i = 0; i < 4; i++)
    {
        if (i != motor_index)
        {
            if (ff_arr[i].status == ffCOMPLETE)
            {
                temp_check_variable = temp_check_variable * 10 + 1;
            }
        }

        else
        {
            if (int_arr[i].intState == STATE_A)
            {
                temp_check_variable = temp_check_variable * 10 + 1;
                motor_arr[i].update_omega = 0;
            }
                 
        }
    }

    if (temp_check_variable == 11111)
        return 1;

    else
        return 0;
}

int checkHomePosition(void)
{
    uint8_t check_variable = 0;

    for (uint8_t i = 0; i < 4; i++)
    {
        check_variable += int_arr[i].fhome_pos;
    }

    check_variable += blnc_int_arr[2].fhome_pos;
    if (check_variable == 4)
        return 1;
    else
        return 0;
}
