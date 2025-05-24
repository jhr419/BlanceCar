#include "filter.h"
#include "string.h"

// 初始化滤波器（可指定初始电压，如-1500~+1500mV）
void Filter_Init(MotorVoltageFilter* filter, int16_t initial_voltage) {
    for(uint8_t i = 0; i < FILTER_WINDOW_SIZE; i++) {
        filter->buffer[i] = initial_voltage;
    }
    filter->index = 0;
    filter->filtered_value = initial_voltage;
}

// 改进的带符号冒泡排序（优化交换逻辑）
static void Sort_Buffer(int16_t* buf, uint8_t size) {
    uint8_t swapped;
    do {
        swapped = 0;
        for(uint8_t i = 0; i < size - 1; i++) {
            if(buf[i] > buf[i + 1]) {
                // 交换元素（使用异或交换避免临时变量）
                buf[i] ^= buf[i + 1];
                buf[i + 1] ^= buf[i];
                buf[i] ^= buf[i + 1];
                swapped = 1;
            }
        }
        size--;
    } while(swapped);
}

// 支持正负电压的滤波处理
int16_t Filter_Process(MotorVoltageFilter* filter, int16_t raw_voltage) {
    // 更新采样窗口（环形缓冲区）
    filter->buffer[filter->index] = raw_voltage;
    filter->index = (filter->index + 1) % FILTER_WINDOW_SIZE;

    // 复制到临时数组排序（不破坏原始数据顺序）
    int16_t temp_buf[FILTER_WINDOW_SIZE];
    memcpy(temp_buf, filter->buffer, sizeof(temp_buf));
    Sort_Buffer(temp_buf, FILTER_WINDOW_SIZE);

    // 去掉首尾各2个极值后求平均（防脉冲干扰）
    int32_t sum = 0;  // 使用int32_t防止累加溢出
    for(uint8_t i = 2; i < FILTER_WINDOW_SIZE - 2; i++) {
        sum += temp_buf[i];
    }
    
    // 计算平均值（四舍五入）
    filter->filtered_value = (int16_t)((sum + (FILTER_WINDOW_SIZE - 4)/2) / (FILTER_WINDOW_SIZE - 4));
    
    return filter->filtered_value;
}
