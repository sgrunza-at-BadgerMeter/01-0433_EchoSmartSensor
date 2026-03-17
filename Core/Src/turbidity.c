/**
 **********************************************************************
 * @file turbidity.c
 *
 * @brief This file contains code for the turbidity measurements
 *
 * @author grunzas
 *
 * @date Dec 23, 2025
 *
 **********************************************************************
 */

#include <stdio.h>

#include "main.h"
#include "turbidity.h"
#include "config.h"
#include "gpio.h"

static TURB_DATA_T	turbidity;

//*********************************************************************
TURB_DATA_T	*getTurbidityPtr( void )
{

   return( &turbidity );
} // end of getTurbidityPtr()

//*********************************************************************
void	displayTurbidity( TURB_DATA_T *t )
{

   if( t != NULL )
   {
      printf( "Turbidity data:\r\n");
      printf( "\t average: %5ld\r\n", t->turbAvg );
      printf( "\t prev:    %5ld\r\n", t->turbPrev );
      printf( "\t sum:     %5ld\r\n", t->turbSum );
      printf( "\t ntu:     %5.2lf\r\n", t->ntu );
   }

   return;
} // end of displayTurbidity()

//*********************************************************************
void
   buildAverageTurbidity(
      TURB_DATA_T		*t,
      uint16_t			value )
{

   double		span;
   double		pctUsed;

   if( t != NULL )
   {
      if( t->turbSum == 0 )
      {
	 t->turbSum = value * TURBIDITY_NUM_SAMPLES;
	 t->turbAvg = value;
      }
      else
      {
	 t->turbSum -= t->turbPrev;
	 t->turbSum += value;
      }

      t->turbAvg = t->turbSum / TURBIDITY_NUM_SAMPLES;
      t->turbPrev = value;

      span = SSP_configuration.turb_50_ntu_cal - SSP_configuration.turb_0_ntu_cal;
      pctUsed = (double)(t->turbAvg - SSP_configuration.turb_0_ntu_cal) / span;
      t->ntu = pctUsed * 5000.0;
   }
   else
   {
      // Logic error -- should have gotten a valid pointer
      Error_Handler();
   }


   return;
} // end of buildAverageTurbidity()

/*
 **********************************************************************
 * @brief turbidityInit() initialize hardware and storage
 *
 * @param none
 *
 * @return none
 *
 **********************************************************************
 */
void turbidityInit( void )
{

   // Ensure LED is currently off
   GPIO_turbidity_led_off();

   // TODO:  Finish turbidity logic


   return;
} // end of turbidityInit()

/*
 **********************************************************************
 * @brief UpdateTurbidity() Called once on each state change for Turbidity Timer (approx 20mS)
 *
 * @param none
 *
 * @return none
 *
 **********************************************************************
 */
void UpdateTurbidity( void )
{

   return;
} // end of UpdateTurbidity()

// end of file turbidity.c
