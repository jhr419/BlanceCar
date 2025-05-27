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
		
//		uart_printf(&huart6, "%f, %f, %f, %f, %f, %f\n", 				
//		car.imu.roll,
//		car.balance_bias,
//		car.imu.gyrox,
//		
//		car.imu.ax,
//		car.imu.ay,
//		car.imu.az
//		);
		
		osDelay(1);
	}
}
