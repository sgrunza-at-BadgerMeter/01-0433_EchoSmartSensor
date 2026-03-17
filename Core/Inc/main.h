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
#include "stm32h5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdbool.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

// Define the maximum length of a serial console message
#define MAX_CONSOLE_MSG_LEN		80

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EBLRX_Pin GPIO_PIN_13
#define EBLRX_GPIO_Port GPIOC
#define ACCEL_INT1_Pin GPIO_PIN_14
#define ACCEL_INT1_GPIO_Port GPIOC
#define ACCEL_INT2_Pin GPIO_PIN_15
#define ACCEL_INT2_GPIO_Port GPIOC
#define RXECHO_Pin GPIO_PIN_0
#define RXECHO_GPIO_Port GPIOA
#define TURBIDITY_IN_Pin GPIO_PIN_1
#define TURBIDITY_IN_GPIO_Port GPIOA
#define LED_V_Pin GPIO_PIN_2
#define LED_V_GPIO_Port GPIOA
#define LOOP1_DAC_Pin GPIO_PIN_3
#define LOOP1_DAC_GPIO_Port GPIOA
#define RXGAIN_Pin GPIO_PIN_4
#define RXGAIN_GPIO_Port GPIOA
#define T_LED_Pin GPIO_PIN_5
#define T_LED_GPIO_Port GPIOA
#define MOTOR_I_Pin GPIO_PIN_6
#define MOTOR_I_GPIO_Port GPIOA
#define TXPULSE_A_Pin GPIO_PIN_7
#define TXPULSE_A_GPIO_Port GPIOA
#define nLED_Pin GPIO_PIN_0
#define nLED_GPIO_Port GPIOB
#define MOTOR_V_Pin GPIO_PIN_1
#define MOTOR_V_GPIO_Port GPIOB
#define LOOP2_DAC_Pin GPIO_PIN_12
#define LOOP2_DAC_GPIO_Port GPIOB
#define RS485_RE_Pin GPIO_PIN_13
#define RS485_RE_GPIO_Port GPIOB
#define RS485_DE_Pin GPIO_PIN_14
#define RS485_DE_GPIO_Port GPIOB
#define WIPER_MOTOR_Pin GPIO_PIN_15
#define WIPER_MOTOR_GPIO_Port GPIOB
#define TXPULSE_B_Pin GPIO_PIN_8
#define TXPULSE_B_GPIO_Port GPIOA
#define ADC_TRIG_IN_Pin GPIO_PIN_11
#define ADC_TRIG_IN_GPIO_Port GPIOA
#define RS485_TX_Pin GPIO_PIN_12
#define RS485_TX_GPIO_Port GPIOA
#define nSTATUS_LED_Pin GPIO_PIN_5
#define nSTATUS_LED_GPIO_Port GPIOB
#define ACCEL_SCL_Pin GPIO_PIN_6
#define ACCEL_SCL_GPIO_Port GPIOB
#define ACCEL_SDA_Pin GPIO_PIN_7
#define ACCEL_SDA_GPIO_Port GPIOB
#define RS485_RX_Pin GPIO_PIN_8
#define RS485_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

void display_last_reset_reason( void );

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
