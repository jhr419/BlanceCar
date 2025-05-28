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
		car.imu.Get_Data(&car.imu);
		car.CarMove(&car, 0);
		
//		uart_printf(&huart_pc, "%f, %f, %f, %f, %f, %f\n", 				
//		car.pid_a.set,
//		car.pid_a.fdb,
//		car.pid_a.out,
//		car.imu.gyrox,
//		car.imu.gyroy,
//		car.imu.gyroz
//		);
		
		osDelay(5);
	}
}
