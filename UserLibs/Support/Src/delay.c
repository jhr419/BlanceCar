#include "main.h"

#define USE_HAL_LEGACY
#include "stm32_hal_legacy.h"

// Timebase source selection (1 for SysTick, 0 for other timer when using FreeRTOS)
// 时基源选择（1表示使用SysTick，0表示使用其他定时器，当使用FreeRTOS时）
//#define Timebase_Source_is_SysTick 1     // Timebase Source is SysTick set to 1
#define Timebase_Source_is_SysTick 0   // When using FreeRTOS, Timebase Source is not SysTick set to 0

#if (!Timebase_Source_is_SysTick)
    extern TIM_HandleTypeDef htim5;      // When using FreeRTOS, Timebase Source is another timer, need to define
    #define Timebase_htim htim5         // 使用FreeRTOS时，时基源为其他定时器，需要定义

    // Macro definitions for timer counter and autoreload value
    // 定时器计数器和自动重装载值的宏定义
    #define Delay_GetCounter()      __HAL_TIM_GetCounter(&Timebase_htim)
    #define Delay_GetAutoreload()   __HAL_TIM_GetAutoreload(&Timebase_htim)
#else
    // SysTick counter and reload value macros
    // SysTick计数器和重装载值宏
    #define Delay_GetCounter()      (SysTick->VAL)
    #define Delay_GetAutoreload()   (SysTick->LOAD)
#endif

// Static variables for delay calculation
// 用于延时计算的静态变量
static uint16_t fac_us = 0;   // Microsecond factor 微秒因子
static uint32_t fac_ms = 0;   // Millisecond factor 毫秒因子

/**
 * @brief Initialize delay function
 * @brief 初始化延时函数
 */
void delay_init(void)
{
#if (!Timebase_Source_is_SysTick)
    fac_ms = 1000000;         // Assuming HAL_InitTick() configures timer to 1MHz
                              // 假设HAL_InitTick()将定时器配置为1MHz
    fac_us = fac_ms / 1000;   // Calculate microsecond factor 计算微秒因子
#else
    fac_ms = SystemCoreClock / 1000;  // Calculate milliseconds factor based on system clock
                                      // 根据系统时钟计算毫秒因子
    fac_us = fac_ms / 1000;          // Calculate microseconds factor 计算微秒因子
#endif
}

/**
 * @brief Microsecond delay
 * @brief 微秒延时
 * @param nus Number of microseconds to delay 需要延时的微秒数
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks = 0;   // Total ticks needed 需要的总滴答数
    uint32_t told = 0;    // Previous counter value 上一次计数器值
    uint32_t tnow = 0;    // Current counter value 当前计数器值
    uint32_t tcnt = 0;    // Accumulated ticks 累计滴答数
    uint32_t reload = 0;  // Reload value 重装载值

    reload = Delay_GetAutoreload();  // Get autoreload value 获取自动重装载值
    ticks = nus * fac_us;           // Calculate required ticks 计算需要的滴答数

    told = Delay_GetCounter();      // Get initial counter value 获取初始计数器值

    while (1)
    {
        tnow = Delay_GetCounter();  // Get current counter value 获取当前计数器值

        if (tnow != told)           // If counter has changed 如果计数器发生变化
        {
            if (tnow < told)        // Handle overflow condition 处理溢出情况
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;            // Update previous value 更新前一个值
            if (tcnt >= ticks)      // Check if delay completed 检查延时是否完成
            {
                break;
            }
        }
    }
}

/**
 * @brief Millisecond delay
 * @brief 毫秒延时
 * @param nms Number of milliseconds to delay 需要延时的毫秒数
 */
void delay_ms(uint32_t nms)
{
    uint32_t ticks = 0;   // Total ticks needed 需要的总滴答数
    uint32_t told = 0;    // Previous counter value 上一次计数器值
    uint32_t tnow = 0;    // Current counter value 当前计数器值
    uint32_t tcnt = 0;    // Accumulated ticks 累计滴答数
    uint32_t reload = 0;  // Reload value 重装载值

    reload = Delay_GetAutoreload();  // Get autoreload value 获取自动重装载值
    ticks = nms * fac_ms;           // Calculate required ticks 计算需要的滴答数

    told = Delay_GetCounter();      // Get initial counter value 获取初始计数器值

    while (1)
    {
        tnow = Delay_GetCounter();  // Get current counter value 获取当前计数器值

        if (tnow != told)           // If counter has changed 如果计数器发生变化
        {
            if (tnow < told)        // Handle overflow condition 处理溢出情况
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;            // Update previous value 更新前一个值
            if (tcnt >= ticks)      // Check if delay completed 检查延时是否完成
            {
                break;
            }
        }
    }
}

/**
 * @brief HAL_Delay override
 * @brief 重写的HAL_Delay函数
 * @param Delay Delay in milliseconds 延时的毫秒数
 */
void HAL_Delay(uint32_t Delay)
{
    uint32_t tickstart = HAL_GetTick();  // Get start tick 获取开始时刻
    uint32_t wait = Delay;               // Set wait time 设置等待时间

    /* If minimum delay is required, uncomment this section */
    /* 如果需要最小延时，取消注释此部分 */
    // if (wait < HAL_MAX_DELAY)
    // {
    //     wait += (uint32_t)(uwTickFreq);
    // }

    // Wait until specified time has elapsed
    // 等待直到指定时间过去
    while ((HAL_GetTick() - tickstart) < wait)
    {
    }
}
