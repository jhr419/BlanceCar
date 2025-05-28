#ifndef _PID2_H_
#define _PID2_H_

#include "main.h"

int Vertical(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z);

#endif
