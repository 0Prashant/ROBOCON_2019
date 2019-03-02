/*30-1-2019*/

#include "feed_forward.h"
#include <math.h>

void ffInit(struct feedStr *ff, float _max_omega, float _percent_div, float _dt)
{
        ff->max_omega = _max_omega;
        ff->percent_div = _percent_div;
        ff->status = ffBUSY;
        ff->dt = _dt;
        ff->index = 0;
}

void ffSetAngle(struct feedStr *ff, float _max_angle)
{
        float max_angle_rad;

        if (_max_angle > 0)
        {
                ff->sign_flag = 1;
        }

        if (_max_angle < 0)
        {
                _max_angle = -1 * _max_angle;
                ff->sign_flag = -1;
        }

        //printf("%d", ff->sign_flag);
        if (_max_angle == 0)
                ff->status = ffCOMPLETE;

        max_angle_rad = (_max_angle * PI) / 180;

        ff->max_angle = _max_angle;
        ff->T = max_angle_rad / ff->max_omega; /*change*/
        ff->t1 = (ff->percent_div * ff->T) / 100.0f;
        ff->t2 = (100.0f - ff->percent_div) * ff->T / 100.0f;
}

float ffGenerate(struct feedStr *ff, float curr_angle)
{

        float new_omega = 0;
        float t = ff->index * ff->dt;

        while (ff->status == ffBUSY)
        {
                if ((t >= 0) && (t < ff->t1))
                {
                        new_omega = (ff->max_omega * t) / ff->t1;
                }

                else if ((t > ff->t1 && (t < ff->t2)))
                {
                        new_omega = ff->max_omega;
                }

                else if ((t > ff->t2) && (t < ff->T))
                {
                        new_omega = ff->max_omega * (t - ff->T) / (ff->t2 - ff->T);
                        //new_omega = ff->max_omega ;
                }

                else if (fabs(curr_angle) < fabs(ff->max_angle) && t > ff->T)
                {
                        new_omega = 3;
                }

                if (fabs(curr_angle) >= fabs(ff->max_angle))
                {
                        ff->status = ffCOMPLETE;
                        return 0;
                }
                ff->index++;
                return new_omega * ff->sign_flag;
        }
        return 0;
}

void ffRestart(struct feedStr *ff)
{
        ff->status = ffBUSY;
        ffResetParams(ff);
}

void ffResetParams(struct feedStr *ff)
{
        ff->index = 0;
}
