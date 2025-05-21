#ifndef _IMU_H_
#define _IMU_H_

#include "main.h"
#include "MPU6500.h"

typedef struct Imu Imu;

struct Imu{
	uint8_t init_result;
	float pitch;
	float roll;
	float yaw;
	
	int (*Enable)(Imu* self);
	void (*Get_Data)(Imu* self);
};

Imu newImu(void);
int Enable(Imu* self);
void Get_Data(Imu* self);
#endif
