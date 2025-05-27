#include "motor.h"

// 创建一个新的电机实例，初始化相关参数
// Create a new motor instance and initialize related parameters
Motor newMotor(Motor_InitTypeDef Init) {
	Motor m;
	m.Init = Init;  // 保存初始化结构体

	m.direction = FORWARD;    // 初始方向设置为正转
	m.setRPM = MOTOR_MIN_RPM; // 初始转速为最小转速

	m.Move = Move;            // 绑定移动函数指针

	// 启动对应通道的PWM输出
	// Start PWM output on the specified channel
	HAL_TIM_PWM_Start(Init.htim, Init.Channel);

	return m; // 返回初始化后的电机结构体
}

// 设置PWM占空比以调整电机转速
// Set the PWM duty cycle to control motor speed
void SetDuty(Motor* self, uint16_t rpm) {
	// 限制RPM在最大最小之间
	// Clamp RPM between MOTOR_MIN_RPM and MOTOR_MAX_RPM
	rpm = CLAMP(rpm, MOTOR_MIN_RPM, MOTOR_MAX_RPM);

	// 根据最大转速换算比较值
	// Calculate the compare value based on maximum RPM and timer period
	uint32_t compare = (uint32_t)(((float)rpm / MOTOR_MAX_RPM) * htim2.Init.Period);

	// 设置PWM比较值来控制占空比
	// Set the PWM compare value to control duty cycle
	__HAL_TIM_SET_COMPARE(self->Init.htim, self->Init.Channel, compare);
}

// 控制电机移动，包括制动、正转、反转和停止
// Control motor movement: brake, forward, reverse, and stop
void Move(Motor* self, uint8_t isBrake, int32_t setRPM) {
	if (isBrake) {
		// 刹车模式：IN1和IN2同时为高电平
		// Brake mode: set both IN1 and IN2 to high
		HAL_GPIO_WritePin(self->Init.IN1_GPIOx, self->Init.IN1_GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(self->Init.IN2_GPIOx, self->Init.IN2_GPIO_Pin, GPIO_PIN_SET);
		self->direction = BRAKE; // 设置方向为刹车
	}
	else if (!isBrake) {
		if (setRPM > 0) {
			// 正转：IN1高，IN2低
			// Forward: IN1 high, IN2 low
			HAL_GPIO_WritePin(self->Init.IN1_GPIOx, self->Init.IN1_GPIO_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(self->Init.IN2_GPIOx, self->Init.IN2_GPIO_Pin, GPIO_PIN_RESET);
			self->direction = FORWARD;
		} else if (setRPM < 0) {
			// 反转：IN1低，IN2高
			// Reverse: IN1 low, IN2 high
			HAL_GPIO_WritePin(self->Init.IN1_GPIOx, self->Init.IN1_GPIO_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(self->Init.IN2_GPIOx, self->Init.IN2_GPIO_Pin, GPIO_PIN_SET);
			self->direction = BACKWARD;
		} else if (setRPM == 0) {
			// 停止：IN1和IN2都低
			// Stop: both IN1 and IN2 low
			HAL_GPIO_WritePin(self->Init.IN1_GPIOx, self->Init.IN1_GPIO_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(self->Init.IN2_GPIOx, self->Init.IN2_GPIO_Pin, GPIO_PIN_RESET);
		}
	}

	// 设置当前转速值
	// Update the target RPM
	self->setRPM = (float)setRPM;

	// 设置PWM占空比（取RPM绝对值）
	// Set PWM compare value based on absolute RPM
	__HAL_TIM_SET_COMPARE(self->Init.htim, self->Init.Channel, __fabs(setRPM));
}
