/**
 **********************************************************************
 * @file console.c
 *
 * @brief This file holds the console handling code
 *
 * @author Steven Grunza
 *
 * @date 15 Sep 2025
 *
 **********************************************************************
 */


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "main.h"
#include "app_threadx.h"
#include "tx_api.h"

#include "usart.h"
#include "accel.h"
#include "i2c.h"

//#define	INCLUDE_FAULT_CMD

#define		INCLUDE_RS485_UNSOLICITED

#include "console.h"
#include "fifo.h"
#include "gpio.h"
#include "system_io.h"
#include "ssp_com.h"
#include "config.h"
#include "turbidity.h"

#include "sonar.h"

#include "version.h"

#include "adc.h"
#include "dac.h"

// Store a pointer to this thread's ID structure
static TX_THREAD	*myID = NULL;


// Status flag to show if there is a message to process in the console_rx_buf
volatile bool	msg_to_process = 0;

// Console message reception buffer, plus space for the control structure QUEUE_T
FIFO_T		console_rx_buf;

// Status flag to determine if RS485 messages received from Controller should
// be echo'd to debug console
bool		rs485_echo = false;

// Status flag to indicate if unsolicited messages are allowed to be sent
bool		rs485_usmsg_allowed = false;

// Defined in system_io.c
extern RS485_COMM_STATISTICS_T		rs485_stats;

// Defined in dac.c
extern DAC_HandleTypeDef 		hdac1;

const CMD_INFO_T        gCommands[] =
{
	 { "?",              	cmd_help,	(const char *)"Show available commands" },
	 { "accel",		cmd_accel,	(const char *)"Read accelerometer"},
	 { "adc",		cmd_adc,	(const char *)"Read from ADC1 or ADC2"},
	 { "dac",		cmd_dac,	(const char *)"Set RXGAIN level"},
	 { "cfg",		cmd_configure,	(const char *)"Configure board"},
#ifdef INCLUDE_FAULT_CMD
	 { "fault",		cmd_fault,	(const char *)"Cause a hard fault"},
#endif
	 { "getTick",		cmd_getTick,	(const char *)"Show current system tick" },
	 { "help",           	cmd_help,	(const char *)"Show available commands" },
	 { "id",           	cmd_id,		(const char *)"Show unique device ID" },
	 { "led",		cmd_led,	(const char *)"Set / Get LED state(s)"},
	 { "motor",		cmd_motor,	(const char *)"Set / Get Motor drive"},
	 { "ping",		cmd_ping,	(const char *)"Send and receive a sonar ping"},
	 { "reset",		cmd_reset,	(const char *)"Reset board"},
	 { "rs485",		cmd_rs485,	(const char *)"RS-485 related commands"},
	 { "swver",		cmd_swver,	(const char *)"Get software version information"},
	 { "temp",		cmd_temp,	(const char *)"Get CPU temperature"},
	 { "turbidity",		cmd_turbidity,	(const char *)"Get turbidity info"},
	 { NULL, NULL, NULL }
};

/*
 **********************************************************************
 * @brief threadConsole() has a FIFO to receive ASCII text message
 * commands straight from the receiving UART at interrupt level
 *
 * There is a msg_to_process flag set at interrupt level when a complete
 * message has been received.
 *
 * @param ulParameters - unused
 *
 * @return none
 **********************************************************************
 */
void
   threadConsole(
      ULONG   		ulParameters )
{
   char		cmd[ MAX_FIFO_ENTRY_LEN ];
   uint8_t	loops;
   int8_t	ret;
   uint16_t	unused;

   myID = tx_thread_identify();

   // Initialize queue to store incoming message from UART
   fifo_init( &console_rx_buf );

   // Clear screen
   for( loops = 0; loops < 10; loops++ )
   {
      printf( "\r\n" );
   }

   printf("EchoSmart / FilterSmart Probe\r\n" );
   printf( BuildInfo );
   printf( "\r\n" );
   printf( "\r\n" );

   cmd_help( "help" );

   printf( "\r\n> " );
   fflush( stdout );

   // Load the configuration structure
   cfg_load_default( &SSP_configuration, LOAD_FROM_FLASH );

   // Start the asynchronous reception process by receiving one character
   console_receive_char();

   while( 1 )
   {
      // Currently using time-slicing so process commands until there are none left
      while( console_rx_buf.entries > 0 )
      {
	 memset( cmd, 0, MAX_FIFO_ENTRY_LEN );

	 ret = fifo_pop( &console_rx_buf,	// ptr to queue
			 cmd, 			// ptr into which to put extracted command
			 &unused );
	 if( ret == FIFO_SUCCESS )
	 {
	    if( cmd[0] != '\0' )
	    {
	       printf( "\r\n" );
	       // Process command if there is a command to process
	       process_msg( (char *)cmd );

	       printf( "\r\n> " );
	       fflush( stdout );
	    }
	 }
      } // end of while( commands to process )
      //tx_thread_suspend( myID );	// need to restart with tx_thread_resume()

      tx_thread_sleep( 10 );
   }

   return;
} // end of threadConsole()


/*
 **********************************************************************
 * @brief Return the address of the FIFO holding the commands.
 *
 * @return pointer to FIFO_T structure
 **********************************************************************
 */
FIFO_T *
   consoleGetCmdFIFO(
      void )
{
   return &console_rx_buf;
} // consoleGetCmdFIFO()


/*
 **********************************************************************
 * @brief cmd_help() displays the help menu on the debug console
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 **********************************************************************
 */
void cmd_help( char     *msg )
{

   const CMD_INFO_T     *pCmdInfo;
   char *c;

   if( msg == NULL )
   {
      return;
   }

   // make the whole string lower case so later processing is easier
   c = msg;
   while( *c != '\0' )
   {
      *c = tolower(*c);
      c++;
   }

   c = msg;

   // might be pointing at a ? or an h

   // move to the end of the current token
   while( (*c != '\0') && (*c != ' ') )
   {
      c++;
   }

   // Check if "help" or "?" is the only thing on the line
   if( *c == '\0' )
   {
      // No more arguments so display first level help
      pCmdInfo = gCommands;

      while( pCmdInfo->cmd_handler != NULL )
      {
	 printf( (char *)pCmdInfo->pCmd );
	 printf( "\t - " );
	 printf( (char *)pCmdInfo->descr );
	 printf( "\r\n" );
	 pCmdInfo++;
      }
      printf( "\r\n\r\n" );
   }

   return;
}

/*
 **********************************************************************
 * @brief process_msg() handle the message from the debug console
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 **********************************************************************
 */
void process_msg( char *msg )
{
   const CMD_INFO_T     *pCmdInfo;
   char                 *pFound;
   static char		prevCmd[ MAX_FIFO_ENTRY_LEN ] = { 0 };


   // Search the list of known commands to find this message's command
   pCmdInfo = gCommands;

   // Check if this is just a carriage return
   if( *msg == '\r' )
   {
      // Copy the previous command
      memcpy( msg, prevCmd, strlen(prevCmd) );
      printf( "%s\r\n", prevCmd );
   }

   if( *msg != ' ' )
   {
      while( pCmdInfo->cmd_handler != NULL )
      {
	 pFound = strstr( msg, pCmdInfo->pCmd );
	 if( pFound == msg )
	 {
	    // This command was found at the beginning of the msg so process it
	    memcpy( prevCmd, msg, MAX_FIFO_ENTRY_LEN );
	    printf( "\r\n" );
	    pCmdInfo->cmd_handler( msg );
	    break;
	 }
	 else
	 {
	    pCmdInfo++;
	 }
      }

      if( pCmdInfo->cmd_handler == NULL )
      {
	 // Got to the end
	 printf( "Unknown command: " );
	 printf( msg );
	 printf( "\r\n" );
      }
   }
   else
   {
      // Just skip commands if they start with a space
   }

   return;
} // end of process_msg()

