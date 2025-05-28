#include "multiTask.h"
#include "cmsis_os.h"
#include "car.h"
#include "calibrate_angle.h"

extern Car car;

void StartMultiTask(void const * argument){
	car.balance_bias = MECHANICAL_BALANCE_BIAS;
	while(1){
		
//		car.balance_bias = update_balance_target(car.encoder_l.rpm, car.encoder_r.rpm);
		osDelay(10);
	}
}
