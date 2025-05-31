#include "pid2.h"
#include "car.h"

#define V_Kp -0.6f

float Med_Angle=MECHANICAL_BALANCE_BIAS;      // 机械中值，能使得小车真正平衡住的角度 
float Target_Speed=0;	  // 期望速度。---二次开发接口，用于控制小车前进后退及其速度。
float 
  Vertical_Kp = 5000.0f * 0.6,//5000.0f,
  Vertical_Kd = 1000.0f * 0.6;//190.0f;      // 直立环Kp、Kd
float 
  Velocity_Kp= V_Kp,
  Velocity_Ki= V_Kp/200.0f;   // 速度环Kp、Ki（正反馈）
float 
  Turn_Kp=-1.0f;

int Vertical(float Med,float Angle,float gyro_Y) 
{
  int PWM_out;
  
  PWM_out = Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
  
  return PWM_out;
} 

/*****************  
速度环PI控制器：Kp*Ek+Ki*Ek_S(Ek_S：偏差的积分)
******************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
  // 定义成静态变量，保存在静态存储器，使得变量不丢掉
  static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;
  float a=0.7;
  
  // 1.计算速度偏差
  // 舍去误差--我的理解：能够让速度为"0"的角度，就是机械中值。
  Encoder_Err = ((encoder_left+encoder_right) / 2.0f - Target);
  // 2.对速度偏差进行低通滤波
  // low_out = (1-a)*Ek+a*low_out_last
  EnC_Err_Lowout = (1-a)*Encoder_Err + a*EnC_Err_Lowout_last; // 使得波形更加平滑，滤除高频干扰，放置速度突变
  EnC_Err_Lowout_last = EnC_Err_Lowout;   // 防止速度过大影响直立环的正常工作
  // 3.对速度偏差积分出位移
  Encoder_S+=EnC_Err_Lowout;
  // 4.积分限幅
  Encoder_S=Encoder_S>15000?15000:(Encoder_S<(-15000)?(-15000):Encoder_S);
  
  // 5.速度环控制输出
  PWM_out = Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
  
  return PWM_out;
}

/*****************  
转向环：系数*Z轴角速度
******************/
int Turn(int gyro_Z)
{
  int PWM_out;
  
  PWM_out = Turn_Kp*gyro_Z;
  
  return PWM_out;
}