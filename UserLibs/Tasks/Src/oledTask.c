#include "oledTask.h"
#include "OLED.h"
#include "carTask.h"
#include "car.h"
#include "cmsis_os.h"
#include "communication.h"
#include "calibrate_angle.h"
#include "button.h"

extern float Med_Angle; 
extern float Target_Speed;
extern float Vertical_Kp;
extern float Vertical_Kd;
extern float Velocity_Kp;
extern float Velocity_Ki;   // 速度环Kp、Ki（正反馈）
extern float Turn_Kp;
extern int Init_target_value;

extern uint8_t cnt;

extern Car car;
uint16_t encoder_count1;
uint16_t encoder_count2;

void ShowPages(void){
	
	switch (cnt)
	{
		case 0:
			OLED_Printf((128 - 10*6)/2, 0, OLED_6X8, "BalanceCar", car.imu.roll);
			OLED_Printf((128 - 12*6)/2, 1*8, OLED_6X8, "Car States");
			OLED_Printf(0, 2*8, OLED_6X8, "Target Speed: %d", car.target_speed);
			OLED_Printf(0, 3*8, OLED_6X8, "Turn Speed:   %d", car.target_turn);
			OLED_Printf(0, 4*8, OLED_6X8, "IsBrake: 	 %d", car.isBrake);
			OLED_Printf(0, 5*8, OLED_6X8, "Battery: 	 %3.2f", 0.9);
			if(car.bt_state) OLED_Printf(0, 6*8, OLED_6X8, "BT State: Offline");
			else OLED_Printf(0, 6*8, OLED_6X8, "BT State: Online");
			OLED_Printf(0, 7*8, OLED_6X8, "Init_t_s: %d ", Init_target_value);
			break;
		case 1:
			OLED_Printf((128 - 10*6)/2, 0, OLED_6X8, "BalanceCar", car.imu.roll);
			OLED_Printf((128 - 13*6)/2, 1*8, OLED_6X8, "PID Parameter");
			OLED_Printf(0, 2*8, OLED_6X8, "Vertical");
			OLED_Printf(0, 3*8, OLED_6X8, "Kp:%.1f", Vertical_Kp);
			OLED_Printf(0, 4*8, OLED_6X8, "KD:%.1f", Vertical_Kd);
			OLED_Printf(0, 5*8, OLED_6X8, "Velocity");
			OLED_Printf(0, 6*8, OLED_6X8, "Kp:%.1f", Velocity_Kp);
			OLED_Printf(0, 7*8, OLED_6X8, "KD:%.3f", Velocity_Ki);
			break;
		case 2:
			OLED_Printf((128 - 10*6)/2, 0, OLED_6X8, "BalanceCar", car.imu.roll);
			OLED_Printf((128 - 8*6)/2, 1*8, OLED_6X8, "Car cmd");
			OLED_Printf(0, 2*8, OLED_6X8, "CMD ID:  %d", car.cmd);
			break;
		case 3:
			OLED_Printf((128 - 10*6)/2, 0, OLED_6X8, "BalanceCar", car.imu.roll);
			OLED_Printf((128 - 8*6)/2, 1*8, OLED_6X8, "IMU Data");
			OLED_Printf(0, 2*8, OLED_6X8, "Pitch:  %3.1f", car.imu.roll);
			OLED_Printf(0, 3*8, OLED_6X8, "Gyro-y: %3.1f", car.imu.gyrox);
			break;
		case 4:
			OLED_Printf((128 - 10*6)/2, 0, OLED_6X8, "BalanceCar", car.imu.roll);
			OLED_Printf((128 - 12*6)/2, 1*8, OLED_6X8, "Encoder Data");
			OLED_Printf(0, 2*8, OLED_6X8, "Encoder RPM");
			OLED_Printf(0, 3*8, OLED_6X8, "Left:   %3.1f", car.encoder_l.rpm);
			OLED_Printf(0, 4*8, OLED_6X8, "Right:  %3.1f", car.encoder_r.rpm);
			break;	
	}
	OLED_Update();
}

void StartOledTask(void const * arguments){
	while(1){
		ShowPages();
		
		osDelay(100);
	}
}
