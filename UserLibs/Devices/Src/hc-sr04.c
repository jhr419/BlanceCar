#include "hc-sr04.h"
#include "communication.h"
#include "tim.h"

uint32_t lastCapture = 0;       // 回波信号上升沿时间戳 | Echo Signal Rising Edge Timestamp
uint32_t falling_edge_time = 0;      // 回波信号下降沿时间戳 | Echo Signal Falling Edge Timestamp
uint32_t current_time = 0;
volatile uint32_t pulseWidth_us = 0;
volatile uint8_t captureFlag = 0;
float dis = 0;
void HC_trig(void){
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);  // 拉低TRIG引脚(确保起始状态) | Pull TRIG low (ensure initial state)
    delay_us(50);                                                 // 保持低电平50us(大于2ms周期要求) | Hold low for 50us (>2ms cycle requirement)
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);    // 拉高TRIG引脚10us以上 | Pull TRIG high for >10us
}

float calc_dis(void){
    // 距离(cm) = 脉冲宽度(us)×10^-6 × 声速(342.62m/s) ÷ 2 × 100(cm/m)
    dis = (float)pulseWidth_us * 0.001 * 342.62 / 2;
}

// 外部中断服务例程
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == EHCO_Pin) {
        uint32_t current = __HAL_TIM_GET_COUNTER(&htim10);
        
        if (HAL_GPIO_ReadPin(EHCO_GPIO_Port, EHCO_Pin) == GPIO_PIN_SET) {
            // 上升沿：记录起始时间
            lastCapture = current;
        } else {
            // 下降沿：计算脉宽（处理定时器溢出）
            if (current >= lastCapture) {
                pulseWidth_us = current - lastCapture;
            } else {
                // 处理16位定时器溢出
                pulseWidth_us = (65535 - lastCapture) + current + 1;
            }
            captureFlag = 1;  // 标记新数据可用
						dis = calc_dis();
        }
    }
}

// 获取脉冲宽度(us)
uint32_t Get_PulseWidth(void){
    while(!captureFlag); // 等待新捕获
    captureFlag = 0;
    return pulseWidth_us;
}




