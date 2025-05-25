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
												//角度-速度串行
#define PID_A_P 5000.0f  //200.0f
#define PID_A_I	0.0f    //1.0f
#define PID_A_D	0.0f    //0.0f
                        
#define PID_L_P	2200.0f //2200.0f
#define PID_L_I	50.0f   //50.0f  
#define PID_L_D	10.0f   //10.0f  
                        
#define PID_R_P	2200.0f //2200.0f
#define PID_R_I	50.0f   //50.0f  
#define PID_R_D	10.0f   //10.0f  

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
	
	PID_init(&c.pid_a, PID_POSITION, pid_a, A_MAX_OUT, A_MAX_IOUT);
	PID_init(&c.pid_l, PID_POSITION, pid_l, V_MAX_OUT, V_MAX_IOUT);
	PID_init(&c.pid_r, PID_POSITION, pid_r, V_MAX_OUT, V_MAX_IOUT);
	
	HAL_GPIO_WritePin(STBY_GPIO_Port,	STBY_Pin, GPIO_PIN_SET);
	
	c.CarMove = CarMove;
	
	return c;
}

void MotorPidCalc(Car* self, int8_t setSpeed_l, int8_t setSpeed_r){
	static int8_t symbol=0;
	PID_calc(&self->pid_a, self->imu.roll, MECHANICAL_BALANCE_BIAS);
	if(self->pid_a.error[0] > 0) symbol = 1;
	else if (self->pid_a.error[0] < 0) symbol = -1;
	if(__fabs(self->imu.roll - MECHANICAL_BALANCE_BIAS)<0.5){
		PID_calc(&self->pid_l, self->encoder_l.rpm, A_MAX_IOUT);
		PID_calc(&self->pid_r, self->encoder_r.rpm, A_MAX_IOUT);	
	}else
	{
//		PID_calc(&self->pid_l, self->encoder_l.rpm, 20); //测试速度环
//		PID_calc(&self->pid_r, self->encoder_r.rpm, 20);
		PID_calc(&self->pid_l, self->encoder_l.rpm, self->pid_a.out);
		PID_calc(&self->pid_r, self->encoder_r.rpm, self->pid_a.out);
	}
}

void CarMove(Car* self, int8_t setSpeed){
//	uint8_t dircation = (self->imu.roll > MECHANICAL_BALANCE_BIAS) ? BACKWARD : ((self->imu.roll < MECHANICAL_BALANCE_BIAS) ? FORWARD : 0);
	float set_rpm_l;
	float set_rpm_r;

	setSpeed = __fabs(setSpeed);
	MotorPidCalc(self, setSpeed, setSpeed);
	
//	set_rpm_l = self->pid_l.out;
//	set_rpm_r = self->pid_r.out;
	set_rpm_l = self->pid_a.out;
	set_rpm_r = self->pid_a.out;
	
//	if(__fabs(self->imu.roll - 0.0)<0.5){
//		set_rpm_l = 20;
//		set_rpm_r = 20;
//	}
	if(car.imu.roll > 70 || car.imu.roll < -70){
		car.isBrake = 1;
	}else car.isBrake = 0;
	
	self->motor_l.Move(&self->motor_l, car.isBrake, (int32_t)set_rpm_l);
	self->motor_r.Move(&self->motor_r, car.isBrake, (int32_t)set_rpm_r);
}
