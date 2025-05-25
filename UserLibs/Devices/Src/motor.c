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


void Move(Motor* self, uint8_t isBrake, int32_t setRPM) {
	if (isBrake) {
		HAL_GPIO_WritePin(self->Init.IN1_GPIOx, self->Init.IN1_GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(self->Init.IN2_GPIOx, self->Init.IN2_GPIO_Pin, GPIO_PIN_SET);
		self->direction = BRAKE;
	}
	else if(!isBrake){
		if (setRPM > 0) {
			HAL_GPIO_WritePin(self->Init.IN1_GPIOx, self->Init.IN1_GPIO_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(self->Init.IN2_GPIOx, self->Init.IN2_GPIO_Pin, GPIO_PIN_RESET);
			self->direction = FORWARD;
		} else if (setRPM < 0) {
			HAL_GPIO_WritePin(self->Init.IN1_GPIOx, self->Init.IN1_GPIO_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(self->Init.IN2_GPIOx, self->Init.IN2_GPIO_Pin, GPIO_PIN_SET);
			self->direction = BACKWARD;
		}	else if(setRPM == 0){
			HAL_GPIO_WritePin(self->Init.IN1_GPIOx, self->Init.IN1_GPIO_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(self->Init.IN2_GPIOx, self->Init.IN2_GPIO_Pin, GPIO_PIN_RESET);
	}
	}
	self->setRPM = (float)setRPM;
	
	__HAL_TIM_SET_COMPARE(self->Init.htim, self->Init.Channel, __fabs(setRPM));
}
