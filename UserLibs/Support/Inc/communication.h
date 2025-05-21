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

void uart_printf(UART_HandleTypeDef *huart, const char *fmt, ...);

#endif
