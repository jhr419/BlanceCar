#include "calibrate_angle.h"
#include "car.h"


// ==== 定义变量（可放在主控文件开头） ====
float angle_target = MECHANICAL_BALANCE_BIAS;       // 自学习后的目标角度（初始设为机械平衡偏置）
float learning_rate = 0.00001f;   // 学习率，值越小调整越慢
float speed_filter = 0.0f;        // 一阶滤波后的速度
float speed_alpha = 0.98f;        // 滤波系数（越接近1越平滑）

// ==== 输入结构体中必须包含左右轮速度 ====
float update_balance_target(float speed_l, float speed_r)
{
    // 步骤1：计算车体整体速度（左右轮平均）
    float current_speed = (speed_l + speed_r) / 2.0f;

    // 步骤2：滑动平均滤波处理速度，抑制波动
    speed_filter = speed_alpha * speed_filter + (1.0f - speed_alpha) * current_speed;

    // 步骤3：根据速度趋势微调目标角度（车冲前就往后拉角度）
    angle_target -= learning_rate * speed_filter;

    // 步骤4（可选）：限制角度范围，防止过度偏移
    if (angle_target > 5.0f) angle_target = 5.0f;
    if (angle_target < -5.0f) angle_target = -5.0f;

    return angle_target;
}
