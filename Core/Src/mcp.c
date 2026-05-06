/**
 **********************************************************************
 * @file mcp.c
 *
 * @brief A brief description of this file's purpose
 *
 * @author grunzas
 *
 * @date Mar 12, 2026
 *
 **********************************************************************
 */

#include <stdio.h>

#include "tx_api.h"

#include "main.h"
#include "config.h"
#include "turbidity.h"
#include "wiper.h"
#include "system_io.h"
#include "dts.h"
#include "ssp_com.h"
#include "modbus.h"


#define SRC_MCP_C_

#include "mcp.h"

// Defined in dtc.c
extern DTS_HandleTypeDef hdts;



/*
 **********************************************************************
 * @brief mcp_thread_start() is the main control process for the sensor
 *
 * @param ulParameters - unused
 *
 * @return none
 **********************************************************************
 */
void
   mcp_thread_start(
      ULONG   		ulParameters )
{
   HAL_StatusTypeDef		retVal;

   bool				configOK;
   MCP_SYS_STATE_E		state;

   static int32_t		temperature = 0;

   // start with a delay to allow time for the other threads to init
   tx_thread_sleep( 1 * TX_TIMER_TICKS_PER_SECOND );

   // Load a configuration into the sensor
   configOK = cfg_verify_nvconfig( SSP_nvcfg_addr, sizeof(SSP_CONFIG_T) );
   if( configOK )
   {
      memcpy( &SSP_configuration, SSP_nvcfg_addr, sizeof( SSP_CONFIG_T) );
   }
   else
   {
      cfg_load_default( &SSP_configuration, LOAD_ALL_SETTINGS );
      printf( "Flash-based configuration is not valid, using defaults\r\n" );
   }

   // Initialize the wiper software even if there is no hardware to support it
   wiperInitMotor();

   // Initialize the turbidity software even if there is no hardware to support it
   turbidityInit();

   state = INIT_STATE;

   /* Start DTS peripheral */
   retVal = HAL_DTS_Start( &hdts );
   if( retVal != HAL_OK)
   {
     /* DTS start Error */
     Error_Handler();
   }

   /* Get temperature in deg C */
   retVal = HAL_DTS_GetTemperature( &hdts, &SSP_status.cpu_temperature );
   if(retVal != HAL_OK)
   {
     /* DTS GetTemperature Error */
     Error_Handler();
   }

   // Initialize status structure
   SSP_status.auto_gain = false;
   SSP_status.composite_wf = 0;
   SSP_status.echo_loss = false;
   SSP_status.fix_gain_band = false;
   SSP_status.bInitStatus = INIT_NEED_SETUP;
   SSP_status.ntu = 0;
   SSP_status.previousNTU = 0;
   SSP_status.turbidityHoldOffTimer = 0;
   SSP_status.turbidityTimeout = 0;
   SSP_status.levelLoop_value = SSP_configuration.levelLoopMin;
   SSP_status.auxLoop_value = SSP_configuration.turbLoopMin;

   temperature = 0;	// Should cause a temperature display event

   while( 1 )
   {

      switch( state )
      {
	 case INIT_STATE:
	    updateLevelLoop( SSP_status.levelLoop_value );
	    updateTurbLoop( SSP_status.auxLoop_value );
	    state = SETUP_STATE;
	    break;

	 case SETUP_STATE:
	    break;

	 case PING_STATE:
	    break;

	 case ANALYZE_STATE:
	    break;

	 case GET_NEXT_WAVEFORM_STATE:
	    break;

	 case LOOP_STATE:
	    break;

	 case GET_NEXT_WAVEFORM_GROUP_STATE:
	    break;

	 case LAST_STATE:
	    break;

	 default:
	    state = INIT_STATE;
	    break;
      }

      if( SSP_configuration.hasTurbidity )
      {
	 UpdateTurbidity();
      }

      // TODO:  Remove the following debug code
      {
	 //printf( "mcp ping %ld\r\n", pingCount++ );
	 tx_thread_sleep( 10 * TX_TIMER_TICKS_PER_SECOND );	// place-holder until final logic is in place
      }

      /* Get temperature in deg C */
      retVal = HAL_DTS_GetTemperature( &hdts, &SSP_status.cpu_temperature );
      if(retVal != HAL_OK)
      {
        /* DTS GetTemperature Error */
        Error_Handler();
      }

      if( abs( SSP_status.cpu_temperature - temperature ) > TEMP_REPORT_DIFF )
      {
	 printf( "cpu temperature is %ld C\r\n", SSP_status.cpu_temperature );
	 temperature = SSP_status.cpu_temperature;
      }

      tx_thread_sleep( 10 );	// place-holder until final logic is in place

      // Should periodically check if the saved configuration matches the current configuration
      // Update the saved configuration if necessary

   } // end of while(1)

   return;
} // end of mcp_thread_start()

// end of file mcp.c
