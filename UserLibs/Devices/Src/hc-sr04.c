#include "hc-sr04.h"
#include "communication.h"
#include "tim.h"
#include "button.h"
#include "car.h"

#define MAX_PAGE 5

extern Car car;
extern int Init_target_value;

uint8_t K[4] = {0, 0, 0, 0};  

// 系统状态标志 | System status flags
uint8_t isTrig = 0;   // 连续测量触发标志 | Continuous measurement trigger flag
uint8_t uint = 0;     // 单位选择标志(0:cm 1:inch) | Unit selection flag (0:cm 1:inch)
uint8_t alarm = 0;     // 报警功能标志 | Alarm function flag

uint8_t cnt=0;
uint32_t time1=0;
uint32_t time2=0;
uint32_t time3=0;

uint32_t current;

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
	switch(GPIO_Pin)
    {
		case BT_STATE_Pin:
			if(HAL_GPIO_ReadPin(BT_STATE_GPIO_Port, BT_STATE_Pin) == GPIO_PIN_SET){
				//online
				car.bt_state = 0;
				
//				HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_SET);
			}
			else if(HAL_GPIO_ReadPin(BT_STATE_GPIO_Port, BT_STATE_Pin) == GPIO_PIN_RESET){
				car.bt_state = 1;
				car.target_speed = 0;
//				HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_SET);
			}
			break;
		case EHCO_Pin:
			current = __HAL_TIM_GET_COUNTER(&htim10);
        
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
			
			if(dis < 800){
				car.isBarrier = 1;
				HAL_TIM_Base_Stop_IT(&htim11);
				__HAL_TIM_SET_AUTORELOAD(&htim11, 7999); 
				HAL_TIM_Base_Start_IT(&htim11);
			}
			//YOUR SHIT CODE  else car.isBarrier = 0;
        
		}
			break;
			
				
        case K1_Pin:  // 单次测量按键 | Single measurement button
            if(GPIO_PIN_SET == HAL_GPIO_ReadPin(K1_GPIO_Port, K1_Pin))
            {
							K[0] = 1;                     // 标记按键按下 | Mark button pressed
							
							cnt = (cnt==0) ? 0 : (cnt-1);
            }
            else if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(K1_GPIO_Port, K1_Pin))
            {
              K[0] = 0;  // 标记按键释放 | Mark button released
				OLED_Clear();
			OLED_Update();
            }
			
            break;
            
        case K2_Pin:  // 连续测量模式切换按键 | Continuous mode toggle button
            if(GPIO_PIN_SET == HAL_GPIO_ReadPin(K2_GPIO_Port, K2_Pin))
            {
              // 切换K2状态 | Toggle K2 state
              K[1] = 1;
              cnt = (cnt==MAX_PAGE) ? MAX_PAGE : (cnt+1);
			  
            }
			else if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(K2_GPIO_Port, K2_Pin))
            {
			  K[1] = 0;
				OLED_Clear();
			OLED_Update();	
			}
			
            break;
            
        case K3_Pin:  // 报警功能按键 | Alarm function button
            if(GPIO_PIN_SET == HAL_GPIO_ReadPin(K3_GPIO_Port, K3_Pin))
            {
              K[2] = 1;
            }
						else if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(K2_GPIO_Port, K2_Pin))
            {
				car.isBrake = (0 == car.isBrake) ? 1 : 0;
							K[2] = 0;
						}
            break;
            
        case K4_Pin:  // 单位切换按键 | Unit toggle button
            if(GPIO_PIN_SET == HAL_GPIO_ReadPin(K4_GPIO_Port, K4_Pin))
            {
                K[3] = 1; 
				Init_target_value = 8;		//Reset Init_target_value     
            }
            else if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(K4_GPIO_Port, K4_Pin))
            {
                K[3] = 0;  // 标记按键释放 | Mark button released
            }
            break;
    }
}

// 获取脉冲宽度(us)
uint32_t Get_PulseWidth(void){
    while(!captureFlag); // 等待新捕获
    captureFlag = 0;
    return pulseWidth_us;
}




