/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

// Stop wiper when the ADC value from the detector voltage is greater 2.9V
#define	WIPER_END_VOLTAGE	2.9

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

void GPIO_led_off( void );
void GPIO_led_on( void );
void GPIO_led_toggle( void );

void GPIO_status_led_off( void );
void GPIO_status_led_on( void );
void GPIO_status_led_toggle( void );

void GPIO_turbidity_led_off( void );
void GPIO_turbidity_led_on( void );
void GPIO_turbidity_led_toggle( void );

void GPIO_wiper_off( void );
void GPIO_wiper_on( void );

void spin_wiper( uint32_t revs );
void wiper_home( void );
void blip_wiper( void );


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

