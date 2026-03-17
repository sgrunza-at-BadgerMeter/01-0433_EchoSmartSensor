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


#define SRC_MCP_C_

#include "mcp.h"


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
   uint32_t	pingCount = 0;

   bool				configOK;
   MCP_SYS_STATE_E		state;

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
      cfg_load_default( &SSP_configuration );
      printf( "Flash-based configuration is not valid, using defaults\r\n" );
   }

   // Initialize the wiper software even if there is no hardware to support it
   wiperInitMotor();

   // Initialize the turbidity software even if there is no hardware to support it
   turbidityInit();

   state = INIT_STATE;

   // Initialize status structure
   SSP_status.auto_gain = false;
   SSP_status.composite_wf = 0;
   SSP_status.echo_loss = false;
   SSP_status.fix_gain_band = false;
   SSP_status.need_setup = true;
   SSP_status.ntu = 0;
   SSP_status.previousNTU = 0;
   SSP_status.turbidityHoldOffTimer = 0;
   SSP_status.turbidityTimeout = 0;
   SSP_status.leveLoop_value = SSP_configuration.levelLoopMin;
   SSP_status.auxLoop_value = SSP_configuration.turbLoopMin;

   while( 1 )
   {

      switch( state )
      {
	 case INIT_STATE:
	    rs485_update_loops( &SSP_status );
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
	 printf( "mcp ping %ld\r\n", pingCount++ );
	 tx_thread_sleep( 10 * TX_TIMER_TICKS_PER_SECOND );	// place-holder until final logic is in place
      }

      tx_thread_sleep( 10 );	// place-holder until final logic is in place

   } // end of while(1)

   return;
} // end of mcp_thread_start()

// end of file mcp.c
