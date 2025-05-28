#include "oledTask.h"
#include "OLED.h"
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
		OLED_Printf(0, 0, OLED_6X8, "%6.3f", car.imu.roll);

		OLED_Update();
		
		osDelay(100);
	}
}