#ifdef INCLUDE_FAULT_CMD
/*
 **********************************************************************
 * @brief cmd_fault() causes bad things to happen.  It's a jump to and
 * address without valid code.  The intent is to check the system's
 * recovery, most likely enabled by the watchdog functionality.
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 **********************************************************************
 */
void cmd_fault( char     *msg )
{
   CMD_INFO_T		badCmd;

   badCmd.descr = "bad command";
   badCmd.pCmd  = "fubar";
   badCmd.cmd_handler = (CMD_HANDLER_T *)0xFFFFFFF0;

   badCmd.cmd_handler( (char *) badCmd.descr );

   return;
} // end of cmd_fault()
#endif	// INCLUDE_FAULT_CMD

/*
 **********************************************************************
 * @brief cmd_reset() causes the system to reset
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 **********************************************************************
 */
void cmd_reset( char     *msg )
{

   NVIC_SystemReset();

   return;
} // end of cmd_reset()

/*
 **********************************************************************
 * @brief cmd_swver() displays system software information
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 **********************************************************************
 */
void cmd_swver( char     *msg )
{

   printf( SoftwareNameString );
   printf( "\r\n" );

   printf( BuildInfo );
   printf( "\r\n" );

   return;
} // end of cmd_swver()

/*
 **********************************************************************
 * @brief cmd_getTick() returns the current system tick
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 **********************************************************************
 */
void cmd_getTick( char     *msg )
{
   uint32_t		current_tick;

   current_tick = HAL_GetTick();


   printf( "Current tick is %ld (0x%8.8lx)\r\n",
	   current_tick,
	   current_tick );
   return;
} // end of cmd_getTick()

/*
 **********************************************************************
 * @brief cmd_rs485() handles all the RS-485 related commands
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 *
 * rs485 echo
 * rs485 echo on
 * rs485 echo off
 *
 * rs485 stats
 * rs485 stats clear
 *
 * rs485 usmsg		{ gives status of unsolicited message }
 * rs485 usmsg enable
 * rs485 usmsg disable
 * rs485 usmsg sspid	{ send unsolicited SSP ID Response 0x41 }
 * rs485 usmsg raw aa bb cc dd {send unsolicited bytes }
 *
 *
 **********************************************************************
 */
void cmd_rs485( char     *msg )
{
   char 	*c;
   int		cmpVal;
   bool		done;

   if( msg == NULL )
   {
      return;
   }

   done = false;

   // make the whole string lower case so later processing is easier
   c = msg;
   while( *c != '\0' )
   {
      *c = tolower(*c);
      c++;
   }

   c = msg;

   // should be pointing at r of rs485

   // move to the end of the current token
   while( (*c != '\0') && (*c != ' ') )
   {
      c++;
   }

   if( *c != '\0' )
   {
      // Jump past the space
      c++;
   }

   // Check if this is an echo, help, stats, or usmsg command
   if( *c == 'e' )
   {
      // Looks like an echo command
      cmpVal = strncmp( c, "echo", 4 );
      if( cmpVal == 0 )
      {
	 // It is an echo command

	 // move to the end of the current token
	 while( (*c != '\0') && (*c != ' ') )
	 {
	    c++;
	 }

	 if( *c != '\0' )
	 {
	    // Jump past the space
	    c++;
	 }

	 if( *c == '\0' )
	 {
	    // Command was rs485 echo without any parameters
	    printf( "rs485 echo status is " );
	    if( rs485_echo )
	    {
	       printf( "on\r\n" );
	    }
	    else
	    {
	       printf( "off\r\n" );
	    }
	    done = true;
	 }

	 if( !done && ( *(c+1) != '\0' ) )
	 {
	    // character at c and (c+1) are not null
	    if( ( *c == 'o') && ( *(c+1) == 'n') )
	    {
	       rs485_echo = true;
	       done = true;

	       printf( "rs485 echo status is " );
	       if( rs485_echo )
	       {
		  printf( "on\r\n" );
	       }
	       else
	       {
		  printf( "off\r\n" );
	       }
	    }
	 }

	 if( !done )
	 {
	    if( *(c+2) != '\0' )
	    {
	       if( ( *c == 'o') &&
		   ( *(c+1) == 'f') &&
		   ( *(c+2) == 'f') )
	       {
		  rs485_echo = false;
		  done = true;
		  printf( "rs485 echo status is " );
		  if( rs485_echo )
		  {
		     printf( "on\r\n" );
		  }
		  else
		  {
		     printf( "off\r\n" );
		  }
	       }
	    }
	 }
      }
   }
   else if( *c == 'h' && (strlen(c) > 3) )
   {
      // Looks like a help command
      cmpVal = strncmp( c, "help", 4 );
      if( cmpVal == 0 )
      {
         done = true;
	 printf( "rs485 echo on\r\n" );
	 printf( "rs485 echo off\r\n" );
	 printf( "\r\n" );
	 printf( "rs485 stats\r\n" );
	 printf( "rs485 stats clear\r\n" );
	 printf( "\r\n" );
	 printf( "rs485 usmsg		{ gives status of unsolicited message }\r\n" );
	 printf( "rs485 usmsg enable\r\n" );
	 printf( "rs485 usmsg disable\r\n" );
	 printf( "rs485 usmsg sspid	{ send unsolicited SSP ID Response 0x41 }\r\n" );
	 printf( "rs485 usmsg raw aa bb cc dd {send unsolicited bytes }\r\n" );
	 printf( "\r\n" );
      }
   }
   else if( (*c == 's') && (strlen(c) > 4) )
   {
      // Looks like a stats command
      cmpVal = strncmp( c, "stats", 5 );
      if( cmpVal == 0 )
      {
	 // move to the end of the current token
	 while( (*c != '\0') && (*c != ' ') )
	 {
	    c++;
	 }

	 if( *c != '\0' )
	 {
	    // Jump past the space
	    c++;
	 }

	 // Should be pointing to either a NULL or the c of clear
	 if( *c == '\0' )
	 {
	    // This is a stats command to request the current statistics
	    done = true;
	    //rs485_stat_print( &rs485_stats );
	    printf("called rs485_stat_print()\r\n");
	 }

	 cmpVal = strncmp( c, "clear", 5 );
	 if( !done && (strlen(c) >= 4) )
	 {
	    if( cmpVal == 0 )
	    {
	       // This is a stats clear command to clear the counters
	       done = true;
	       //rs485_stat_clear( &rs485_stats );
	       printf("rs485_stat_clear()\r\n");
	    }
	 }
      }
   }
   else if( *c == 'u' )
   {
      // Looks like a umsg command
      cmpVal = strncmp( c, "usmsg", 5 );
      if( cmpVal == 0 )
      {
	 // move to the end of the current token
	 while( (*c != '\0') && (*c != ' ') )
	 {
	    c++;
	 }

	 if( *c != '\0' )
	 {
	    // Jump past the space
	    c++;
	 }

	 // c should point to either a NULL or enable, disable, sspid, or raw

	 if( *c == '\0' )
	 {
	    // This is a rs485 usmsg command to get the flag status
	    // ..allowing unsolicited messages to be sent
	    done = true;
	    printf( "Unsolicited Messages ");
	    if( rs485_usmsg_allowed )
	    {
	       printf( "allowed\r\n" );
	    }
	    else
	    {
	       printf( "not allowed\r\n" );
	    }
	 }

	 // enable, disable, sspid, or raw

	 cmpVal = strncmp( c, "enable", 6 );
	 if( (cmpVal == 0) && !done )
	 {
	    // This is a rs485 usmsg enable command
	    done = true;
	    rs485_usmsg_allowed = true;

	    printf( "Unsolicited Messages ");
	    if( rs485_usmsg_allowed )
	    {
	       printf( "allowed\r\n" );
	    }
	    else
	    {
	       printf( "not allowed\r\n" );
	    }
	 }

	 cmpVal = strncmp( c, "disable", 7 );
	 if( (cmpVal == 0) && !done )
	 {
	    // This is a rs485 usmsg disable command
	    done = true;
	    rs485_usmsg_allowed = false;

	    printf( "Unsolicited Messages ");
	    if( rs485_usmsg_allowed )
	    {
	       printf( "allowed\r\n" );
	    }
	    else
	    {
	       printf( "not allowed\r\n" );
	    }
	 }

	 cmpVal = strncmp( c, "sspid", 5 );
	 if( (cmpVal == 0) && !done )
	 {
	    // This is a rs485 usmsg sspid command
	    done = true;
	    if( rs485_usmsg_allowed )
	    {
	       printf("send an unsolicited SSP ID message\r\n");
	       IdRequestCmd();
	    }
	    else
	    {
	       printf("not allowed to send unsolicited message\n\r" );
	    }
	 }

	 cmpVal = strncmp( c, "raw", 3 );
	 if( (cmpVal == 0) && !done )
	 {
	    // This is a rs485 usmsg raw command

	    // c points to r of raw

	    // move to the end of the current token
	    while( (*c != '\0') && (*c != ' ') )
	    {
	       c++;
	    }

	    if( *c != '\0' )
	    {
	       // Jump past the space
	       c++;
	    }

	    // c should point to the first part of the byte string

	    done = true;
	    if( rs485_usmsg_allowed )
	    {
	       printf("send an unsolicited raw message\r\n");
	       rs485_send_raw( c );
	    }
	    else
	    {
	       printf("not allowed to send unsolicited raw message\r\n" );
	    }
	 }
      }
   }

   if( !done )
   {
      // Congrats!  You have won 'stump the parser'
      printf( "Unable to parse command \"%s\"\r\n", msg );
   }

   return;
} // end of cmd_rs485()


