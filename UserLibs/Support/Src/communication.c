#include "communication.h"
#include "car.h"

extern Car car;

uint8_t tx_buffer[BUF_SIZE];
uint8_t rx_data_buffer2[BUF_SIZE];
uint8_t rx_data_buffer6[BUF_SIZE];

void uart_SendMsg(UART_HandleTypeDef *huart, uint8_t* msg){
	uint8_t len = sizeof(msg);
	HAL_UART_Transmit_IT(huart, msg, len);
}

	
void uart_printf(UART_HandleTypeDef *huart, const char *fmt, ...){
	static uint8_t tx_buf[256] = {0};
	static va_list ap;
	static uint16_t len;
	
	va_start(ap, fmt);
	
	len = vsprintf((char *)tx_buf, fmt, ap);
	
	va_end(ap);
	
	HAL_UART_Transmit_IT(huart, tx_buf, len);
}

void UART_IdleCallback(UART_HandleTypeDef* huart){
	if(huart->Instance == USART2){
		HAL_UART_DMAStop(huart);
		
		uint16_t rx_len = huart->RxXferSize - huart->RxXferCount;
//		uart_printf(&huart_pc, "PC msg return: %s\n",rx_data_buffer2);
		memset(rx_data_buffer2,0,BUF_SIZE);
		
		HAL_UART_Receive_DMA(&huart_pc, rx_data_buffer2, BUF_SIZE);
	}
	else if(huart->Instance == USART6){
		HAL_UART_DMAStop(huart);
		
		uint16_t rx_len = huart->RxXferSize - huart->RxXferCount;

		if(!car.isBarrier)
			car.cmd = rx_data_buffer6[0];
		
//		uart_printf(&huart_pc,"%d", car.cmd);
		memset(rx_data_buffer6,0,BUF_SIZE);
		
		HAL_UART_Receive_DMA(&huart_bt, rx_data_buffer6, BUF_SIZE);
	}
}

void USART2_IRQHandler(void){
  HAL_UART_IRQHandler(&huart2);

	if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE)){
    __HAL_UART_CLEAR_IDLEFLAG(&huart2);
		
    UART_IdleCallback(&huart2);
  }
}

void USART6_IRQHandler(void){
  HAL_UART_IRQHandler(&huart6);
	
	if(__HAL_UART_GET_FLAG(&huart6, UART_FLAG_IDLE)){
		__HAL_UART_CLEAR_IDLEFLAG(&huart6);
		
		UART_IdleCallback(&huart6);
	}
}
