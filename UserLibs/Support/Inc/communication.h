#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "main.h"
#include "usart.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define huart_pc huart2
#define huart_bt huart6
#define BUF_SIZE 64

typedef union{
	float data;
	uint8_t bytes[4];
}fp32;

typedef struct{
	uint8_t header;
	
	fp32 pid_a_p;
	fp32 pid_a_i;
	fp32 pid_a_d;
	
	fp32 pid_l_p;
	fp32 pid_l_i;
	fp32 pid_l_d;
	
	fp32 pid_r_p;
	fp32 pid_r_i;
	fp32 pid_r_d;
	
	uint8_t tail;
}prams_msg_t;

void uart_SendMsg(UART_HandleTypeDef *huart, uint8_t* msg);
void uart_printf(UART_HandleTypeDef *huart, const char *fmt, ...);

#endif
