#include "car.h"

// 定义左/右电机和编码器相关的硬件配置（定时器、GPIO端口与引脚）
// Define hardware configuration for left/right motors and encoders
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

// 定义平衡角度 PID 和速度 PID 的参数
// Define PID parameters for angle and velocity control
#define PID_A_P 3400.0f
#define PID_A_I	0.0f
#define PID_A_D	10.0f

#define PID_L_P	2800.0f
#define PID_L_I	50.0f
#define PID_L_D	0.0f

#define PID_R_P	2800.0f
#define PID_R_I	50.0f
#define PID_R_D	0.0f

// 定义全局小车对象
// Global car instance
Car car;

// 创建小车对象，初始化电机、编码器、IMU 和 PID 控制器
// Create a car instance and initialize motors, encoders, IMU, and PID controllers
Car newCar(void){
	Car c;

	// 初始化左电机配置
	// Initialize left motor
	Motor_InitTypeDef motor_l_Init = {
		.htim 				= &MOTOR_TIM,
		.Channel 			= MOTOR_L_CHANNEL,
		.IN1_GPIOx 		= MOTOR_L_IN1_PORT,
		.IN1_GPIO_Pin = MOTOR_L_IN1_PIN,
		.IN2_GPIOx 		= MOTOR_L_IN2_PORT,
		.IN2_GPIO_Pin = MOTOR_L_IN2_PIN
	};

	// 初始化右电机配置
	// Initialize right motor
	Motor_InitTypeDef motor_r_Init = {
		.htim 				= &MOTOR_TIM,
		.Channel 			= MOTOR_R_CHANNEL,
		.IN1_GPIOx 		= MOTOR_R_IN1_PORT,
		.IN1_GPIO_Pin = MOTOR_R_IN1_PIN,
		.IN2_GPIOx 		= MOTOR_R_IN2_PORT,
		.IN2_GPIO_Pin = MOTOR_R_IN2_PIN
	};

	// 创建左右电机对象
	// Create motor instances
	c.motor_l = newMotor(motor_l_Init);
	c.motor_r = newMotor(motor_r_Init);

	// 初始化左右编码器
	// Initialize encoders
	c.encoder_l = newEncoder(&ENCODER_L_TIM, TIM_CHANNEL_ALL);
	c.encoder_r = newEncoder(&ENCODER_R_TIM, TIM_CHANNEL_ALL);

	// 初始化 IMU
	// Initialize IMU
	c.imu = newImu();
	c.imu.Enable(&c.imu); // 启用 IMU（初始化）

	// PID 参数数组
	// PID parameter arrays
	float pid_a[3] = {PID_A_P, PID_A_I, PID_A_D};
	float pid_l[3] = {PID_L_P, PID_L_I, PID_L_D};
	float pid_r[3] = {PID_R_P, PID_R_I, PID_R_D};

	// 初始化 PID 控制器（角度和速度）
	// Initialize PID controllers
	PID_init(&c.pid_a, PID_POSITION, pid_a, A_MAX_OUT, A_MAX_IOUT);
	PID_init(&c.pid_l, PID_POSITION, pid_l, V_MAX_OUT, V_MAX_IOUT);
	PID_init(&c.pid_r, PID_POSITION, pid_r, V_MAX_OUT, V_MAX_IOUT);

	// 启用电机驱动板（STBY 引脚拉高）
	// Enable motor driver board (set STBY high)
	HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_SET);

	// 绑定小车移动函数
	// Bind move function
	c.CarMove = CarMove;

	return c;
}

// PID 计算：角度 PID -> 速度 PID（左/右）
// PID calculation: Angle PID -> Velocity PIDs (left/right)
void MotorPidCalc(Car* self, int8_t setSpeed_l, int8_t setSpeed_r){
	// 计算平衡角度的 PID 输出
	// Calculate angle balance PID output
	PID_calc(&self->pid_a, self->imu.roll, MECHANICAL_BALANCE_BIAS);

	// 使用角度 PID 输出作为速度 PID 的期望值
	// Use angle PID output as the setpoint for speed PIDs
	PID_calc(&self->pid_l, self->encoder_l.rpm, self->pid_a.out);
	PID_calc(&self->pid_r, self->encoder_r.rpm, self->pid_a.out);
}

// 小车移动控制函数
// Car movement control function
void CarMove(Car* self, int8_t setSpeed){
	float set_rpm_l;
	float set_rpm_r;

	// 获取绝对速度值
	// Take absolute value of set speed
	setSpeed = __fabs(setSpeed);

	// 进行 PID 计算
	// Perform PID calculation
	MotorPidCalc(self, setSpeed, setSpeed);

	// 计算左右电机目标转速（角度 PID 输出 + 各自速度 PID 输出）
	// Compute target RPMs for each motor
	set_rpm_l = self->pid_l.out + self->pid_a.out;
	set_rpm_r = self->pid_r.out + self->pid_a.out;

	// 如果倾角过大，进行刹车
	// Brake if the tilt angle is too large
	if (car.imu.roll > 80 || car.imu.roll < -80) {
		car.isBrake = 1;
	} else {
		car.isBrake = 0;
	}

	// 控制左右电机运行
	// Drive left and right motors
	self->motor_l.Move(&self->motor_l, car.isBrake, (int32_t)set_rpm_l);
	self->motor_r.Move(&self->motor_r, car.isBrake, (int32_t)set_rpm_r);
}
