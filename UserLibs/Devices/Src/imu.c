#include "imu.h"

Imu newImu(void){
	Imu i;
	
	i.Enable = Enable;
	i.Get_Data = Get_Data;
	
	return i;
}

int Enable(Imu* self){
	self->init_result = (uint8_t)MPU_6500_Init();
}

void Get_Data(Imu* self){
	MPU6500_DMP_Get_Data(&(self->pitch), &(self->roll), &(self->yaw));
}
