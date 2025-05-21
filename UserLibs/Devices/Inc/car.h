#ifndef _CAR_H_
#define _CAR_H_

#include "main.h"
#include "motor.h"
#include "encoder.h"
#include "imu.h"
#include "oled.h"
#include "pid.h"

#define MAX_OUT  150
#define MAX_IOUT 80

typedef struct Car Car;

struct Car{
	Motor motor_l;
	Motor motor_r;
	Encoder encoder_l;
	Encoder encoder_r;
	Imu imu;
	
	pid_t pid_a;
	pid_t pid_l;
	pid_t pid_r;
	
	void (*CarMove)(Car* self, int8_t setSpeed);
};

Car newCar(void);
void CarMove(Car* self, int8_t setSpeed);
#endif
