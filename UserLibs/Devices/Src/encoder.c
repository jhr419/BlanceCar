#include "encoder.h"
#include "car.h"

// 创建一个新的编码器实例，初始化计数器和通道信息
// Create a new encoder instance and initialize counter and channel info
Encoder newEncoder(TIM_HandleTypeDef *htim, uint32_t Channel) {
	Encoder e;
	e.htim = htim;           // 关联定时器句柄
	e.Channel = Channel;     // 设置通道
	e.count = 0;             // 初始化当前计数值
	e.last_count = 0;        // 初始化上一次计数值

	e.GetCount = GetCount;  // 绑定获取计数函数指针

	// 启动编码器接口（包含通道A和B）
	// Start the encoder interface (both channel A and B)
	HAL_TIM_Encoder_Start(htim, Channel);

	return e; // 返回初始化后的编码器结构体
}

// 获取当前编码器计数值，同时更新内部状态
// Get the current encoder count and update internal state
// 函数命名为 GetCount 可能存在歧义，可改名为 UpdateAndGetCount 更明确
uint16_t GetCount(Encoder* self) {
	self->last_count = self->count; // 保存上一次的计数值

	static uint16_t cnt = 0;
	// 从硬件计数器中读取当前值
	// Read current value from hardware counter
	cnt = (uint16_t)__HAL_TIM_GET_COUNTER(self->htim);

	// 重置计数器为0，为下一次计数做准备
	// Reset the counter to 0 for the next measurement cycle
	__HAL_TIM_SetCounter(self->htim, 0);

	// 计算当前rpm值：如果计数器出现回绕则进行校正
	// Calculate RPM: correct if counter wraps around
	self->rpm = (float)((cnt > 30000) ? (cnt - 65536) : cnt);

	return self->count; // 返回当前（尚未更新的）总计数值
}

// 获取当前编码器计算得到的RPM（转速）
// Get the current calculated RPM (rotations per unit time)
int16_t GetRPM(Encoder* self) {
	return self->rpm;
}
