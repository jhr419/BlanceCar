#include "car.h"
#include "pid2.h"
#include "communication.h"
#include "hc-sr04.h"
#include "cmsis_os.h"

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
#define PID_A_P 3000.0f//2800.0f
#define PID_A_I	0.0f
#define PID_A_D	0.0f //1200.0f

#define PID_L_P	2800.0f
#define PID_L_I	50.0f
#define PID_L_D	0.0f

#define PID_R_P	2800.0f
#define PID_R_I	50.0f
#define PID_R_D	0.0f

// 定义全局小车对象
// Global car instance
Car car;

int Vertical_out,Velocity_out,Turn_out; // 直立环&速度环&转向环的输出变量
int RUNNING = 0;	//标记当前小车的运动状态
int ROAD_MODE = 0;	//小车的避障模式
int PWM_out;
int PWM_MAX=30000,PWM_MIN=-30000;	// PWM限幅变量
int MOTO1,MOTO2;
int Init_target_value = 8;

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
	car.bt_state = 0;
	HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_SET);

	// 绑定小车移动函数
	// Bind move function
	c.CarMove = CarMove;

	return c;
}

int MotorPidCalc(){
	int PWM_out;
	
	//运动相关指令
	if(car.cmd == CMD_STOP) 		{car.target_speed = 0; RUNNING = 0;}
	if(car.cmd == CMD_STOP_SLOWLY)	{
		if(car.target_speed >= 4)	car.target_speed = car.target_speed * 0.5;
		else	car.cmd = CMD_STOP;
	}
	if(car.cmd == CMD_FORWARD)  	{
		if(RUNNING != 1)			{car.target_speed = Init_target_value;	RUNNING = 1;}
		else	RUNNING = 1;
	}
	if(car.cmd == CMD_BACKWARD) 	{
		if(RUNNING != 2)			{car.target_speed = -1 * Init_target_value; RUNNING = 2;}
		else	RUNNING = 2;
	}
	if(car.cmd == CMD_SPEED_UP)		{
		if(RUNNING == 1) {car.target_speed += 1;car.cmd = CMD_DEFAULT;}
		if(RUNNING == 2) {car.target_speed -= 1;car.cmd = CMD_DEFAULT;}
	}
	if(car.cmd == CMD_SPEED_DOWN)	{
		if(RUNNING == 1) {if(car.target_speed > 0) car.target_speed -= 2; car.cmd = CMD_DEFAULT;}
		if(RUNNING == 2) {if(car.target_speed < 0) car.target_speed += 2; car.cmd = CMD_DEFAULT;}
	}
	car.target_speed = CLAMP(car.target_speed, -40, 40);
	
	//转向相关指令
	if(car.cmd == CMD_STOP) 		car.target_turn = 0;
	if(car.cmd == CMD_STOP_SLOWLY)	car.target_turn = 0;
	if(car.cmd == CMD_LEFT)			car.target_turn = -3000;
	if(car.cmd == CMD_RIGHT)		car.target_turn = 3000;
	if(car.cmd == CMD_TURN_AROUND)	{
		if(car.target_speed == 0) car.target_speed = 0;
		else car.target_turn = 5000;
	}
	if(car.cmd == CMD_TURN_CLEAR)	car.target_turn = 0;
		
	//辅助功能指令
	if(car.cmd == CMD_DOMINATE)		{HAL_GPIO_TogglePin(STBY_GPIO_Port,STBY_Pin); car.cmd = CMD_DEFAULT;}
	if(car.cmd == CMD_ROAD_PLANNING){
		ROAD_MODE=(0==ROAD_MODE)?1:0;
		car.cmd = CMD_DEFAULT;
	}
	
	car.target_turn = CLAMP(car.target_turn, -5000, 5000);
	    
//	uart_printf(&huart_bt, "%d, %d", car.target_speed, car.target_turn);
  /*转向约束*/
//  if((Left==0)&&(Right==0))Turn_Kd=-0.6;    // 若无左右转向指令，则开启转向约束
//  else if((Left==1)||(Right==1))Turn_Kd=0;  // 若左右转向指令接收到，则去掉转向约束
     
	Velocity_out = Velocity(car.target_speed,car.encoder_l.rpm,car.encoder_r.rpm);
	Vertical_out = Vertical(Velocity_out + MECHANICAL_BALANCE_BIAS, car.imu.roll, car.imu.gyrox);
	Turn_out=Turn(car.target_turn);
	PWM_out = Vertical_out;
	return PWM_out;
}

void CarDirection(uint8_t cmd){
	float roll_bias;
	switch(cmd){
		case 0xC1:
			break;
		case 0xC2:
			break;
		case 0xC3:
			break;
		case 0xC4:
			break;
		default:
			break;
	}
}

void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}

void Barrier(){
	switch(ROAD_MODE){
		case 0:
			break;
		case 1:
			if(car.isBarrier)
			car.cmd = CMD_BACKWARD;
			
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;	
		}
}

// 小车移动控制函数
// Car movement control function
void CarMove(Car* self, int8_t setSpeed){
	Barrier();
	PWM_out=MotorPidCalc();
	
	// 如果倾角过大，进行刹车
	// Brake if the tilt angle is too large
	if (car.imu.roll > 60 || car.imu.roll < -60) {
		car.isBrake = 1;
	} else {
		car.isBrake = 0;
	}
	
	MOTO1 = PWM_out-Turn_out; // 左电机
	MOTO2 = PWM_out+0.8*Turn_out; // 右电机
	
  Limit(&MOTO1,&MOTO2);     // PWM限幅
//	uart_printf(&huart_pc, "%d, %d, %d, %d\n", 				
//	car.isBarrier, car.cmd, car.target_speed,ROAD_MODE
//		);
//	uart_printf(&huart_pc, "%d\n", 				
//	Init_target_value
//		);
  self->motor_l.Move(&self->motor_l, car.isBrake, MOTO1);
	self->motor_r.Move(&self->motor_r, car.isBrake, MOTO2);
}
