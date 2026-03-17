/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dac.h"
#include "sonar.h"
#include "gpio.h"
#include "config.h"

// ADC1 is used for Turbidity Photo Detector, Turbidity LED voltage, wiper motor
// current, and wiper motor voltage
//
//
// ADC2 is dedicated to the SONAR reception
//
// The above assignments are strictly software configured and can be reconfigured
// if necessary.
//
// Note that hardware oversampling is enabled on both ADC's.  The hardware
// also implements a right-shift to scale the result to the same
// value as if the oversampling was not used.  No additional processing is required
// however the timing is affected since more samples are being acquired for each
// reading.  This is only important for timed sequences such as the SONAR pulse
// measurements on ADC2

// LPTIM1 is used to create a sampling trigger for ADC2.  The
// HAL_LPTIM_IC_CaptureCallback() routine is used to start an ADC2 sample.
//
// The HAL_ADC_ConvCpltCallback() routing is used to move the sample into memory
// and to change the DAC for gain control if necessary.

/**
 * @brief There are multiple uses for ADC1.  This variable keeps track of
 * the current usage of the ADC to avoid changing it during async readings
 */
static ADC_TYPE_E		ADC_config = ADC_UNCFG;

// Defined in tim.c
extern TIM_HandleTypeDef	htim4;

// Defined in tim.c
extern TIM_HandleTypeDef	htim5;

// Defined in dac.c
extern DAC_HandleTypeDef 	hdac1;

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.SamplingMode = ADC_SAMPLING_MODE_NORMAL;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = ENABLE;
  hadc1.Init.Oversampling.Ratio = ADC_OVERSAMPLING_RATIO_256;
  hadc1.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_8;
  hadc1.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  hadc1.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}
/* ADC2 init function */
void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.SamplingMode = ADC_SAMPLING_MODE_BULB;
  hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc2.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  for( int i = 0; i < SONAR_SAMPLES; i++ )
  {
     SONAR_sample_data[i].time = 0xDEAD;
     SONAR_sample_data[i].sample = 0xC0DE;
  }

  /* USER CODE END ADC2_Init 2 */

}

static uint32_t HAL_RCC_ADC_CLK_ENABLED=0;

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC;
    PeriphClkInitStruct.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* ADC1 clock enable */
    HAL_RCC_ADC_CLK_ENABLED++;
    if(HAL_RCC_ADC_CLK_ENABLED==1){
      __HAL_RCC_ADC_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_INP0
    PA1     ------> ADC1_INP1
    PA2     ------> ADC1_INP14
    PA6     ------> ADC1_INP3
    PB1     ------> ADC1_INP5
    */
    GPIO_InitStruct.Pin = RXECHO_Pin|TURBIDITY_IN_Pin|LED_V_Pin|MOTOR_I_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MOTOR_V_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MOTOR_V_GPIO_Port, &GPIO_InitStruct);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

