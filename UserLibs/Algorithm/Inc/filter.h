#ifndef FILTER_H
#define FILTER_H

#include "main.h"

#define FILTER_WINDOW_SIZE 16  // 建议根据实际采样频率调整（8-16之间）

// 电机电压滤波器结构体（可管理多个电机）
typedef struct {
    int16_t buffer[FILTER_WINDOW_SIZE];
    uint8_t index;
    int16_t filtered_value;
} MotorVoltageFilter;

void Filter_Init(MotorVoltageFilter* filter, int16_t initial_voltage);
int16_t Filter_Process(MotorVoltageFilter* filter, int16_t raw_voltage);

#endif
