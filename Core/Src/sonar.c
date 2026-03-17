/**
 **********************************************************************
 * @file sonar.c
 *
 * @brief This file holds the code to send a sonar ping and receive
 * the echo
 *
 * @author grunzas
 *
 * @date Nov 11, 2025
 *
 **********************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "main.h"
#include "adc.h"

#define SRC_SONAR_C_

#include "sonar.h"

#define MSG_BUFSZ	80
char	msg[ MSG_BUFSZ ];

// Defined in tim.c
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

// Defined in adc.c
extern ADC_HandleTypeDef hadc2;

// Defined in usart.c
extern UART_HandleTypeDef huart1;

/*
 **********************************************************************
 * @brief Create a ping and listen to the result
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 **********************************************************************
 */
void ping( char *c )
{

   HAL_StatusTypeDef	ret;

   uint32_t		startTick;
   uint32_t		stopTick;
   uint32_t		currentTick;
   bool			timeOut;
   SONAR_STATS_T	stats;


   SONAR_state = SONAR_PING_ACTIVE;

   SONAR_sample_index = 0;

   timeOut = false;

   // Setup ADC to receive sonar signals
   adc_select_channel( ADC_SONAR );

   // Setup TIM1_CH1 (PA8) TXPULSE_B and TIM1_CH1N (PA7) TXPULSE_A for Sonar output pulse transformer
   HAL_TIM_PWM_Start( &htim1, TIM_CHANNEL_1 );
   HAL_TIMEx_PWMN_Start( &htim1, TIM_CHANNEL_1 );

   // Enable TIM1 Update interrupt
   __HAL_TIM_ENABLE_IT( &htim1, TIM_IT_UPDATE);


   while( SONAR_state == SONAR_PING_ACTIVE )
   {
      asm( "nop" );	// NOP
   }

   // Finished sending ping
   TIM1->CR1 &= ~TIM_CR1_CEN;	// The __HAL_TIM_DISABLE disable call is kind of large
   __HAL_TIM_DISABLE_IT( &htim1, TIM_IT_UPDATE);

   // Now start listening

   // Setup TIM5 to monitor sample timing
   htim5.Instance->CNT = 0;	// start counter at zero

   ret = HAL_TIM_Base_Start( &htim5 );
   if( ret != HAL_OK )
   {
      printf("HAL_TIM_Base_Start() failed in file %s at line %d\r\n",
	       __FILE__, __LINE__ );
      fflush( stdout );
   }

   // Setup TIM4 so that it can start an ADC sample every 5 us

   ret = HAL_TIM_Base_Start_IT( &htim4 );
   if( ret != HAL_OK )
   {
      printf("HAL_TIM_Base_Start_IT() failed in file %s at line %d\r\n",
	       __FILE__, __LINE__ );
      fflush( stdout );
   }

   startTick = HAL_GetTick();
   stopTick = startTick + (10 * 1000); 	// 10 second timeout

   while( SONAR_sample_index < SONAR_SAMPLES )
   {
      //asm( "nop" );	// NOP

      currentTick = HAL_GetTick();
      if( currentTick > stopTick )
      {
	 // Timeout
	 timeOut = true;
	 break;
      }
   }

   if( timeOut )
   {
      printf( "timeout while waiting for %d samples.  Only got %d\r\n",
	       SONAR_SAMPLES, SONAR_sample_index );
      fflush( stdout );

      HAL_TIM_Base_Stop_IT( &htim4 );
      HAL_TIM_Base_Stop( &htim5 );
   }

   stopTick = HAL_GetTick();

   snprintf( msg, MSG_BUFSZ, "ping done\r\n" );
   HAL_UART_Transmit( &huart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY );

   snprintf( msg, MSG_BUFSZ, "Tick,\tADC counts\r\n" );
   HAL_UART_Transmit( &huart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY );

   sonar_calc_stats( SONAR_sample_data, SONAR_SAMPLES, &stats );

   for( int i = 0; i < SONAR_SAMPLES; i++ )
   {
      snprintf( msg, MSG_BUFSZ, "%4.4ld,\t%4.4d\r\n",
	       SONAR_sample_data[i].time,
	       SONAR_sample_data[i].sample );
      HAL_UART_Transmit( &huart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY );
   }

   printf(" Sonar Stats:\r\n" );
   printf(" \tmax     = %d (0x%x)\r\n", stats.max, (uint16_t)stats.max );
   printf(" \tmin     = %d (0x%x)\r\n", stats.min, (uint16_t)stats.min );
   printf(" \tstdev   = %f\r\n", stats.stdev );
   printf(" \taverage = %f\r\n", stats.average );
   printf("\r\n\n");

   return;
} // end of ping()

/*
 **********************************************************************
 * @brief Calculate some statistics on the given sonar data
 *
 * @param data - pointer to array of sonar data
 *
 * @param len - number of samples in array
 *
 * @param stats - location to which the results should be stored
 *
 * @return none
 **********************************************************************
 */
void
   sonar_calc_stats(
      SONAR_SAMPLE_T 	*data,
      uint16_t 		len,
      SONAR_STATS_T 	*stats )
{

   uint16_t	i;

   uint32_t	average;
   uint32_t	differenceSquaredSum;
   uint32_t	stdev;

   if( (data == NULL) || (stats == NULL) )
   {
      Error_Handler();
   }

   if( len == 0 )
   {
      Error_Handler();
   }

   memset( stats, 0, sizeof(SONAR_STATS_T) );
   stats->min = 32767;

   average = 0;
   for( i = 0; i < len; i++ )
   {
      average += data[i].sample;

      if( stats->max < data[i].sample )
      {
	 stats->max = data[i].sample;
      }

      if( stats->min > data[i].sample )
      {
	 stats->min = data[i].sample;
      }

   }

   average = average / len;
   stats->average = average;

   differenceSquaredSum = 0;
   for( i = 0; i < len; i++ )
   {
      differenceSquaredSum += (data[i].sample - average) * (data[i].sample - average);
   }

   stdev = sqrt(differenceSquaredSum / len);

   stats->stdev = stdev;

   return;
} // end of sonar_calc_stats()

// end of file sonar.c

