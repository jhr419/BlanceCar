#include "calibrate_angle.h"
#include "car.h"

// ==== 定义变量（可放在主控文件开头） ====
float angle_target = MECHANICAL_BALANCE_BIAS;   // 初始为机械平衡偏置
float learning_rate = 0.001f;                 // 学习率
float speed_filter = 0.0f;                      // 一阶滤波后的速度
float speed_alpha = 0.8f;                       // 滤波系数
float speed_estimate = 0.0f;                    // 积分估算的速度
const float dt = 0.01f;                         // 控制周期（单位秒），假设 10ms 调用一次

// ==== 使用 IMU 加速度估算速度并更新角度 ====
float update_balance_target(float acc_x)
{
    // 步骤1：对加速度积分，估算当前速度（acc_x 单位为 m/s²）
    speed_estimate += acc_x * dt;

    // 步骤2：滤波速度，抑制波动
    speed_filter = speed_alpha * speed_filter + (1.0f - speed_alpha) * speed_estimate;

    // 步骤3：根据速度趋势调整目标角度
    angle_target -= learning_rate * speed_filter;

    // 步骤4（可选）：限制角度范围
    if (angle_target >  5.0f) angle_target =  5.0f;
    if (angle_target < -5.0f) angle_target = -5.0f;

    return angle_target;
}
