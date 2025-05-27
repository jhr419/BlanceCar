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
	car.balance_bias = MECHANICAL_BALANCE_BIAS;
	while(1){
		car.balance_bias = update_balance_target(car.imu.ay);
		osDelay(5);
	}
}