/*
 **********************************************************************
 * @brief cmd_configure() handles setting and displaying the board's
 * configuration
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 *
 *
 *
 * cfg     	-- show current RAM-based configuration
 * cfg load	-- copy Flash-based config to RAM
 * cfg save	-- save RAM-based config to Flash
 *
 * cfg addr x	-- set RS-485 address
 * cfg eq_type	-- set equipment type
 * cfg fw_ver	-- set fw_ver
 * cfg gainAdj x - set gainAdjust to x
 * cfg help	-- this display
 * cfg hw_id xx -- set hw_id
 * cfg range x	-- set range to x
 * cfg sn xxx	-- set serialNumber
 * cfg turb	-- set if turbidity is supported
 * cfg wiper x	-- set if wiper is supported
 *
 **********************************************************************
 */
void cmd_configure( char     *msg )
{
   bool		done = false;
   int		cmpVal;

   char		*c;

   if( msg == NULL )
   {
      return;
   }

   // make the whole string lower case so later processing is easier
   c = msg;
   while( *c != '\0' )
   {
      *c = tolower(*c);
      c++;
   }

   c = msg;

   // move to the end of the current token
   while( (*c != '\0') && (*c != ' ') )
   {
      c++;
   }

   // skip over the space if there is one
   if( *c != '\0' )
   {
      c++;
   }

   // Check 'cfg' is the only thing on the line
   if( *c == '\0' )
   {
      // No more arguments so display current in-RAM config

      printf( "\r\n" );
      printf( "configuration structure holds:\r\n" );
      printf( "\tserialNumber:\t%d\r\n", SSP_configuration.serialNumber );
      printf( "\thw_id:       \t%d\r\n", SSP_configuration.hw_id );
      printf( "\tfw_ver:      \t%d\r\n", SSP_configuration.fw_ver );
      printf( "\teq_type:     \t%s(%d)\r\n",
	      cfg_equip_name(SSP_configuration.eq_type),
	      SSP_configuration.eq_type );
      printf( "\taddress:     \t%d\r\n", SSP_configuration.address );
      printf( "\thasWiper:    \t%d\r\n", SSP_configuration.hasWiper );
      printf( "\thasTurbidity:\t%d\r\n", SSP_configuration.hasTurbidity );
      printf( "\trange:       \t%d\r\n", SSP_configuration.range );
      printf( "\tgainAdjust:  \t%d\r\n", SSP_configuration.gainAdjust );
      printf( "\tUUID:        \t0x%8.8lx %8.8lx %8.8lx\r\n", SSP_configuration.uuid.first,
	      SSP_configuration.uuid.second, SSP_configuration.uuid.third );


      printf( "sizeof(SSP_configuration) is %d\r\n", sizeof(SSP_configuration) );

      printf( "\r\n" );
      done = true;
   }
   else
   {
      // There is more to process

      cmpVal = strncmp( c, "load", 4 );
      if( cmpVal == 0 )
      {
	 // move to the end of the current token
	 while( (*c != '\0') && (*c != ' ') )
	 {
	    c++;
	 }

	 // skip over the space if there is one
	 if( *c != '\0' )
	 {
	    c++;
	 }

	 cmpVal = strncmp( c, "default", 7 );
	 if( cmpVal == 0 )
	 {
	    cfg_load_default( &SSP_configuration, LOAD_USER_SETTINGS );
	 }
	 else
	 {
	    // This is a load cmd
	    bool		cfgOk;

	    cfgOk = cfg_verify_nvconfig( SSP_nvcfg_addr, sizeof(SSP_CONFIG_T) );
	    if( cfgOk )
	    {
	       memcpy( &SSP_configuration, SSP_nvcfg_addr, sizeof( SSP_CONFIG_T) );
	    }
	    else
	    {
	       printf( "Flash-based configuration is not valid\r\n" );
	    }
	 }
	 done = true;
      }

      if( !done )
      {
	 cmpVal = strncmp( c, "save", 4 );
	 if( cmpVal == 0 )
	 {
	    // This is a save cmd
	    bool	writeOk;

	    done = true;

	    writeOk = cfg_write_nvconfig(
		         &SSP_configuration,		// in RAM
			 SSP_nvcfg_addr,		// in Flash
			 sizeof( SSP_CONFIG_T ) );	// size in bytes
	    if( writeOk )
	    {
	       printf( "Configuration written to Flash\r\n" );
	    }
	    else
	    {
	       printf( "Write to Flash failed in %s at %d\r\n", __FILE__, __LINE__ );
	    }
	 }
      }

      if( !done )
      {
	 cmpVal = strncmp( c, "addr", 4 );
	 if( cmpVal == 0 )
	 {
	    uint8_t	addr;

	    // This is an addr command
	    done = true;

	    // move to the end of the current token
	    while( (*c != '\0') && (*c != ' ') )
	    {
	       c++;
	    }

	    // skip over the space if there is one
	    if( *c != '\0' )
	    {
	       c++;
	    }

	    addr = (uint8_t) strtol( c, NULL, 0 );
	    SSP_configuration.address = addr;

	    printf( "\taddress:     \t%d\r\n", SSP_configuration.address );
	 }
      }

      if( !done )
      {
	 cmpVal = strncmp( c, "eq_type", 7 );
	 if( cmpVal == 0 )
	 {
	    uint8_t	eq_type;

	    // This is an eq_type command
	    done = true;

	    // move to the end of the current token
	    while( (*c != '\0') && (*c != ' ') )
	    {
	       c++;
	    }

	    // skip over the space if there is one
	    if( *c != '\0' )
	    {
	       c++;
	    }

	    eq_type = (uint8_t) strtol( c, NULL, 0 );
	    if( eq_type <= EQ_TYPE_LCD_CTRL )
	    {


	       SSP_configuration.eq_type = eq_type;

	       printf( "\teq_type:     \t%s(%d)\r\n\n",
		       cfg_equip_name(SSP_configuration.eq_type),
		       SSP_configuration.eq_type );
	    }
	    else
	    {
	       printf( "%d is an invalid equipment type\r\n", eq_type );
	    }
	 }
      }

      if( !done )
      {
	 cmpVal = strncmp( c, "fw_ver", 6 );
	 if( cmpVal == 0 )
	 {
	    uint8_t		fw_ver;

	    // This is an fw_ver command
	    done = true;

	    // move to the end of the current token
	    while( (*c != '\0') && (*c != ' ') )
	    {
	       c++;
	    }

	    // skip over the space if there is one
	    if( *c != '\0' )
	    {
	       c++;
	    }

	    fw_ver = (uint8_t) strtol( c, NULL, 0 );
	    SSP_configuration.fw_ver = fw_ver;

	    printf( "\tfw_ver:      \t%d\r\n", SSP_configuration.fw_ver );
	 }
      }

      if( !done )
      {
	 cmpVal = strncmp( c, "gainadj", 7 );
	 if( cmpVal == 0 )
	 {
	    int32_t		gainAdjust;

	    // This is a gain adjust command
	    done = true;

	    // move to the end of the current token
	    while( (*c != '\0') && (*c != ' ') )
	    {
	       c++;
	    }

	    // skip over the space if there is one
	    if( *c != '\0' )
	    {
	       c++;
	    }

	    gainAdjust = strtol( c, NULL, 0 );
	    if( (gainAdjust > -4096) && (gainAdjust < 4096) )
	    {
	       SSP_configuration.gainAdjust = gainAdjust;

	       printf( "\tgainAdjust:  \t%d\r\n\n", SSP_configuration.gainAdjust );
	    }
	    else
	    {
	       printf( "gainAdjust range is [-4095, 4095]\r\n" );
	    }
	 }
      }

      if( !done )
      {
	 cmpVal = strncmp( c, "hw_id", 5 );
	 if( cmpVal == 0 )
	 {
	    uint8_t		hw_id;

	    // This is a hw_id command
	    done = true;

	    // move to the end of the current token
	    while( (*c != '\0') && (*c != ' ') )
	    {
	       c++;
	    }

	    // skip over the space if there is one
	    if( *c != '\0' )
	    {
	       c++;
	    }

	    hw_id = (uint8_t) strtol( c, NULL, 0 );
	    SSP_configuration.hw_id = hw_id;

	    printf( "\thw_id:       \t%d\r\n", SSP_configuration.hw_id );
	 }
      }

      if( !done )
      {
	 cmpVal = strncmp( c, "range", 5 );
	 if( cmpVal == 0 )
	 {
	    uint8_t		range;

	    // This is a range command
	    done = true;

	    // move to the end of the current token
	    while( (*c != '\0') && (*c != ' ') )
	    {
	       c++;
	    }

	    // skip over the space if there is one
	    if( *c != '\0' )
	    {
	       c++;
	    }

	    range = (uint8_t) strtol( c, NULL, 0 );
	    if( range < 4 )
	    {
	       SSP_configuration.range = range;

	       printf( "\trange:       \t%d\r\n", SSP_configuration.range );
	    }
	    else
	    {
	       printf( "valid range settings are 0, 1, 2, or 3\r\n\n");
	    }
	 }
      }

      if( !done )
      {
	 cmpVal = strncmp( c, "serialnumber", 12 );
	 if( cmpVal == 0 )
	 {
	    uint16_t		serialNumber;

	    // This is a serial number command
	    done = true;

	    // move to the end of the current token
	    while( (*c != '\0') && (*c != ' ') )
	    {
	       c++;
	    }

	    // skip over the space if there is one
	    if( *c != '\0' )
	    {
	       c++;
	    }

	    serialNumber = (uint16_t) strtol( c, NULL, 0 );
	    SSP_configuration.serialNumber = serialNumber;

	    printf( "\tserialNumber:\t%d\r\n", SSP_configuration.serialNumber );
	 }
      }

      if( !done )
      {
	 cmpVal = strncmp( c, "turb", 4 );
	 if( cmpVal == 0 )
	 {
	    // This is a turbidity command
	    done = true;

	    // move to the end of the current token
	    while( (*c != '\0') && (*c != ' ') )
	    {
	       c++;
	    }

	    // skip over the space if there is one
	    if( *c != '\0' )
	    {
	       c++;
	    }

	    if( *c == '0' )
	    {
	       SSP_configuration.hasTurbidity = 0;
	    }
	    else if( *c == '1' )
	    {
	       SSP_configuration.hasTurbidity = 1;
	    }
	    else
	    {
	       printf( "0 = no turbidity support, 1 = turbidity support\r\n\n");
	    }

	    printf( "\thasTurbidity:\t%d\r\n", SSP_configuration.hasTurbidity );
	 }
      }

      if( !done )
      {
	 cmpVal = strncmp( c, "wiper", 5 );
	 if( cmpVal == 0 )
	 {
	    // This is a wiper command
	    done = true;

	    // move to the end of the current token
	    while( (*c != '\0') && (*c != ' ') )
	    {
	       c++;
	    }

	    // skip over the space if there is one
	    if( *c != '\0' )
	    {
	       c++;
	    }

	    if( *c == '0' )
	    {
	       SSP_configuration.hasWiper = 0;
	    }
	    else if( *c == '1' )
	    {
	       SSP_configuration.hasWiper = 1;
	    }
	    else
	    {
	       printf( "0 = no wiper 1 = wiper present\r\n");
	    }

	    printf( "\thasWiper:    \t%d\r\n", SSP_configuration.hasWiper );
	 }
      }

      if( !done )
      {
	 cmpVal = strncmp( c, "help", 4 );
	 if( cmpVal == 0 )
	 {
	    // This is a help command
	    done = true;

	    printf( "cfg           -- show current RAM-based configuration\r\n" );
            printf( "cfg load      -- copy Flash-based config to RAM\r\n" );
            printf( "cfg save      -- save RAM-based config to Flash\r\n" );
	    printf( "\r\n" );
            printf( "cfg addr x    -- set 8-bit RS-485 address\r\n" );
            printf( "cfg eq_type   -- set equipment type (1=Probe, 2=Power Supply, 3=Controller)\r\n" );
            printf( "cfg fw_ver    -- set 8-bit fw_ver\r\n" );
            printf( "cfg gainAdj x -- set gainAdjust to x (-4095, 4095)\r\n" );
            printf( "cfg help      -- this display\r\n" );
            printf( "cfg hw_id xx  -- set 8-bit hw_id\r\n" );
            printf( "cfg range x   -- set range to x [0, 3]\r\n" );
            printf( "cfg sn xxx    -- set 16-bit serialNumber\r\n" );
            printf( "cfg turb      -- set if turbidity is supported (1=yes, 2=no)\r\n" );
            printf( "cfg wiper x   -- set if wiper is supported (1=yes, 2=no)\r\n" );
            printf( "\r\n" );
	 }
      }

   }

   if( !done )
   {
      // Congrats!  You have won 'stump the parser'
      printf( "Unable to parse command \"%s\"\r\n", msg );
   }

   return;
} // end of cmd_configure()

