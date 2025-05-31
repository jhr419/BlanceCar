#include "multiTask.h"
#include "cmsis_os.h"
#include "car.h"
#include "calibrate_angle.h"
#include "hc-sr04.h"
#include "potentiometers.h"

extern Car car;

void StartMultiTask(void const * argument){
	while(1){
		HC_trig();
		ADC_Scan();
		osDelay(100);
	}
}
