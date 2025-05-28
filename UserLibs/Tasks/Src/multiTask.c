#include "multiTask.h"
#include "cmsis_os.h"
#include "car.h"
#include "calibrate_angle.h"
#include "hc-sr04.h"

extern Car car;

void StartMultiTask(void const * argument){
	car.balance_bias = MECHANICAL_BALANCE_BIAS;
	while(1){
		HC_trig();
	
		
		osDelay(10);
	}
}
