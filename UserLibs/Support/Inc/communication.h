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

#define MSG_HEADER 	 0xAA
#define MSG_TAIL	 0xFF
#define MSG_LEN		 3
#define CMD_LEFT     0xC1
#define CMD_RIGHT    0xC2
#define CMD_FORWARD  0xC3
#define CMD_BACKWARD 0xC4
#define CMD_STOP     0xC5

typedef union{
	float data;
	uint8_t bytes[4];
}fp32;

typedef struct{
	uint8_t header;
	uint8_t cmd_id;
	uint8_t tail;
}prams_msg_t;

void uart_SendMsg(UART_HandleTypeDef *huart, uint8_t* msg);
void uart_printf(UART_HandleTypeDef *huart, const char *fmt, ...);

#endif