/*
 **********************************************************************
 * @brief cmd_get_threadID() returns the thread ID that was saved
 * when the console thread was running
 *
 * @param none
 *
 * @return pointer to thread structure
 **********************************************************************
 */
TX_THREAD *
   cmd_get_threadID(
      void )
{
   return( myID );
} // end of cmd_get_threadID()

/*
 **********************************************************************
 * @brief cmd_led() sets or gets the LED state
 *
 * @param msg - complete message from the debug console
 *
 * led - gives status of all LEDs and photo detector
 *
 * led on dbg turns on debug LED
 * led on status turns on status LED
 * led on turbidity turns on turbidity LED
 *
 * led off dbg turns off debug LED
 * led off status turns off status LED
 * led off turbidity turns off turbidity LED
 *
 *
 * @return none
 **********************************************************************
 */
void cmd_led( char     *msg )
{
   char 	*c;
   int			cmpVal;
   bool			done;
   GPIO_PinState	pinState;
   bool			cmdIsOn;


   if( msg == NULL )
   {
      return;
   }

   done = false;
   cmdIsOn = false;

   // make the whole string lower case so later processing is easier
   c = msg;
   while( *c != '\0' )
   {
      *c = tolower(*c);
      c++;
   }

   c = msg;

   // should be pointing at l of led

   // move to the end of the current token
   while( (*c != '\0') && (*c != ' ') )
   {
      c++;
   }

   if( *c != '\0' )
   {
      // Jump past the space
      c++;
   }

   // Check if this is a status request or a state change
   if( *c == '\0' )
   {
      // Looks like a status request
      done = true;
      printf("Debug LED is ");
      pinState = HAL_GPIO_ReadPin( nLED_GPIO_Port, nLED_Pin );
      if( pinState == GPIO_PIN_SET )
      {
	 printf( "off\r\n" );
      }
      else
      {
	 printf( "on\r\n" );
      }

      printf("Status LED is ");
      pinState = HAL_GPIO_ReadPin( nSTATUS_LED_GPIO_Port, nSTATUS_LED_Pin );
      if( pinState == GPIO_PIN_SET )
      {
	 printf( "off\r\n" );
      }
      else
      {
	 printf( "on\r\n" );
      }

      printf("Turbidity LED is ");
      pinState = HAL_GPIO_ReadPin( T_LED_GPIO_Port, T_LED_Pin );
      if( pinState == GPIO_PIN_SET )
      {
	 printf( "on\r\n" );
      }
      else
      {
	 printf( "off\r\n" );
      }
   }

   if( !done && (*c == 'o') )
   {
      // Either an 'on' or an 'off'
      c++;
      if( *c == 'n' )
      {
	 // it's an on command
	 cmdIsOn = true;
	 done = true;
      }

      if( *c == 'f' )
      {
	 // it's either an of or an off
	 cmdIsOn = false;
	 done = true;
      }

      if( done )		// found on or off
      {
	 // move to the end of the current token
	 while( (*c != '\0') && (*c != ' ') )
	 {
	    c++;
	 }

	 if( *c != '\0' )
	 {
	    // Jump past the space
	    c++;
	 }

	 cmpVal = strncmp( c, "dbg", 3 );
	 if( cmpVal == 0 )
	 {
	    if( cmdIsOn )
	    {
	       GPIO_led_on();
	    }
	    else
	    {
	       GPIO_led_off();
	    }
	 }

	 cmpVal = strncmp( c, "status", 6 );
	 if( cmpVal == 0 )
	 {
	    if( cmdIsOn )
	    {
	       GPIO_status_led_on();
	    }
	    else
	    {
	       GPIO_status_led_off();
	    }
	 }

	 cmpVal = strncmp( c, "turbidity", 6 );
	 if( cmpVal == 0 )
	 {
	    if( cmdIsOn )
	    {
	       GPIO_turbidity_led_on();
	    }
	    else
	    {
	       GPIO_turbidity_led_off();
	    }
	 }
      }
   }

   if( !done && (*c == 't') )
   {
      cmpVal = strncmp( c, "toggle", 6 );
      if( cmpVal == 0 )
      {
	 // move to the end of the current token
	 while( (*c != '\0') && (*c != ' ') )
	 {
	    c++;
	 }

	 if( *c != '\0' )
	 {
	    // Jump past the space
	    c++;
	 }

	 cmpVal = strncmp( c, "dbg", 3 );
	 if( cmpVal == 0 )
	 {
	    done = true;
	    GPIO_led_toggle();
	 }

	 cmpVal = strncmp( c, "status", 6 );
	 if( cmpVal == 0 )
	 {
	    done = true;
	    GPIO_status_led_toggle();
	 }

	 cmpVal = strncmp( c, "turbidity", 6 );
	 if( cmpVal == 0 )
	 {
	    done = true;
	    GPIO_turbidity_led_toggle();
	 }
      }
   }

   if( !done && (*c == 'h') )
   {
      cmpVal = strncmp( c, "help", 4 );
      if( cmpVal == 0 )
      {
	 // Give some help information
	 done = true;

	 printf( " led - gives status of all LEDs and photo detector\r\n" );
	 printf( "\r\n" );
	 printf( "led on dbg 		- turns on debug LED\r\n" );
	 printf( "led off dbg 		- turns off debug LED\r\n" );
	 printf( "led toggle dbg 	- changes state of debug LED\r\n" );
	 printf( "\r\n" );
	 printf( "led on status 	- turns on status LED\r\n" );
	 printf( "led off status 	- turns off status LED\r\n" );
	 printf( "led toggle status	- changes state of status LED\r\n" );
	 printf( "\r\n" );
	 printf( "led on turbidity 	- turns on turbidity LED\r\n" );
	 printf( "led off turbidity 	- turns off turbidity LED\r\n" );
	 printf( "led toggle turbidity	- changes state of tubidity LED\r\n" );
	 printf( "\r\n" );
      }
   }

   if( !done )
   {
      // Congrats!  You have won 'stump the parser'
      printf( "Unable to parse command \"%s\"\r\n", msg );
   }
   fflush( stdout );

   return;
} // end of cmd_led()

