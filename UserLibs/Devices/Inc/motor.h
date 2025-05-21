#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "main.h"
#include "stdio.h"
#include "tim.h"

#define BRAKE		 0
#define	FORWARD  1
#define	BACKWARD 2

#define MOTOR_MIN_RPM 0
#define MOTOR_MAX_RPM 330

#define CLAMP(x, min, max)  ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

typedef struct {
	TIM_HandleTypeDef *htim;
	uint32_t Channel;

	GPIO_TypeDef* IN1_GPIOx;
	uint16_t IN1_GPIO_Pin;
	GPIO_TypeDef* IN2_GPIOx;
	uint16_t IN2_GPIO_Pin;
} Motor_InitTypeDef;

typedef struct Motor Motor;

struct Motor {
	//member variable
	Motor_InitTypeDef Init;
	uint8_t direction;
	uint16_t setRPM;

	//member method
	void (*Move)(Motor* self, uint8_t direction, uint16_t setRPM);
};

Motor newMotor(Motor_InitTypeDef Init);
void Move(Motor* self, uint8_t direction, uint16_t setRPM);

#endif
