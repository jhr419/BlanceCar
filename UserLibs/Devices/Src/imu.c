#include "imu.h"

// 创建一个新的 IMU（惯性测量单元）实例，并初始化函数指针
// Create a new IMU instance and initialize function pointers
Imu newImu(void) {
	Imu i;

	// 绑定启用函数和获取数据函数
	// Bind enable and data retrieval functions
	i.Enable = Enable;
	i.Get_Data = Get_Data;

	return i; // 返回初始化后的 IMU 结构体
}

// 初始化 IMU，调用底层 MPU6500 初始化函数
// Initialize the IMU by calling the low-level MPU6500 initialization
int Enable(Imu* self) {
	// 保存初始化结果（成功或失败）
	// Save the result of initialization (success or failure)
	self->init_result = (uint8_t)MPU_6500_Init();
	// 此函数应返回初始化状态（例如 0 表示成功）
	// Should return initialization status (e.g., 0 for success)
	return self->init_result;
}

// 从 MPU6500 中获取姿态角数据（俯仰、横滚、航向）
// Retrieve attitude data (pitch, roll, yaw) from MPU6500
void Get_Data(Imu* self) {
	// 调用 DMP（数字运动处理器）接口获取三轴姿态角数据
	// Call the DMP interface to get pitch, roll, and yaw values
	MPU6500_DMP_Get_Data(&(self->pitch), &(self->roll), &(self->yaw), 
												&(self->ax), &(self->ay), &(self->az),
												&(self->gyrox), &(self->gyroy), &(self->gyroz));
}
