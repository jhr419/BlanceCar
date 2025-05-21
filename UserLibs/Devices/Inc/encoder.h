#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "main.h"
#include "tim.h"

typedef struct Encoder Encoder;

struct Encoder{
	TIM_HandleTypeDef *htim;
	uint32_t Channel;
	uint16_t count;
	uint16_t last_count;
	float rpm;
	uint16_t (*GetCount)(Encoder* self);
};

Encoder newEncoder(TIM_HandleTypeDef *htim,	uint32_t Channel);
uint16_t GetCount(Encoder* self);
int16_t GetRPM(Encoder* self);
#endif
