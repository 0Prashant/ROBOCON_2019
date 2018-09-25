#ifndef DATA
#define DATA
#include<iostream>
int robotx = 600;
int roboty = 600;

class Checkpnt
{
private:
    int x, y;

public:
    void Set_Checkpoint_x(int value)
    {
        x = value;
    }
    void Set_Checkpoint_y(int value)
    {
        y = value;
    }
    int Get_Checkpoint_x()
            {
                return x;
            }
    int Get_Checkpoint_y()
            {
                return y;
            }
};
Checkpnt checkpoint[10];

void border(void)
{
    std::cout<<"*******************************************************************************";
}
#endif
