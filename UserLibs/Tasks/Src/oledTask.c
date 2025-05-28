#include "oledTask.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "carTask.h"
#include "car.h"
#include "cmsis_os.h"
#include "communication.h"
#include "calibrate_angle.h"
extern Car car;
uint16_t encoder_count1;
uint16_t encoder_count2;

void StartOledTask(void const * argument){
	while(1){
//		ssd1306_Printf(0,0, Font_7x10, White, "car roll:%.3f     ", car.imu.roll);
//////		ssd1306_Printf(0,10, Font_7x10, White, "balance roll:%.3f     ", car.balance_bias);
//////		ssd1306_Printf(0,20, Font_7x10, White, "gyro:%.3f     ", car.imu.gyroy);
//		ssd1306_UpdateScreen();
		
		osDelay(10);
	}
}
