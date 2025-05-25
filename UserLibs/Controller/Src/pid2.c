#include "pid2.h"

#define balance_UP_KP 0.0f
#define balance_UP_KD 0.0f

int balance_UP(float Angle,float Mechanical_balance,float Gyro)
{  
   float Bias;//角度误差
   int balance;//直立环计算出来的电机控制pwm
   Bias=Angle-Mechanical_balance;                   
   //===求出平衡的角度中值和机械相关
   balance=balance_UP_KP*Bias+balance_UP_KD*Gyro;  
   //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
   return balance;
}

