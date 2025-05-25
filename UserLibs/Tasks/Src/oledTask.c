#include "oledTask.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "carTask.h"
#include "car.h"
#include "cmsis_os.h"
#include "communication.h"
extern Car car;
uint16_t encoder_count1;
uint16_t encoder_count2;

void StartOledTask(void const * argument){
	car.set_v = 0;
	static int8_t symbol = -1;
	while(1){
		car.set_v += symbol * 10;
		if(car.set_v == 10)
			symbol = -1;
		else if(car.set_v == -10)
			symbol = 1;
		
		
		osDelay(3000);
	}
}
