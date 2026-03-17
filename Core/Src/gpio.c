/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include <stdio.h>
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "adc.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
     PB2   ------> S_LPTIM1_CH1
     PA13(JTMS/SWDIO)   ------> DEBUG_JTMS-SWDIO
     PA14(JTCK/SWCLK)   ------> DEBUG_JTCK-SWCLK
     PA15(JTDI)   ------> DEBUG_JTDI
     PB3(JTDO/TRACESWO)   ------> DEBUG_JTDO-SWO
     PB4(NJTRST)   ------> DEBUG_NJTRST
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(EBLRX_GPIO_Port, EBLRX_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(T_LED_GPIO_Port, T_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, nLED_Pin|RS485_RE_Pin|WIPER_MOTOR_Pin|nSTATUS_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : EBLRX_Pin */
  GPIO_InitStruct.Pin = EBLRX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EBLRX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ACCEL_INT1_Pin ACCEL_INT2_Pin */
  GPIO_InitStruct.Pin = ACCEL_INT1_Pin|ACCEL_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : T_LED_Pin */
  GPIO_InitStruct.Pin = T_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(T_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : nLED_Pin RS485_RE_Pin WIPER_MOTOR_Pin nSTATUS_LED_Pin */
  GPIO_InitStruct.Pin = nLED_Pin|RS485_RE_Pin|WIPER_MOTOR_Pin|nSTATUS_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_LPTIM1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ADC_TRIG_IN_Pin */
  GPIO_InitStruct.Pin = ADC_TRIG_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADC_TRIG_IN_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

#define	USE_HW_DIRECT
/*
 **********************************************************************
 * @brief GPIO_led_off() turns off the debug LED
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void GPIO_led_off( void )
{
#ifdef USE_HW_DIRECT
      nLED_GPIO_Port->BSRR = nLED_Pin;
#else
   HAL_GPIO_WritePin(
	    nLED_GPIO_Port,
	    nLED_Pin,
	    GPIO_PIN_SET);
#endif
   return;
} // end of GPIO_led_off()

/*
 **********************************************************************
 * @brief GPIO_led_on() turns on the debug LED
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void GPIO_led_on( void )
{
#ifdef USE_HW_DIRECT
   nLED_GPIO_Port->BSRR = nLED_Pin << 16;
#else
   HAL_GPIO_WritePin(
	    nLED_GPIO_Port,
	    nLED_Pin,
	    GPIO_PIN_RESET);
#endif
   return;
} // end of GPIO_led_on()

/*
 **********************************************************************
 * @brief GPIO_led_toggle() inverts the debug LED
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void GPIO_led_toggle( void )
{
   HAL_GPIO_TogglePin(
	    nLED_GPIO_Port,
	    nLED_Pin );

   return;
} // end of GPIO_led_toggle()

/*
 **********************************************************************
 * @brief GPIO_status_led_off() turns off the status LED
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void GPIO_status_led_off( void )
{
#ifdef USE_HW_DIRECT
   nSTATUS_LED_GPIO_Port->BSRR = nSTATUS_LED_Pin;
#else
   HAL_GPIO_WritePin(
	    nSTATUS_LED_GPIO_Port,
	    nSTATUS_LED_Pin,
	    GPIO_PIN_SET);
#endif
   return;
} // end of GPIO_status_led_off()

/*
 **********************************************************************
 * @brief GPIO_status_led_toggle() inverts the status LED
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void GPIO_status_led_toggle( void )
{
   HAL_GPIO_TogglePin(
	    nSTATUS_LED_GPIO_Port,
	    nSTATUS_LED_Pin );
   return;
} // end of GPIO_status_led_toggle()

/*
 **********************************************************************
 * @brief GPIO_status_led_on() turns on the status LED
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void GPIO_status_led_on( void )
{
#ifdef USE_HW_DIRECT
   nSTATUS_LED_GPIO_Port->BSRR = nSTATUS_LED_Pin << 16;
#else
   HAL_GPIO_WritePin(
	    nSTATUS_LED_GPIO_Port,
	    nSTATUS_LED_Pin,
	    GPIO_PIN_RESET);
#endif
   return;
} // end of GPIO_status_led_on()

/*
 **********************************************************************
 * @brief GPIO_turbidity_led_off() turns off the turbidity LED
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void GPIO_turbidity_led_off( void )
{
#ifdef USE_HW_DIRECT
   T_LED_GPIO_Port->BSRR = T_LED_Pin << 16;
#else
   HAL_GPIO_WritePin(
	    T_LED_GPIO_Port,
	    T_LED_Pin,
	    GPIO_PIN_RESET);
#endif
   return;
} // end of GPIO_turbidity_led_off()

/*
 **********************************************************************
 * @brief GPIO_turbidity_led_toggle() inverts the turbidity LED
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void GPIO_turbidity_led_toggle( void )
{
   HAL_GPIO_TogglePin(
	    T_LED_GPIO_Port,
	    T_LED_Pin );

   return;
} // end of GPIO_turbidity_led_toggle()

/*
 **********************************************************************
 * @brief GPIO_turbidity_led_on() turns on the turbidity LED
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void GPIO_turbidity_led_on( void )
{
#ifdef USE_HW_DIRECT
   T_LED_GPIO_Port->BSRR = T_LED_Pin;
#else
   HAL_GPIO_WritePin(
	    T_LED_GPIO_Port,
	    T_LED_Pin,
	    GPIO_PIN_SET);
#endif
   return;
} // end of GPIO_turbidity_led_on()

/*
 **********************************************************************
 * @brief GPIO_wiper_on() turns on the wiper motor
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void GPIO_wiper_on( void )
{
#ifdef USE_HW_DIRECT
   WIPER_MOTOR_GPIO_Port->BSRR = WIPER_MOTOR_Pin;
#else
   HAL_GPIO_WritePin(
	    WIPER_MOTOR_GPIO_Port,
	    WIPER_MOTOR_Pin,
	    GPIO_PIN_SET);
#endif
   return;
} // end of GPIO_wiper_on()


/*
 **********************************************************************
 * @brief GPIO_wiper_off() turns off the wiper motor
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void GPIO_wiper_off( void )
{
#ifdef USE_HW_DIRECT
   WIPER_MOTOR_GPIO_Port->BSRR = WIPER_MOTOR_Pin << 16;
#else
   HAL_GPIO_WritePin(
	    WIPER_MOTOR_GPIO_Port,
	    WIPER_MOTOR_Pin,
	    GPIO_PIN_RESET);
#endif
   return;
} // end of GPIO_wiper_off()

/*
 **********************************************************************
 * @brief spin_wiper() spin wiper the given number of times
 *
 * @param revs - number of times to spin wiper
 *
 * @return none
 **********************************************************************
 */
void spin_wiper( uint32_t revs )
{
   uint32_t	i;

    // Start counting revolutions
   for( i = 0; i < revs; i++ )
   {
      GPIO_wiper_on();
      HAL_Delay( 100 );
      wiper_home();
   }

   return;
} // end of spin_wiper()

/*
 **********************************************************************
 * @brief blip_wiper() turn the wiper a little bit
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
#define		BLIP_WIPER_MS	200
void blip_wiper( void )
{


   GPIO_wiper_on();

   HAL_Delay( BLIP_WIPER_MS );

   GPIO_wiper_off();
   return;
} // end of blip_wiper()

/*
 **********************************************************************
 * @brief wiper_home() sends wiper to home position
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
#define		WIPER_HOME_DETECT_DIFF	0xE10
#define		WIPER_HOME_DELAY_MS	600

void wiper_home( void )
{
   int32_t	reading;

   GPIO_wiper_on();

   reading = adc_get_reading( ADC_TURB_RCV, false );

   // Look for wiper passing over reed switch
   while( reading < WIPER_HOME_DETECT_DIFF )
   {
      reading = adc_get_reading( ADC_TURB_RCV, false );
   }

   // Let wiper coast to home position
   HAL_Delay( WIPER_HOME_DELAY_MS );
   GPIO_wiper_off();

   return;
} // end of wiper_home()


/* USER CODE END 2 */
