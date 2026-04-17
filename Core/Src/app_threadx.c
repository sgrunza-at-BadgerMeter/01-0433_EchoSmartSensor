/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "main.h"
#include <stdio.h>
#include "system_io.h"
#include "mcp.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TX_THREAD tx_app_thread;
TX_QUEUE tx_app_msg_queue;
/* USER CODE BEGIN PV */

static TX_THREAD	tx_console_thread;

#define CONSOLE_STACK_SZ	(1024*3)
uint8_t	consoleStack[ CONSOLE_STACK_SZ ];

static TX_THREAD	rs485_rx_thread;

#define RS485_RX_STACK_SZ	1024
uint8_t	rs485RXstack[ RS485_RX_STACK_SZ ];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */
  CHAR *pointer;

  /* Allocate the stack for tx app thread  */
  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
  /* Create tx app thread.  */
  if (tx_thread_create(&tx_app_thread, "tx app thread", tx_app_thread_entry, 0, pointer,
                       TX_APP_STACK_SIZE, TX_APP_THREAD_PRIO, TX_APP_THREAD_PREEMPTION_THRESHOLD,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }
  /* Allocate the stack for mcp_cmd_queue.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       TX_APP_MSG_QUEUE_FULL_SIZE * sizeof(ULONG), TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
  /* Create mcp_cmd_queue.  */
  if (tx_queue_create(&tx_app_msg_queue, "mcp_cmd_queue", TX_APP_SINGLE_MSG_SIZE,
                      pointer, TX_APP_MSG_QUEUE_FULL_SIZE * sizeof(ULONG)) != TX_SUCCESS)
  {
    return TX_QUEUE_ERROR;
  }

  /* USER CODE BEGIN App_ThreadX_Init */
  /* USER CODE END App_ThreadX_Init */

  return ret;
}
/**
  * @brief  Function implementing the tx_app_thread_entry thread.
  * @param  thread_input: Hardcoded to 0.
  * @retval None
  */
void tx_app_thread_entry(ULONG thread_input)
{
  /* USER CODE BEGIN tx_app_thread_entry */

   uint8_t	retVal;

   // Create RS485_RX_Thread
   retVal = tx_thread_create(
	       &rs485_rx_thread,		// thread control block pointer
	       "rs485_rx_Thread",		// thread name string
	       rs485_rx,			// thread entry function
	       0,				// 32-bit input value to function
	       rs485RXstack,			// pointer to start of stack
	       RS485_RX_STACK_SZ,	 	// stack size in bytes
	       TX_CONSOLE_THREAD_PRIO,		// thread priority
	       TX_APP_THREAD_PREEMPTION_THRESHOLD,	// preemption threshold
	       TX_APP_THREAD_TIME_SLICE, 	// thread time-slice value
	       TX_APP_THREAD_AUTO_START );	// automatic start selection
   if ( retVal != TX_SUCCESS)
   {
      printf( "tx_thread_create() failed for rs485_rx() thread\r\n" );
      return;
   }

   // Create threadConsole
   retVal = tx_thread_create(
	       &tx_console_thread,		// thread control block pointer
	       "consoleThread",			// thread name string
	       threadConsole,			// thread entry function
	       0,				// 32-bit input value to function
	       consoleStack,			// pointer to start of stack
	       CONSOLE_STACK_SZ,	 	// stack size in bytes
	       TX_CONSOLE_THREAD_PRIO,		// thread priority
	       TX_APP_THREAD_PREEMPTION_THRESHOLD,	// preemption threshold
	       TX_APP_THREAD_TIME_SLICE, 	// thread time-slice value
	       TX_APP_THREAD_AUTO_START );	// automatic start selection
   if ( retVal != TX_SUCCESS)
   {
      printf( "tx_thread_create() failed for threadConsole thread\r\n" );
      return;
   }

   // Create Master Control Process a.k.a MCP_Thread
   retVal = tx_thread_create(
	       &tx_mcp_thread,			// thread control block pointer
	       "mcp",				// thread name string
	       mcp_thread_start,		// thread entry function
	       0,				// 32-bit input value to function
	       mcpStack,			// pointer to start of stack
	       MCP_STACK_SZ,	 		// stack size in bytes
	       TX_MCP_THREAD_PRIO,		// thread priority
	       TX_APP_THREAD_PREEMPTION_THRESHOLD,	// preemption threshold
	       TX_APP_THREAD_TIME_SLICE, 	// thread time-slice value
	       TX_APP_THREAD_AUTO_START );	// automatic start selection
   if ( retVal != TX_SUCCESS)
   {
      printf( "tx_thread_create() failed for threadConsole thread\r\n" );
      return;
   }


   // If all the above threads start and are working,
   // then go to sleep for a while, then check if all the
   // other threads are still ok.  This would be a good
   // place to tickle the watchdog if all the tasks are ok.

  /* USER CODE END tx_app_thread_entry */
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN Before_Kernel_Start */

  /* USER CODE END Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN Kernel_Start_Error */

  /* USER CODE END Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
/*
 **********************************************************************
 * @brief getID_tx_app_thread_entry() returns it's Thread ID, thus
 * avoiding needing a globabl variable
 *
 * @param ulParameters - unused
 *
 * @return Pointer to ThreadID structure
 **********************************************************************
 */
TX_THREAD *
   getID_tx_app_thread_entry(
      void )
{
   return( &tx_app_thread );
} // end of getID_tx_app_thread_entry()

/*
 **********************************************************************
 * @brief getID_tx_console_thread() returns it's Thread ID, thus
 * avoiding needing a globabl variable
 *
 * @param ulParameters - unused
 *
 * @return Pointer to ThreadID structure
 **********************************************************************
 */
TX_THREAD *
   getID_tx_console_thread(
      void )
{
   return( &tx_console_thread );
} // end of getID_tx_console_thread()

/*
 **********************************************************************
 * @brief getID_rs485_rx_thread() returns it's Thread ID, thus
 * avoiding needing a globabl variable
 *
 * @param none
 *
 * @return Pointer to ThreadID structure
 **********************************************************************
 */
TX_THREAD *
   getID_rs485_rx_thread(
      void )
{
   return( &rs485_rx_thread );
} // end of getID_rs485_rx_thread()

/* USER CODE END 1 */
