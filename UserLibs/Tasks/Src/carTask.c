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

		osDelay(5);
	}
}
