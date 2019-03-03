/**author:Aasman Bashyal
* !code for robowar robot
*email:074bex402.aasman@pcampus.edu.np
*/
#include <math.h>
#define max_Velocity 240

float ramping_Factor = 1;
float velocity_Robot[3] ={0,0,0};
float velocity_Motor[4]={0,0,0,0};
float next_Velocity_Motor[4]={0,0,0,0};

float coupling_Matrix[4][3] = { { 0.75 ,  0.75,  0.75},
                                { 0.7 , -0.7,  0.7},
                                {-1 , -1,  1},
                                {-0.7 ,  0.7,  0.7}};

const uint8_t pwm_pin[4] = {44,46,45,6};
const uint8_t m_Ch[4][2] ={{ A14 , A15},
                           { A10 , A11},
                           { A13 , A12},
                           { A8 , A9}};

void setup()
{
   Serial.begin(9600);
   Serial3.begin(9600);
   Serial2.begin(9600);
   for(int i=0;i<4;i++)
   {
       pinMode(pwm_pin[i],OUTPUT);
       for(int j=0;j<2;j++)
       {
           pinMode(m_Ch[i][j],OUTPUT);
           digitalWrite(m_Ch[i][j], LOW);
       }
   }
}

void loop()
{
   input_From_bluetooth();
   calc_Robot_velocity();
   update_Robot_velocity();
}

void input_From_bluetooth()
{
   char data;
   if(Serial3.available())
   {
      data = Serial3.read();
      Serial.print(data);
      if(data == 'F')
      {
         velocity_Robot[0] = 0;
         velocity_Robot[1] = max_Velocity;
         velocity_Robot[2] = 0;
      }
       else if(data == 'B')
      {
         velocity_Robot[0] = 0;
         velocity_Robot[1] = -max_Velocity;
         velocity_Robot[2] = 0;
      }
       else if(data == 'L')
      {
         velocity_Robot[0] = -max_Velocity;
         velocity_Robot[1] = 0;
         velocity_Robot[2] = 0;
      }
       else if(data == 'R')
      {
         velocity_Robot[0] = max_Velocity;
         velocity_Robot[1] = 0;
         velocity_Robot[2] = 0;
      }
       else if(data == 'G')
      {
         velocity_Robot[0] = max_Velocity/2;
         velocity_Robot[1] = max_Velocity/2;
         velocity_Robot[2] = 0;
      }
       else if(data == 'I')
      {
         velocity_Robot[0] = max_Velocity/2;
         velocity_Robot[1] = max_Velocity/2;
         velocity_Robot[2] = 0;
      }
       else if(data == 'H')
      {
         velocity_Robot[0] = 0;
         velocity_Robot[1] = 0;
         velocity_Robot[2] = max_Velocity;
      }
       else if(data == 'J')
      {
         velocity_Robot[0] = 0;
         velocity_Robot[1] = 0;
         velocity_Robot[2] = -max_Velocity;
      }
       else if(data == 'S')
      {
         velocity_Robot[0] = 0;
         velocity_Robot[1] = 0;
         velocity_Robot[2] = 0;
      }
      else if(data == 'X')
      {
        //home
        Serial2.write("h");
      }
      else if(data == 'x')
      {
        //home
      }
      else if(data == 'W')
      {
        //tip
         Serial2.write("j");
      }
      else if(data == 'u')
      {
        //tip
      }
      else if(data == 'U')
      {
        //faal
         Serial2.write("1");
      }
      else if(data == 'w')
      {
        //faal
      }
   }
}

/*void calc_Robot_velocity()
{
   for(int i=0 ;i<4 ;i++)
   {
      velocity_Motor[i] = 0;
      for(int j=0 ;j<3 ;j++)
      {
         velocity_Motor[i] += coupling_Matrix[i][j] * velocity_Robot[j];
      }
      if(velocity_Motor[i] > max_Velocity)
      {
        velocity_Motor[i] = max_Velocity;
      }
      if(velocity_Motor[i] < -max_Velocity)
      {
        velocity_Motor[i] = -max_Velocity;
      }
      ramp(i);
   }
}*/
void calc_Robot_velocity()
{
   for(int i=0 ;i<4 ;i++)
   {
      next_Velocity_Motor[i] = 0;
      for(int j=0 ;j<3 ;j++)
      {
         next_Velocity_Motor[i] += coupling_Matrix[i][j] * velocity_Robot[j];
      }
      if(next_Velocity_Motor[i] > max_Velocity)
      {
        next_Velocity_Motor[i] = max_Velocity;
      }
      if(next_Velocity_Motor[i] < -max_Velocity)
      {
        next_Velocity_Motor[i] = -max_Velocity;
      }
      ramp(i);
   }
}

void ramp(int i)
{
  if(velocity_Motor[i] != next_Velocity_Motor[i])
  {
    if((velocity_Motor[i]+ramping_Factor) < next_Velocity_Motor[i])
      velocity_Motor[i] += ramping_Factor;
    else if((velocity_Motor[i]-ramping_Factor) > next_Velocity_Motor[i])
      velocity_Motor[i] -= ramping_Factor;
    else
      velocity_Motor[i] = next_Velocity_Motor[i];  
  }
}

void update_Robot_velocity()
{
   for(int i=0 ;i<4 ;i++)
   {
      if(velocity_Motor[i] > 0)
      {
         digitalWrite(m_Ch[i][0],HIGH);
         digitalWrite(m_Ch[i][1],LOW);
      }
      if(velocity_Motor[i] < 0)
      {
         digitalWrite(m_Ch[i][0],LOW);
         digitalWrite(m_Ch[i][1],HIGH);
      }
      if(velocity_Motor[i] == 0)
      {
         digitalWrite(m_Ch[i][0],HIGH);
         digitalWrite(m_Ch[i][1],HIGH);
      }
      analogWrite(pwm_pin[i],fabs(velocity_Motor[i]));
   }
}
