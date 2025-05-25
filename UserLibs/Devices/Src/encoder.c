#include "encoder.h"
#include "car.h"

Encoder newEncoder(TIM_HandleTypeDef *htim,	uint32_t Channel){
	Encoder e;
	e.htim = htim;
	e.Channel = Channel;
	e.count = 0;
	e.last_count = 0;
	
	e.GetCount = GetCount;
	
	HAL_TIM_Encoder_Start(htim, Channel);
	
	return e;
}

//这个函数，可以直接更新实例，同时也能返回值,这里命名还有待商榷
uint16_t GetCount(Encoder* self){
	self->last_count = self->count;
	static uint16_t cnt=0;
	cnt = (uint16_t)__HAL_TIM_GET_COUNTER(self->htim);

	__HAL_TIM_SetCounter(self->htim, 0);
	
	self->rpm = (float)((cnt > 30000) ? (cnt - 65536) : cnt);
	return self->count;
}

int16_t GetRPM(Encoder* self){
	return self->rpm;
}