#define ADC_CALIBRATION_SPIN_MAX		1000

    HAL_StatusTypeDef		val;

    // Need to indicate no ADC conversions in progress
    adc_release();

    val = HAL_ADCEx_Calibration_Start( &hadc1, ADC_SINGLE_ENDED );
    if( val != HAL_OK )
    {
       printf( "HAL_ADCEx_Calibration_Start() failed in %s at %d\r\n",
 		__FILE__, __LINE__ );
       Error_Handler();
    }

  /* USER CODE END ADC1_MspInit 1 */
  }
  else if(adcHandle->Instance==ADC2)
  {
  /* USER CODE BEGIN ADC2_MspInit 0 */

  /* USER CODE END ADC2_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC;
    PeriphClkInitStruct.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* ADC2 clock enable */
    HAL_RCC_ADC_CLK_ENABLED++;
    if(HAL_RCC_ADC_CLK_ENABLED==1){
      __HAL_RCC_ADC_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC2 GPIO Configuration
    PA0     ------> ADC2_INP0
    */
    GPIO_InitStruct.Pin = RXECHO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(RXECHO_GPIO_Port, &GPIO_InitStruct);

    /* ADC2 interrupt Init */
    HAL_NVIC_SetPriority(ADC2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC2_IRQn);
  /* USER CODE BEGIN ADC2_MspInit 1 */


    HAL_StatusTypeDef		val;

    val = HAL_ADCEx_Calibration_Start( &hadc2, ADC_SINGLE_ENDED );
    if( val != HAL_OK )
    {
       printf( "HAL_ADCEx_Calibration_Start() failed in %s at %d\r\n",
 		__FILE__, __LINE__ );
       Error_Handler();
    }

  /* USER CODE END ADC2_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_ADC_CLK_ENABLED--;
    if(HAL_RCC_ADC_CLK_ENABLED==0){
      __HAL_RCC_ADC_CLK_DISABLE();
    }

    /**ADC1 GPIO Configuration
    PA0     ------> ADC1_INP0
    PA1     ------> ADC1_INP1
    PA2     ------> ADC1_INP14
    PA6     ------> ADC1_INP3
    PB1     ------> ADC1_INP5
    */
    HAL_GPIO_DeInit(GPIOA, RXECHO_Pin|TURBIDITY_IN_Pin|LED_V_Pin|MOTOR_I_Pin);

    HAL_GPIO_DeInit(MOTOR_V_GPIO_Port, MOTOR_V_Pin);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC1_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
  else if(adcHandle->Instance==ADC2)
  {
  /* USER CODE BEGIN ADC2_MspDeInit 0 */

  /* USER CODE END ADC2_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_ADC_CLK_ENABLED--;
    if(HAL_RCC_ADC_CLK_ENABLED==0){
      __HAL_RCC_ADC_CLK_DISABLE();
    }

    /**ADC2 GPIO Configuration
    PA0     ------> ADC2_INP0
    */
    HAL_GPIO_DeInit(RXECHO_GPIO_Port, RXECHO_Pin);

    /* ADC2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC2_IRQn);
  /* USER CODE BEGIN ADC2_MspDeInit 1 */

  /* USER CODE END ADC2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
 ***********************************************************************
 * @brief This routine forces the system control variable for the ADC
 * to the unconfigured state with no error checks.
 *
 * @param none
 *
 * @return none
 *
 ***********************************************************************
 */
void adc_release( void )
{
   ADC_config = ADC_UNCFG;

   return;
} // end of adc_release()

/**
 ***********************************************************************
 * @brief This routine sets the ADC system to the requested channel
 *
 * @param t -- requested channel
 *
 * @return result status
 *
 ***********************************************************************
 */
HAL_StatusTypeDef adc_select_channel( ADC_TYPE_E t )
{
   HAL_StatusTypeDef		val;
   ADC_ChannelConfTypeDef 	sConfig = {0};
   ADC_HandleTypeDef		*handle;



   // Assume something is wrong
   val = HAL_ERROR;

   handle = NULL;

   if( ADC_config != ADC_UNCFG )
   {
      // The ADC is already configured.  The adc_release() function
      // should have been called before this function.
      val = HAL_ERROR;
   }
   else
   {
      sConfig.Rank = ADC_REGULAR_RANK_1;
      sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
      sConfig.SingleDiff = ADC_SINGLE_ENDED;
      sConfig.OffsetNumber = ADC_OFFSET_NONE;
      sConfig.Offset = 0;

      switch( t )
      {
	 case ADC_TURB_RCV:
	    sConfig.Channel = ADC_TURB_RCV_CH;
	    handle = &hadc1;
	    break;

	 case ADC_TURB_PWR:
	    sConfig.Channel = ADC_TURB_PWR_CH;
	    handle = &hadc1;
	    break;

	 case ADC_WIPER_I:
	    sConfig.Channel = ADC_WIPER_I_CH;
	    handle = &hadc1;
	    break;

	 case ADC_WIPER_V:
	    sConfig.Channel = ADC_WIPER_V_CH;
	    handle = &hadc1;
	    break;

	 case ADC_SONAR:
	    sConfig.Channel = ADC_SONAR_CH;
	    handle = &hadc2;
	    break;

	 case ADC_UNCFG:
	    asm("nop");	// NOP
	    break;

	 default:
	    // Should never get here
	    printf("Critical error in %s at %d\r\n", __FILE__, __LINE__ );
	    Error_Handler();
	    break;
      } // end of switch(t)
   }

   if( handle )
   {
      val = HAL_ADC_ConfigChannel( handle, &sConfig );
      if( val != HAL_OK )
      {
	 printf( "HAL_ADC_ConfigChannel() failed in %s at %d\r\n",
		  __FILE__, __LINE__ );
	 Error_Handler();
      }
   }

   return( val );
} // end of adc_selet_channel()

/**
 ***********************************************************************
 * @brief This routine handles asynchronous errors from the ADC hardware
 *
 * @param hadc - handle of the affected adc
 *
 * @return none
 *
 ***********************************************************************
 */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
   if( hadc->Instance == ADC1 )
   {
      // Problem with Turbidity or wiper
   }
   else if( hadc->Instance == ADC2 )
   {
      // Problem with SONAR
   }
   else
   {
      // Truly unknown problem
      printf("Critical error in %s at %d\r\n", __FILE__, __LINE__ );
      Error_Handler();
   }

   return;
} // HAL_ADC_ErrorCallback()

/**
 ***********************************************************************
 * @brief This routine gets one reading from the specified interface
 *
 * @param t - type of reading
 *
 * @param stopWhenDone - true if the ADC should be turned off after
 * one reading
 *
 * @return 12-bit value right justified on success, -1 on error
 *
 ***********************************************************************
 */
int32_t
   adc_get_reading(
      ADC_TYPE_E 		t,
      bool			stopWhenDone )
{

   int32_t		reading;
   HAL_StatusTypeDef	ret;
   ADC_HandleTypeDef	*handle;
   //uint32_t		startTick;
   //uint32_t		stopTick;


   ret = adc_select_channel( t );
   if( ret != HAL_OK )
   {
      printf("adc_select_channel() failed in %s and line %d with code %d\r\n",
	       __FILE__, __LINE__, ret );
      reading = -1;
   }
   else
   {
      reading = 1;
      if( t == ADC_SONAR )
      {
	 handle = &hadc2;
      }
      else
      {
	 handle = &hadc1;
      }
   }

   if( reading > 0 )
   {
      ret = HAL_ADC_Start( handle );
      if( ret != HAL_OK )
      {
	 printf( "HAL_ADC_Start() failed in %s at %d with code %d\r\n",
		  __FILE__, __LINE__, ret );
      }
      else
      {
	 //startTick = HAL_GetTick();
	 ret = HAL_ADC_PollForConversion( handle, 1000 );	// 1 second timeout
	 if( ret != HAL_OK )
	 {
	    printf( "HAL_ADC_PollForConversion() failed in %s at %d with code %d\r\n",
		     __FILE__, __LINE__, ret );
	 }
	 else
	 {
	    //stopTick = HAL_GetTick();
	    reading = HAL_ADC_GetValue( handle );
	 }
      }
      if( stopWhenDone )
      {
	 ret = HAL_ADC_Stop( handle );
	 if( ret != HAL_OK )
	 {
	    printf( "HAL_ADC_Stop() failed in %s at %d with code %d\r\n",
		     __FILE__, __LINE__, ret );
	 }
      }
   }

   //printf( "It took %ld ticks to get a conversion done.\r\n", (stopTick-startTick) );
   //fflush( stdout );

   return( reading );
} // end of adc_get_reading()

/**
 ***********************************************************************
 * @brief This ISR is called at interrupt level at the end of an ADC
 * conversion
 *
 * @param hadc	ADC handle
 *
 * @return none
 *
 ***********************************************************************
 */
void __attribute__((optimize("-Ofast"))) HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
   uint32_t		reading;

   if( hadc->Instance == ADC1 )
   {
      // Handle conversion complete for ADC_TURB_RCV, ADC_TURB_PWR,
      // ADC_WIPER_I, ADC_WIPER_V
   }
   else if( hadc->Instance == ADC2 )
   {

      // Handle conversion complete for ADC_SONAR
      if( SONAR_sample_index > SONAR_SAMPLES )
      {
	 Error_Handler();	// should never happen
      }

      // Save last sample
      reading = HAL_ADC_GetValue( hadc );
      SONAR_sample_data[ SONAR_sample_index ].time = htim5.Instance->CNT;
      SONAR_sample_data[ SONAR_sample_index ].sample = reading;

      SONAR_sample_index++;

      if( SONAR_sample_index > SONAR_SAMPLES )
      {
	 // Done
	 HAL_TIM_Base_Stop_IT( &htim4 );
	 HAL_TIM_Base_Stop( &htim5 );

	 SONAR_state = SONAR_IDLE;
      }
      else
      {
	 // Handle update of DAC to control programmable amplifier
      }
   }

   return;
} // end of HAL_ADC_ConvCpltCallback()

/**
  * @brief  Fast start of a single ADC conversion with interrupt on conversion complete,
  * based on HAL_ADC_Start_IT() but faster since it was taking too long to convert a sample
  *
  * @note   Interruptions enabled in this function according to initialization
  *         setting : EOC (end of conversion), EOS (end of sequence),
  *         OVR overrun.
  *         Each of these interruptions has its dedicated callback function.
  * @note   Case of multimode enabled (when multimode feature is available):
  *         HAL_ADC_Start_IT() must be called for ADC Slave first, then for
  *         ADC Master.
  *         For ADC Slave, ADC is enabled only (conversion is not started).
  *         For ADC Master, ADC is enabled and multimode conversion is started.
  * @note   To guarantee a proper reset of all interruptions once all the needed
  *         conversions are obtained, HAL_ADC_Stop_IT() must be called to ensure
  *         a correct stop of the IT-based conversions.
  * @note   By default, HAL_ADC_Start_IT() does not enable the End Of Sampling
  *         interruption. If required (e.g. in case of oversampling with trigger
  *         mode), the user must:
  *          1. first clear the EOSMP flag if set with macro __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOSMP)
  *          2. then enable the EOSMP interrupt with macro __HAL_ADC_ENABLE_IT(hadc, ADC_IT_EOSMP)
  *          before calling HAL_ADC_Start_IT().
  * @param hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef nonHAL_ADC_Start_IT(ADC_HandleTypeDef *hadc)
{
   HAL_StatusTypeDef 		tmp_hal_status;

   /* Perform ADC enable and conversion start if no conversion is on going */
   //if (LL_ADC_REG_IsConversionOngoing(hadc->Instance) == 0UL)
   {
      tmp_hal_status = HAL_OK;

      /* Process locked */
      __HAL_LOCK(hadc);

      /* Enable the ADC peripheral */
      LL_ADC_Enable(hadc->Instance);

      /* Set ADC state                                                        */
      /* - Clear state bitfield related to regular group conversion results   */
      /* - Set state bitfield related to regular operation                    */
      ADC_STATE_CLR_SET(hadc->State,
			HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
			HAL_ADC_STATE_REG_BUSY);

      /* Clear ADC group regular conversion flag and overrun flag               */
      /* (To ensure of no unknown state from potential previous ADC operations) */
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));

      /* Process unlocked */
      /* Unlock before starting ADC conversions: in case of potential         */
      /* interruption, to let the process to ADC IRQ Handler.                 */
      __HAL_UNLOCK(hadc);

      __HAL_ADC_ENABLE_IT(hadc, ADC_IT_EOC);

      /* Enable conversion of regular group.                                  */
      /* If software start has been selected, conversion starts immediately.  */

      /* Start ADC group regular conversion */
      LL_ADC_REG_StartConversion(hadc->Instance);

      /* Process unlocked */
      __HAL_UNLOCK(hadc);
   }
