#include "car.h"

#define MOTOR_TIM 				htim2
#define MOTOR_L_CHANNEL 	TIM_CHANNEL_1
#define MOTOR_L_IN1_PORT 	DIN1_GPIO_Port
#define MOTOR_L_IN1_PIN 	DIN1_Pin
#define MOTOR_L_IN2_PORT 	DIN2_GPIO_Port
#define MOTOR_L_IN2_PIN 	DIN2_Pin

#define MOTOR_R_CHANNEL 	TIM_CHANNEL_2
#define MOTOR_R_IN1_PORT 	AIN1_GPIO_Port
#define MOTOR_R_IN1_PIN 	AIN1_Pin
#define MOTOR_R_IN2_PORT 	AIN2_GPIO_Port
#define MOTOR_R_IN2_PIN 	AIN2_Pin

#define ENCODER_L_TIM htim3
#define ENCODER_R_TIM htim1

#define PID_A_P	0.5f
#define PID_A_I	1.0f
#define PID_A_D	0.0f

#define PID_L_P	1.0f
#define PID_L_I	0.0f
#define PID_L_D	0.0f
#define PID_R_P	1.0f
#define PID_R_I	0.0f
#define PID_R_D	0.0f

Car car;

Car newCar(void){
	Car c;
	
	Motor_InitTypeDef motor_l_Init = {
		.htim 				= &MOTOR_TIM,
		.Channel 			= MOTOR_L_CHANNEL,
		.IN1_GPIOx 		= MOTOR_L_IN1_PORT,
		.IN1_GPIO_Pin = MOTOR_L_IN1_PIN,
		.IN2_GPIOx 		= MOTOR_L_IN2_PORT,
		.IN2_GPIO_Pin = MOTOR_L_IN2_PIN
	};
	
	Motor_InitTypeDef motor_r_Init = {
		.htim 				= &MOTOR_TIM,
		.Channel 			= MOTOR_R_CHANNEL,
		.IN1_GPIOx 		= MOTOR_R_IN1_PORT,
		.IN1_GPIO_Pin = MOTOR_R_IN1_PIN,
		.IN2_GPIOx 		= MOTOR_R_IN2_PORT,
		.IN2_GPIO_Pin = MOTOR_R_IN2_PIN
	};
	
	c.motor_l = newMotor(motor_l_Init);
	c.motor_r = newMotor(motor_r_Init);
	
	c.encoder_l = newEncoder(&ENCODER_L_TIM, TIM_CHANNEL_ALL);
	c.encoder_r = newEncoder(&ENCODER_R_TIM, TIM_CHANNEL_ALL);

	c.imu = newImu();
	c.imu.Enable(&c.imu);
	
	float pid_a[3] = {PID_A_P, PID_A_I, PID_A_D};
	float pid_l[3] = {PID_L_P, PID_L_I, PID_L_D};
	float pid_r[3] = {PID_R_P, PID_R_I, PID_R_D};
	
	PID_init(&c.pid_a, PID_POSITION, pid_a, MAX_OUT, MAX_IOUT);
	PID_init(&c.pid_l, PID_POSITION, pid_l, MAX_OUT, MAX_IOUT);
	PID_init(&c.pid_r, PID_POSITION, pid_r, MAX_OUT, MAX_IOUT);
	
	HAL_GPIO_WritePin(STBY_GPIO_Port,	STBY_Pin, GPIO_PIN_SET);
	
	c.CarMove = CarMove;
	
	return c;
}

void MotorPidCalc(Car* self, int8_t setSpeed_l, int8_t setSpeed_r){
	PID_calc(&self->pid_a, self->imu.roll, 0);
	PID_calc(&self->pid_l, (float)self->encoder_l.rpm, __fabs(self->pid_a.out));
	PID_calc(&self->pid_r, (float)self->encoder_r.rpm, __fabs(self->pid_a.out));	
}

void CarMove(Car* self, int8_t setSpeed){
//	uint8_t dircation = (setSpeed > 0) ? FORWARD : ((setSpeed < 0) ? BACKWARD : 0);
	uint8_t dircation = (self->pid_a.out > 0) ? FORWARD : ((self->pid_a.out < 0) ? BACKWARD : 0);

	setSpeed = __fabs(setSpeed);
	MotorPidCalc(self, setSpeed, setSpeed);
	
	self->motor_l.Move(&self->motor_l, dircation, self->pid_l.out);
	self->motor_r.Move(&self->motor_r, dircation, self->pid_r.out);
}