/*
 **********************************************************************
 * @brief cmd_adc() reads from the ADC1 or ADC2 converter
 *
 * @param msg - complete message from the debug console
 *
 * adc - returns all readings
 *
 * adc sonar returns current value at sonar ADC input
 * adc tled returns current value at Turbidity LED driver
 * adc tdet returns current value at Turbidity photo detector input
 *
 * adc wiperi returns current value at wiper motor current input
 * adc wiperv returns current value at wiper motor voltage input
 *
 * @return none
 **********************************************************************
 */
void cmd_adc( char     *msg )
{
   char 			*c;
   int			cmpVal;
   uint32_t		reading;
   double		value;

   bool			sonar = false;
   bool			tled = false;
   bool			tdet = false;
   bool			wiperi = false;
   bool			wiperv = false;
   bool			help = false;

   if( msg == NULL )
   {
      return;
   }

   // make the whole string lower case so later processing is easier
   c = msg;
   while( *c != '\0' )
   {
      *c = tolower(*c);
      c++;
   }

   c = msg;

   // should be pointing at a of adc

   // move to the end of the current token
   while( (*c != '\0') && (*c != ' ') )
   {
      c++;
   }

   if( *c != '\0' )
   {
      // Jump past the space
      c++;
   }

   // Check if this is a request for all readings or just one
   if( *c == '\0' )
   {
      // All readings
      sonar = true;
      tled = true;
      tdet = true;
      wiperi = true;
      wiperv = true;
   }
   else
   {
      cmpVal = strncmp( c, "sonar", 5 );
      if( cmpVal == 0 )
      {
	 // Get the sonar value
	 sonar = true;
      }

      cmpVal = strncmp( c, "tled", 4 );
      if( cmpVal == 0 )
      {
	 // Get the turbidity LED power voltage
	 tled = true;
      }

      cmpVal = strncmp( c, "tdet", 4 );
      if( cmpVal == 0 )
      {
	 // Get the turbidity photo detector voltage
	 tdet = true;
      }

      cmpVal = strncmp( c, "wiperi", 6 );
      if( cmpVal == 0 )
      {
	 // Get the wiper motor current reading
	 // (voltage across sense resistor)
	 wiperi = true;
      }

      cmpVal = strncmp( c, "wiperv", 6 );
      if( cmpVal == 0 )
      {
	 // Get the wiper motor voltage
	 wiperv = true;
      }

      cmpVal = strncmp( c, "help", 4 );
      if( cmpVal == 0 )
      {
	 // Get the wiper motor voltage
	 help = true;
      }
   } // end of checking what particular value was requested

   if( sonar )
   {
      reading = adc_get_reading( ADC_SONAR, true );
      value = (double) reading / (double) ADC_MAX_VALUE * (double) ADC_MAX_VOLTAGE;
      printf( "RXECHO pin:    %5.3lfV (0x%4.4lx)\r\n", value, reading );
   } // end of case sonar

   if( tled )
   {

      reading = adc_get_reading( ADC_TURB_PWR, true );
      value = (double) reading / (double) ADC_MAX_VALUE * (double) ADC_MAX_VOLTAGE;
      printf( "LED_V pin:     %5.3lfV (0x%4.4lx)\r\n", value, reading );
   } // end of case tled

   if( tdet )
   {
      reading = adc_get_reading( ADC_TURB_RCV, true );
      value = (double) reading / (double) ADC_MAX_VALUE * (double) ADC_MAX_VOLTAGE;
      printf( "TURB_V pin     %5.3lfV (0x%4.4lx)\r\n", value, reading );
   } // end of case tdet

   if( wiperi )
   {
      reading = adc_get_reading( ADC_WIPER_I, true );
      value = (double) reading / (double) ADC_MAX_VALUE * (double) ADC_MAX_VOLTAGE;
      printf( "MOTOR_I pin is %5.3lfV (0x%4.4lx)", value, reading );

      value = value / (double) ADC_MOTOR_SENSE_VALUE * 1000;

      printf( ", which is %lf mA\r\n", value );
   } // end of case wiperi

   if( wiperv )
   {
      reading = adc_get_reading( ADC_WIPER_V, true );
      value = (double) reading / (double) ADC_MAX_VALUE * (double) ADC_MAX_VOLTAGE;
      printf( "MOTOR_V pin is %5.3lfV (0x%4.4lx)\r\n", value, reading );
   } // end of case wiperv

   if( help )
   {
      printf( "adc - returns all readings \r\n" );
      printf( "\r\n" );
      printf( "adc sonar returns current value at sonar ADC input \r\n" );
      printf( "\r\n" );
      printf( "adc tled returns current value at Turbidity LED driver\r\n" );
      printf( "adc tdet returns current value at Turbidity photo detector input\r\n" );
      printf( "\r\n" );
      printf( "adc wiperi returns current value at wiper motor current input\r\n" );
      printf( "adc wiperv returns current value at wiper motor voltage input\r\n" );
      printf( "\r\n" );
   }

   if( !wiperv && !wiperi && !tdet && !tled && !sonar && !help )
   {
      // Congrats!  You have won 'stump the parser'
      printf( "Unable to parse command \"%s\"\r\n", msg );
   }

   return;
} // end of cmd_adc()

