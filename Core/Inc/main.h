/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define ROUND3(x)  (((int)((x) * 1000 + 0.5f)) / 1000.0f)
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define STBY_Pin GPIO_PIN_1
#define STBY_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define DIN2_Pin GPIO_PIN_4
#define DIN2_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define E4A_Pin GPIO_PIN_6
#define E4A_GPIO_Port GPIOA
#define E4B_Pin GPIO_PIN_7
#define E4B_GPIO_Port GPIOA
#define DIN1_Pin GPIO_PIN_0
#define DIN1_GPIO_Port GPIOB
#define EHCO_Pin GPIO_PIN_12
#define EHCO_GPIO_Port GPIOB
#define EHCO_EXTI_IRQn EXTI15_10_IRQn
#define BT_STATE_Pin GPIO_PIN_15
#define BT_STATE_GPIO_Port GPIOB
#define BT_STATE_EXTI_IRQn EXTI15_10_IRQn
#define I2C3_SDA_Pin GPIO_PIN_6
#define I2C3_SDA_GPIO_Port GPIOC
#define AIN1_Pin GPIO_PIN_7
#define AIN1_GPIO_Port GPIOC
#define I2C3_SCL_Pin GPIO_PIN_8
#define I2C3_SCL_GPIO_Port GPIOC
#define K4_Pin GPIO_PIN_9
#define K4_GPIO_Port GPIOC
#define K4_EXTI_IRQn EXTI9_5_IRQn
#define E1A_Pin GPIO_PIN_8
#define E1A_GPIO_Port GPIOA
#define E1B_Pin GPIO_PIN_9
#define E1B_GPIO_Port GPIOA
#define TRIG_Pin GPIO_PIN_10
#define TRIG_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define K3_Pin GPIO_PIN_2
#define K3_GPIO_Port GPIOD
#define K3_EXTI_IRQn EXTI2_IRQn
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define K2_Pin GPIO_PIN_4
#define K2_GPIO_Port GPIOB
#define K2_EXTI_IRQn EXTI4_IRQn
#define K1_Pin GPIO_PIN_5
#define K1_GPIO_Port GPIOB
#define K1_EXTI_IRQn EXTI9_5_IRQn
#define AIN2_Pin GPIO_PIN_6
#define AIN2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
