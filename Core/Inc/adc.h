/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

extern ADC_HandleTypeDef hadc2;

/* USER CODE BEGIN Private defines */

/**
 * @brief This structure enumerates the different uses of ADC1
 */
typedef enum ADC_TYPE_E
{
   ADC_TURB_RCV = 1,	///< turbidity photo detector, also wiper reed switch
   ADC_TURB_PWR,	///< turbidity LED driver voltage
   ADC_WIPER_I,		///< wiper motor current when motor is active
   ADC_WIPER_V,		///< wiper motor voltage when motor is off
   ADC_SONAR,		///< RX_ECHO from ADC2
   ADC_UNCFG		///< ADC1 is not configured or not in use
} ADC_TYPE_E;

/**
 * @brief These definitions help map the conversion source to  the actual
 * hardware channel
 */
#define ADC_TURB_RCV_CH		ADC_CHANNEL_1
#define ADC_TURB_PWR_CH		ADC_CHANNEL_14
#define ADC_WIPER_I_CH		ADC_CHANNEL_3
#define ADC_WIPER_V_CH		ADC_CHANNEL_5
#define ADC_SONAR_CH		ADC_CHANNEL_0

#define ADC_MAX_VALUE		0x0FFF
#define ADC_MAX_VOLTAGE		3.3
#define ADC_MOTOR_SENSE_VALUE	(0.1)

/* USER CODE END Private defines */

void MX_ADC1_Init(void);
void MX_ADC2_Init(void);

/* USER CODE BEGIN Prototypes */
HAL_StatusTypeDef adc_select_channel( ADC_TYPE_E t );
void adc_release( void );
int32_t
   adc_get_reading(
      ADC_TYPE_E 		t,
      bool			stopWhenDone );

HAL_StatusTypeDef nonHAL_ADC_Start_IT(ADC_HandleTypeDef *hadc);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