//   else
//   {
//      tmp_hal_status = HAL_BUSY;
//   }

   /* Return function status */
   return tmp_hal_status;
} // end of nonHAL_ADC_Start_IT()

/**
 ***********************************************************************
 * @brief This ISR is called for all ADC2 interrupts
  *
 * @param none
 *
 * @return none
 *
 ***********************************************************************
 */
void ADC2_IRQHandler(void)
{
   uint32_t		reading;


   // Switch from High level function calls to a quick direct write
   // Set is off
   //nLED_GPIO_Port->BSRR = nLED_Pin;

   // Handle conversion complete for ADC_SONAR
   if( SONAR_sample_index > SONAR_SAMPLES )
   {
      Error_Handler();	// should never happen
   }

   // Save last sample
   reading = ADC2->DR;	// Reading DR register also clears the interrupt flag

   SONAR_sample_data[ SONAR_sample_index ].time = htim5.Instance->CNT;
   SONAR_sample_data[ SONAR_sample_index ].sample = reading;
   SONAR_sample_index++;

   if( SONAR_sample_index > SONAR_SAMPLES )
   {
      // Done
      HAL_TIM_Base_Stop_IT( &htim4 );
      HAL_TIM_Base_Stop( &htim5 );

      SONAR_state = SONAR_IDLE;
   }
   else
   {
      // Handle update of DAC to control programmable amplifier
      int16_t			computedGain;
      HAL_StatusTypeDef		retVal;

      computedGain = SONAR_sample_index >> GAIN_TABLE_SHIFT_INDEX[ SSP_configuration.range ];

      if( computedGain >= (SONAR_SAMPLES/16) )
      {
	 computedGain = SONAR_SAMPLES/16 - 1;
	 if( computedGain > 3 )
	 {
	    computedGain = RX_GAIN_TABLE[computedGain] + SSP_configuration.gainAdjust;
	    computedGain &= DAC_MAX_VALUE;
	 }
	 retVal = HAL_DAC_SetValue( &hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, computedGain );
	 if( retVal != HAL_OK )
	 {
	    printf( "HAL_DAC_SetValue() failed in %s at line %d with error %d\r\n",
		     __FILE__, __LINE__, retVal );
	 }
	 if( computedGain > DAC_MAX_VALUE )
	 {
	    asm("nop");
	 }
      }
   }


   return;
} // end of ADC2_IRQHandler()

/* USER CODE END 1 */