/*
 **********************************************************************
 * @brief cmd_dac() sets the output voltage for the RXGAIN signal
 *
 * @param msg - complete message from the debug console
 *
 * dac - returns all readings
 *
 * dac {abcd} sets RXGAIN output level
 *
 * dac sweep {start} {stop} {times} set the RXGAIN output level
 * 	to values between the {start} and {stop} parameter for
 * 	{times} loops
 *
 * @return none
 **********************************************************************
 */
void cmd_dac( char     *msg )
{
   char 		*c;
   int			cmpVal;
   uint32_t		reading;
   double		value;
   HAL_StatusTypeDef	val;
   uint32_t		data;

   int32_t		start;
   int32_t		stop;
   int32_t		times;

   bool			done = false;

   if( msg == NULL )
   {
      return;
   }

   // make the whole string lower case so later processing is easier
   c = msg;
   while( *c != '\0' )
   {
      *c = tolower(*c);
      c++;
   }

   c = msg;

   // should be pointing at d of dac

   // move to the end of the current token
   while( (*c != '\0') && (*c != ' ') )
   {
      c++;
   }

   if( *c != '\0' )
   {
      // Jump past the space
      c++;
   }

   // Check if this is a request for current setting or a new setting
   if( *c == '\0' )
   {
      // Display current setting value
      done = true;
      reading = HAL_DAC_GetValue( &hdac1, DAC_CHANNEL_1 );
      value = dac_voltage( reading );
      if( value < DAC_MIN_VOLTAGE )
      {
         value = (double)DAC_MIN_VOLTAGE;
      }
      printf( "RXGAIN is set to be %5.3lfV (0x%4.4lx)\r\n", value, reading );
   }
   else if( *c == 'h' )
   {
      cmpVal = strncmp( c, "help", 4 );
      if( cmpVal == 0 )
      {
	 // It is a help command
	 done = true;
	 printf( "dac					return current output setting\r\n" );
	 printf( "dac {abcd}				set output to abcd\r\n" );
	 printf( "dac sweep {start} {stop} {times} 	set the RXGAIN output level\r\n" );
	 printf( "				 	to values between the {start}\r\n" );
         printf( "					and {stop} parameter for {times} loops\r\n");
	 printf( "\r\n" );
      }
   }
   else if( *c == 's' )
   {
      // It might be a sweep command
      cmpVal = strncmp( c, "sweep", 5 );
      if( cmpVal == 0 )
      {
	 // It is a sweep command

	 // move to the end of the current token
	 while( (*c != '\0') && (*c != ' ') )
	 {
	    c++;
	 }

	 if( *c != '\0' )
	 {
	    // Jump past the space
	    c++;
	 }

	 start = strtol( c, &c, 0 );

	 if( *c != '\0' )
	 {
	    // Jump past the space
	    c++;
	 }

	 stop = strtol( c, &c, 0 );

	 if( *c != '\0' )
	 {
	    // Jump past the space
	    c++;
	 }

	 times = strtol( c, NULL, 0 );

	 if( start < 0 )
	 {
	    // Bad input
	    done = true;
	    printf("{start} value of %ld is too small\r\n", start );
	 }

	 if( stop < 0 )
	 {
	    // Bad input
	    done = true;
	    printf("{stop} value of %ld is too small\r\n", stop );
	 }

	 if( times < 0 )
	 {
	    // Bad input
	    done = true;
	    printf("{times} value of %ld is too small\r\n", times );
	 }

	 if( start > DAC_MAX_VALUE )
	 {
	    // Bad input
	    start = DAC_MAX_VALUE;
	    printf("{start} value limited to %ld\r\n", start );
	 }

	 if( stop > DAC_MAX_VALUE )
	 {
	    // Bad input
	    stop = DAC_MAX_VALUE;
	    printf("{stop} value limited to %ld\r\n", stop );
	 }

	 if( start > stop )
	 {
	    done = true;
	    printf("{start} needs to be smaller than {stop}\r\n" );
	 }

	 if( !done )
	 {
	    done = true;
	    printf( "Ramping RXGAIN from setting %ld to %ld for %ld loops\r\n",
		     start,
		     stop,
		     times );
	    for( int j = 0; j < times; j++ )
	    {
	       printf( "Loop %d of %ld\r\n", j, times );
	       for( int i = start; i < stop; i++ )
	       {
		  val = HAL_DAC_SetValue( &hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		  if( val != HAL_OK )
		  {
		     printf( "HAL_DAC_SetValue() failed in %s at line %d with error %d\r\n",
			      __FILE__, __LINE__, val );
		  }
		  HAL_Delay(10); 	// 10 ms
	       }
	    }
	 }

      }

   }
   else
   {
      // set the output
      done = true;

      data = strtol( c, NULL, 0 );
      if( data > DAC_MAX_VALUE )
      {
	 printf( "%ld (0x%4.4lx) is too large.  Set to %d (0x%4.4x)",
		  data, data,
		  DAC_MAX_VALUE, DAC_MAX_VALUE );
	 data = DAC_MAX_VALUE;
      }



      val = HAL_DAC_SetValue( &hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, data);
      if( val != HAL_OK )
      {
	 printf( "HAL_DAC_SetValue() failed in %s at line %d with error %d\r\n",
		  __FILE__, __LINE__, val );
      }
      else
      {
	 printf("DAC set %ld (0x%lx)\r\n", data, data );
      }
   }

   if( !done )
   {
      // Congrats!  You have won 'stump the parser'
      printf( "Unable to parse command \"%s\"\r\n", msg );
   }

   return;
} // cmd_dac()

/*
 **********************************************************************
 * @brief cmd_ping() send and receive a sonar ping
 *
 * @param msg - complete message from the debug console
 *
 * ping - send and receive a sonar ping
 *
 * @return none
 **********************************************************************
 */
void cmd_ping( char     *msg )
{
   char 		*c;


   bool			done = false;

   if( msg == NULL )
   {
      return;
   }

   // make the whole string lower case so later processing is easier
   c = msg;
   while( *c != '\0' )
   {
      *c = tolower(*c);
      c++;
   }

   c = msg;

   // should be pointing at p of ping

   // move to the end of the current token
   while( (*c != '\0') && (*c != ' ') )
   {
      c++;
   }

   if( *c != '\0' )
   {
      // Jump past the space
      c++;
   }

   done = true;

   ping( msg );

   if( !done )
   {
      // Congrats!  You have won 'stump the parser'
      printf( "Unable to parse command \"%s\"\r\n", msg );
   }

   return;
} // cmd_ping()

/*
 **********************************************************************
 * @brief cmd_id() display the processor based unique device ID
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 **********************************************************************
 */
void cmd_id( char     *msg )
{
   uint32_t		first;
   uint32_t		second;
   uint32_t		third;

   first = HAL_GetUIDw0();
   second = HAL_GetUIDw1();
   third = HAL_GetUIDw2();

   printf( "Device ID is: 0x%8.8lx %8.8lx %8.8lx\r\n", first, second, third );


   return;
} // end of cmd_id()

/*
 **********************************************************************
 * @brief cmd_motor() control the motor output pin
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 *
 * motor help	-- get help
 * motor on	-- turn on motor output drive
 * motor off	-- turn off motor output drive
 * motor {x}	-- rotate wiper {x} times
 * motor	-- get motor output drive state
 *
 **********************************************************************
 */
void cmd_motor( char     *msg )
{
   char 		*c;
   GPIO_PinState	pinState;
   int			cmpVal;

   uint32_t		revs;

   bool			done = false;

   if( msg == NULL )
   {
      return;
   }

   // make the whole string lower case so later processing is easier
   c = msg;
   while( *c != '\0' )
   {
      *c = tolower(*c);
      c++;
   }

   c = msg;

   // should be pointing at m of motor

   // move to the end of the current token
   while( (*c != '\0') && (*c != ' ') )
   {
      c++;
   }

   if( *c != '\0' )
   {
      // Jump past the space
      c++;
   }

   // c should point to a NULL or 'h' or 'o'
   if( *c == '\0' )
   {
      // This is a request for current motor state
      done = true;

      printf("WIPER_MOTOR output drive is ");
      pinState = HAL_GPIO_ReadPin( WIPER_MOTOR_GPIO_Port, WIPER_MOTOR_Pin );
      if( pinState == GPIO_PIN_SET )
      {
	 printf( "on\r\n" );
      }
      else
      {
	 printf( "off\r\n" );
      }
   }
   else
   {
      if( *c == 'o' )
      {
	 // Either on or off
	 cmpVal = strncmp( c, "on", 2 );
	 if( cmpVal == 0 )
	 {
	    // Turn it on
	   done = true;
	   GPIO_wiper_on();

	 }

	 cmpVal = strncmp( c, "off", 3 );
	 if( cmpVal == 0 )
	 {
	    // Turn it off
	   done = true;
	   GPIO_wiper_off();
	 }
      }
      else if( *c == 'h' )
      {
	 // Could be help
	 cmpVal = strncmp( c, "help", 4 );
	 if( cmpVal == 0 )
	 {
	    // It was help
	    done = true;
	    printf( "motor blip -- turn motor on briefly\r\n" );
	    printf( "motor help	-- get help\r\n" );
	    printf( "motor home -- move wiper to home position\r\n" );
	    printf( "motor on	-- turn on motor output drive\r\n" );
	    printf( "motor off	-- turn off motor output drive\r\n" );
	    printf( "motor {x}	-- rotate wiper {x} times\r\n" );
	    printf( "motor	-- get motor output drive state\r\n" );
	    printf( "\r\n\r\n" );
	 }
	 else
	 {
	    // Could be home
	    cmpVal = strncmp( c, "home", 4 );
	    if( cmpVal == 0 )
	    {
	       wiper_home();
	       done = true;
	    }
	 }
      }
      else if( *c == 'b' )
      {
	 // could be blip
	 cmpVal = strncmp( c, "blip", 4 );
	 if( cmpVal == 0 )
	 {
	    // It was blip
	    blip_wiper();
	    done = true;
	 }
      }
      else
      {
	 // Maybe it is a motor {x} command
	 revs = strtol( c, &c, 0 );
	 if( revs > 0 )
	 {
	    spin_wiper( revs );
	    done = true;
	 }
	 else
	 {
	    // No idea what this is so do not set done
	 }
      }
   }

   if( !done )
   {
      // Congrats!  You have won 'stump the parser'
      printf( "Unable to parse command \"%s\"\r\n", msg );
   }

   return;
} // end of cmd_motor()

/*
 **********************************************************************
 * @brief cmd_turbidity() check current turbidity
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 *
 **********************************************************************
 */
void cmd_turbidity( char     *msg )
{

   int32_t		light_reading;
   int32_t		dark_reading;

   int32_t		iDarkValue;
   int32_t		iLightValue;

   double		dDarkValue;
   double		dLightValue;

   int16_t		tValue;

   TURB_DATA_T		*t_ptr;

   t_ptr = getTurbidityPtr();

   // Turn on Turbidity LED for 2 ms
   GPIO_turbidity_led_on();

   HAL_Delay( 2 );

   // Measure light level
   light_reading = adc_get_reading( ADC_TURB_RCV, false );

   // Turn off Turbidity LED
   GPIO_turbidity_led_off();

   // Measure light level
   HAL_Delay( 1 );
   dark_reading = adc_get_reading( ADC_TURB_RCV, false );

   // Calculate result
   if(dark_reading > 597)   // 597 is cross point of formula below
   {	/* Apply correction formula to compensate for DC Blocker in H/W --> Makes amplification constant */
      dDarkValue = dark_reading;
      dDarkValue = 2.0668 * dDarkValue - 636.91;
      if(dDarkValue > 4095)	// Ensure it doesn't exceed limits
      {
	 dDarkValue = 4095;
      }
      iDarkValue = dDarkValue;
   }

   if(light_reading > 597)  // 597 is cross point of formula below
   {	/* Apply correction formula to compensate for DC Blocker in H/W --> Makes amplification constant */
      dLightValue = light_reading;
      dLightValue = 2.0668 * dLightValue - 636.91;
      if(dLightValue > 4095)	// Ensure it doesn't exceed limits
      {
	 dLightValue = 4095;
      }
      iLightValue = dLightValue;
   }

   // Magical math from original code
   if(iDarkValue < 50)
   {
      iDarkValue = 0;
   }
   else
   {
      iDarkValue -= 50;
   }

   if(iLightValue > iDarkValue)
   {	// if subtracting dark from light gives a positive value..
      tValue = iLightValue - iDarkValue;	// compute ambinent corrected value value
      // Check bounds
      buildAverageTurbidity( t_ptr, tValue );
   }

   printf("light %ld, dark %ld, difference %d\r\n", light_reading, dark_reading, tValue );
   displayTurbidity( t_ptr );

   return;
} // end of cmd_turbidity()

/*
 **********************************************************************
 * @brief cmd_accel() read the accelerometer
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 *
 * accel {n}	- get X, Y, Z results for 'n' readings
 * accel help	- accel specific help
 * accel id	- get device ID
 * accel test	- get self test results
 *
 *
 **********************************************************************
 */
void cmd_accel( char     *msg )
{

   ACCEL_DATA_T		xyzData;
   HAL_StatusTypeDef	retVal;
   bool			isLevel;
   int			cmpVal;
   char			*c;
   bool			done;
   uint16_t		count;
   uint16_t		i;

   if( msg == NULL )
   {
      return;
   }

   // make the whole string lower case so later processing is easier
   c = msg;
   while( *c != '\0' )
   {
      *c = tolower(*c);
      c++;
   }

   c = msg;

   // should be pointing at a of accel

   // move to the end of the current token
   while( (*c != '\0') && (*c != ' ') )
   {
      c++;
   }

   if( *c != '\0' )
   {
      // Jump past the space
      c++;
   }

   // c should point to a NULL or 'i' or 't' or 'h'
   if( *c == '\0' )
   {
      // This is a request for current accel readings
      done = true;

      retVal = accel_get_xyz( &hi2c1, &xyzData );
      if( retVal != HAL_OK )
      {
	 printf( "Error in accel_get_xyz() in %s at %d\r\n", __FILE__, __LINE__ );
      }
      else
      {
	 isLevel = accel_is_level( &xyzData );
	 if( isLevel )
	 {
	    printf( "Sensor appears to be level\r\n" );
	 }
	 else
	 {
	    printf( "Sensor is not level\r\n" );
	 }
	 accel_display_data( &xyzData, true, false );
      }
   }
   else
   {
      if( *c == 'i' )
      {
	 // Might be id
	 cmpVal = strncmp( c, "id", 2 );
	 if( cmpVal == 0 )
	 {
	    // It is id
	    done = true;
	    accel_check_id( &hi2c1 );
	 }
      }
      else if( *c == 'h')
      {
	 // Might be help
	 cmpVal = strncmp( c, "help", 4 );
	 if( cmpVal == 0 )
	 {
	    // It is help
	    done = true;

	    printf( "accel {n}  - get X, Y, Z results for 'n' readings\r\n" );
            printf( "accel help - accel specific help\r\n" );
       	    printf( "accel id   - get device ID\r\n" );
            printf( "accel test - get self test results\r\n" );
	    printf( "\r\n" );
	 }
      }
      else if( *c == 't' )
      {
	 // Might be test
	 cmpVal = strncmp( c, "test", 4 );
	 if( cmpVal == 0 )
	 {
	    done = true;

	    retVal = accel_selftest( &hi2c1 );
	    if( retVal != HAL_OK )
	    {
	       printf( "Error in accel_get_xyz() in %s at %d\r\n", __FILE__, __LINE__ );
	    }
	 }
      }
      else
      {
	 count = strtol( c, NULL, 0 );
	 done = true;
	 retVal = accel_get_xyz( &hi2c1, &xyzData );
	 if( retVal != HAL_OK )
	 {
	    printf( "Error in accel_get_xyz() in %s at %d\r\n", __FILE__, __LINE__ );
	 }
	 accel_display_data( &xyzData, true, false );
	 for( i = 0; i < count; i++ )
	 {
	    retVal = accel_get_xyz( &hi2c1, &xyzData );
	    if( retVal != HAL_OK )
	    {
	       printf( "Error in accel_get_xyz() in %s at %d\r\n", __FILE__, __LINE__ );
	    }
	    accel_display_data( &xyzData, false, false );
	    HAL_Delay( 500 );
	 }
      }
   }

   if( !done )
   {
      // Congrats!  You have won 'stump the parser'
      printf( "Unable to parse command \"%s\"\r\n", msg );
   }

   return;
} // end of cmd_accel()

/*
 **********************************************************************
 * @brief cmd_tmp() display CPU temperature
 *
 * @param msg - complete message from the debug console
 *
 * @return none
 *
 *
 *
 **********************************************************************
 */
void cmd_temp( char     *msg )
{

   printf( "STM32H523 temperature is %ld C\r\n", SSP_status.cpu_temperature );

   return;
} // end of cmd_temp()
//end of file console.c
