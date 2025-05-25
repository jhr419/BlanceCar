#ifndef _CAR_H_
#define _CAR_H_

#include "main.h"
#include "motor.h"
#include "encoder.h"
#include "imu.h"
#include "oled.h"
#include "pid.h"
#include "filter.h"

#define MECHANICAL_BALANCE_BIAS -1.5f

#define V_MAX_OUT  100.0f
#define V_MAX_IOUT 0.0f 

#define A_MAX_OUT  33000.0f
#define A_MAX_IOUT 12000.0f  
                              

typedef struct Car Car;

struct Car{
	int8_t set_v;
	uint8_t isBrake;
	
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
