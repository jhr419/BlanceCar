#include "carTask.h"
#include "car.h"
#include "communication.h"
#include "cmsis_os.h"
#include "ssd1306.h"
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
extern Car car;

void StartCarTask(void const * argument){
	
	while(1){
		
		car.CarMove(&car, -5);
		
		car.imu.Get_Data(&car.imu);

		uart_printf(&huart2, "%f, %f, %f, \n",
		car.encoder_l.rpm,
		car.imu.roll,
		car.pid_a.out);
		uart_printf(&huart6, "%f, %f, %f, \n",
		car.encoder_l.rpm,
		car.imu.roll,
		car.pid_a.out);
		
//		ssd1306_Printf(0,20,Font_7x10, White, "lrpm:%4f", car.imu.roll);
//		ssd1306_Printf(0,30,Font_7x10, White, "lrpm:%4d", car.encoder_l.rpm);
//		
//		ssd1306_UpdateScreen();
		
		osDelay(10);
	}
}
