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
		uart_printf(&huart2, "%f,%f,%f\n",
		car.encoder_l.rpm,
		car.imu.roll,
		car.pid_a.out);
		
		uart_printf(&huart6, "%f, %f, %f, %f, %f, %f, %d, %d\n", car.pid_a.set, car.pid_a.fdb, car.pid_a.out, car.pid_a.Iout, 
		car.pid_l.out,																															
		car.pid_r.out,
		car.motor_l.direction,
		car.motor_r.direction
		);
		
		osDelay(10);
	}
}
