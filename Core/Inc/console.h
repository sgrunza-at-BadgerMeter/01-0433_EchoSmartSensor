/**
 **********************************************************************
 * @file console.h
 *
 * @brief This file holds the definitions and structures for the
 * 	console handling code
 *
 * @author grunzas
 *
 * @date Sep 15, 2025
 *
 **********************************************************************
 */
#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_

#include "tx_api.h"


#define         MAX_Qd_CMDS     5

typedef void (*CMD_HANDLER_T)(char *);

// Define the structure of the table of known commands
typedef struct
{
   const char *      pCmd;		/**< ptr to name of the command	*/
   CMD_HANDLER_T     cmd_handler;	/**< ptr to the function */
   const char *      descr;		/**< ptr to descriptive text about the command */
} CMD_INFO_T;


void
   threadConsole(
      ULONG pvParameters );

TX_QUEUE
   consoleGetCmdQ(
      void );

TX_THREAD *
   cmd_get_threadID(
      void );

// Define the known commands

#ifdef INCLUDE_FAULT_CMD
void cmd_fault( char *);
#endif

void cmd_configure( char * );
void cmd_help( char     *  );
void cmd_reset( char *);
void cmd_swver( char *);
void cmd_getTick( char *);
void cmd_led( char *);
void cmd_rs485( char * );
void cmd_adc( char * );
void cmd_dac( char * );
void cmd_ping( char * );
void cmd_id( char * );
void cmd_motor( char * );
void cmd_turbidity( char * );
void cmd_accel( char * );
void cmd_temp( char * );


// Define the message processing function
void process_msg( char *   );


#endif /* INC_CONSOLE_H_ */

//end of file console.h
