#include "motor.h"

Motor newMotor(Motor_InitTypeDef Init) {
	Motor m;
	m.Init = Init;

	m.direction = FORWARD;
	m.setRPM = MOTOR_MIN_RPM;

	m.Move = Move;

	HAL_TIM_PWM_Start(Init.htim, Init.Channel);

	return m;
}

void SetDuty(Motor* self, uint16_t rpm) {
	rpm = CLAMP(rpm, MOTOR_MIN_RPM, MOTOR_MAX_RPM);

	uint32_t compare = (uint32_t)(((float)rpm / MOTOR_MAX_RPM) * htim2.Init.Period);

	__HAL_TIM_SET_COMPARE(self->Init.htim, self->Init.Channel, compare);
}


void Move(Motor* self, uint8_t direction, uint16_t setRPM) {
	if (direction == FORWARD) {
		HAL_GPIO_WritePin(self->Init.IN1_GPIOx, self->Init.IN1_GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(self->Init.IN2_GPIOx, self->Init.IN2_GPIO_Pin, GPIO_PIN_RESET);
	} else if (direction == BACKWARD) {
		HAL_GPIO_WritePin(self->Init.IN1_GPIOx, self->Init.IN1_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(self->Init.IN2_GPIOx, self->Init.IN2_GPIO_Pin, GPIO_PIN_SET);
	} else if (direction == BRAKE) {
		HAL_GPIO_WritePin(self->Init.IN1_GPIOx, self->Init.IN1_GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(self->Init.IN2_GPIOx, self->Init.IN2_GPIO_Pin, GPIO_PIN_SET);
	}
	
	self->direction = direction;
	self->setRPM = setRPM;
	
//	SetDuty(self, setRPM);
	__HAL_TIM_SET_COMPARE(self->Init.htim, self->Init.Channel, setRPM);
}
