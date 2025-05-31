#include "potentiometers.h"
#include "car.h"
#include "communication.h"

#define NUM_CHANNELS 3  // ADC通道数量 | Number of ADC channels

uint32_t ADC_value[3];  // ADC采样值缓存数组 | ADC sample value buffer array
uint32_t GetADC;

extern int Init_target_value;

void set_traget_speed(uint16_t ADC_value);

void ADC_Scan(void){
    // 启动ADC1的DMA连续转换
    // Start ADC1 with DMA continuous conversion
    HAL_ADC_Start_DMA(&hadc1,             // ADC1外设句柄 | ADC1 peripheral handle
                     (uint32_t*)ADC_value, // 转换结果存储地址 | Conversion result buffer
                     NUM_CHANNELS);        // 转换通道数量 | Number of conversion channels
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
    // 设置测量阈值(使用通道0的值)
    // Set measurement threshold (using channel 0 value)
	GetADC = (int)(ADC_value[0] / 20);
	GetADC = CLAMP(GetADC, 0, 40);
	set_traget_speed(GetADC);
   
	uart_printf(&huart_pc, "%d, %d\n", 				
	Init_target_value, ADC_value[0]
		);
}

void set_traget_speed(uint16_t ADC_value){
	Init_target_value = ADC_value;
}
