#ifndef _MPU_H
#define _MPU_H

int MPU_6500_Init(void);
int MPU6500_DMP_Get_Data(float *pitch, float *roll, float *yaw, float* ax, float* ay, float* az, float* gyrox, float* gyroy, float* gyroz);
#endif
