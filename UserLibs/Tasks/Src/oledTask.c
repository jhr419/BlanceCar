#include "oledTask.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "carTask.h"
#include "car.h"
#include "cmsis_os.h"
extern Car car;
uint16_t encoder_count1;
uint16_t encoder_count2;

void StartOledTask(void const * argument){
	while(1){
		
		osDelay(1);
	}
}
