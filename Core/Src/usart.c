/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
#include <string.h>

#include "system_io.h"
#include "fifo.h"
#include "console.h"

#include "app_threadx.h"

// USART1 is used for a debug console interface.  Reception is done by interrupt as
// one-character per interrupt

// UART4 is used for the system RS-485 interface


// Defined in console.c
extern  volatile bool	msg_to_process;

// Defined in console.c
extern FIFO_T		console_rx_buf;

// This is the character into which the console debug message is received,
// one byte at a time controlled by interrupts
uint8_t			rx_byte;

// This is the memory area into which the RS485 message is received
uint8_t			rs485_msg_buffer[ MAX_FIFO_ENTRY_LEN ];

uint16_t		rs485_msg_buffer_len;

// Define max length of a debug console message
#define 		MAX_DBG_CONSOLE_MSG	80

// Defined in system_io.c
extern FIFO_T		rs485_buffer;

// Flag used to indicate thread's status
extern bool		rs485_thread_sleeping;

// Defined in system_io.c
extern RS485_TRANSMIT_MSG_T	transmit_msg;

/* USER CODE END 0 */

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
DMA_HandleTypeDef handle_GPDMA1_Channel1;
DMA_HandleTypeDef handle_GPDMA1_Channel0;

/* UART4 init function */
void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_RS485Ex_Init(&huart4, UART_DE_POLARITY_HIGH, 16, 8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_EnableFifoMode(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}
/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART4;
    PeriphClkInitStruct.Uart4ClockSelection = RCC_UART4CLKSOURCE_CSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* UART4 clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**UART4 GPIO Configuration
    PB14     ------> UART4_DE
    PA12     ------> UART4_TX
    PB8     ------> UART4_RX
    */
    GPIO_InitStruct.Pin = RS485_DE_Pin|RS485_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = RS485_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_UART4;
    HAL_GPIO_Init(RS485_TX_GPIO_Port, &GPIO_InitStruct);

    /* UART4 DMA Init */
    /* GPDMA1_REQUEST_UART4_TX Init */
    handle_GPDMA1_Channel1.Instance = GPDMA1_Channel1;
    handle_GPDMA1_Channel1.Init.Request = GPDMA1_REQUEST_UART4_TX;
    handle_GPDMA1_Channel1.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    handle_GPDMA1_Channel1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    handle_GPDMA1_Channel1.Init.SrcInc = DMA_SINC_INCREMENTED;
    handle_GPDMA1_Channel1.Init.DestInc = DMA_DINC_FIXED;
    handle_GPDMA1_Channel1.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
    handle_GPDMA1_Channel1.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
    handle_GPDMA1_Channel1.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
    handle_GPDMA1_Channel1.Init.SrcBurstLength = 1;
    handle_GPDMA1_Channel1.Init.DestBurstLength = 1;
    handle_GPDMA1_Channel1.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT1|DMA_DEST_ALLOCATED_PORT0;
    handle_GPDMA1_Channel1.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    handle_GPDMA1_Channel1.Init.Mode = DMA_NORMAL;
    if (HAL_DMA_Init(&handle_GPDMA1_Channel1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle, hdmatx, handle_GPDMA1_Channel1);

    if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel1, DMA_CHANNEL_NPRIV) != HAL_OK)
    {
      Error_Handler();
    }

    /* GPDMA1_REQUEST_UART4_RX Init */
    handle_GPDMA1_Channel0.Instance = GPDMA1_Channel0;
    handle_GPDMA1_Channel0.Init.Request = GPDMA1_REQUEST_UART4_RX;
    handle_GPDMA1_Channel0.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    handle_GPDMA1_Channel0.Init.Direction = DMA_PERIPH_TO_MEMORY;
    handle_GPDMA1_Channel0.Init.SrcInc = DMA_SINC_FIXED;
    handle_GPDMA1_Channel0.Init.DestInc = DMA_SINC_INCREMENTED;
    handle_GPDMA1_Channel0.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_BYTE;
    handle_GPDMA1_Channel0.Init.DestDataWidth = DMA_DEST_DATAWIDTH_BYTE;
    handle_GPDMA1_Channel0.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
    handle_GPDMA1_Channel0.Init.SrcBurstLength = 1;
    handle_GPDMA1_Channel0.Init.DestBurstLength = 1;
    handle_GPDMA1_Channel0.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT1;
    handle_GPDMA1_Channel0.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    handle_GPDMA1_Channel0.Init.Mode = DMA_NORMAL;
    if (HAL_DMA_Init(&handle_GPDMA1_Channel0) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle, hdmarx, handle_GPDMA1_Channel0);

    if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel0, DMA_CHANNEL_NPRIV) != HAL_OK)
    {
      Error_Handler();
    }

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_CSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();

    /**UART4 GPIO Configuration
    PB14     ------> UART4_DE
    PA12     ------> UART4_TX
    PB8     ------> UART4_RX
    */
    HAL_GPIO_DeInit(GPIOB, RS485_DE_Pin|RS485_RX_Pin);

    HAL_GPIO_DeInit(RS485_TX_GPIO_Port, RS485_TX_Pin);

    /* UART4 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* UART4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/*
 **********************************************************************
 * @brief HAL_UART_RxCpltCallback() handles receive interrupts
 *
 * There is a msg_to_process flag set at interrupt level when a complete
 * message has been received.
 *
 * @param huart - prt to UART structure
 *
 * @return none
 *
 * @note Currently using a single location to receive the byte and
 * then copying the byte, then re-using the location.  Could change
 * to using the correct location in the receive FIFO and avoid a
 * single byte copy.
 *
 **********************************************************************
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

   // Define working storage for the debug console message being received
   static char			dbg_msg[ MAX_DBG_CONSOLE_MSG ];
   static uint8_t		i = 0;

   if (huart->Instance == USART1)
   {
      // USART1 -- debug console

      bool		storeChar;
      int8_t		retVal;

      // Default to saving the character
      storeChar = true;

      if( i == 0 )
      {
	 // No valid characters in the debug message so clear it
	 memset( dbg_msg, 0, MAX_DBG_CONSOLE_MSG );
      }

      // Handle Backspace
      if( rx_byte == '\b' )
      {
	 storeChar = false;

	 // Need to remove a character from the message
	 // being stored.
	 if( i > 0 )
	 {
	    dbg_msg[i] = '\0';
	    i--;

	    // Blocking transmit
	    printf( "\b \b" );
	    fflush( stdout );
	 }
      }

      if( (huart->ErrorCode & HAL_UART_ERROR_FE) == HAL_UART_ERROR_FE )
      {
	 // Framing error probably from a received BREAK
	 storeChar = false;
      }

      if( (i == (MAX_DBG_CONSOLE_MSG-1)) && (rx_byte != '\r') )
      {
	 // There is only room for the terminating carriage return
	 storeChar = false;
      }

      if( storeChar )
      {
	 // Send the character to the console for echo-back

	 // This is a blocking write -- which will tie up this ISR until a full character
	 // has been sent.
	 // HAL_UART_Transmit( &huart1, (uint8_t *)&rx_byte, 1, HAL_MAX_DELAY)
	 huart1.Instance->TDR = rx_byte;	// A little hacky but quick

	 // Append to current message
	 dbg_msg[i++] = rx_byte;

	 if( rx_byte == '\r' )
	 {
	    // Change the \r into a NULL terminating null
	    if( i > 1 )
	    {
	       dbg_msg[i-1] = '\0';
	    }

	    // Message is done so push onto the message FIFO
	    retVal = fifo_push( &console_rx_buf, (uint8_t *)dbg_msg, strlen(dbg_msg) );
	    if( retVal != FIFO_SUCCESS )
	    {
	       // Couldn't put the message on the FIFO
	       printf( "Error at line %d in file %s\r\n", __LINE__, __FILE__ );
	    }
	    i = 0;
	    memset( dbg_msg, 0, MAX_DBG_CONSOLE_MSG );
	    msg_to_process = true;
	    // Send wake up to console task
	    // TODO: tx_thread_resume( &uartThread );
	 }
      }

      /* Receive the next byte */
      console_receive_char();
   }
   else if (huart->Instance == UART4 )
   {
      // UART4 -- RS485 interface to Controller / Power Supply

      bool		goodMsg = true;

      int8_t		retVal;

      if( (huart->ErrorCode & HAL_UART_ERROR_FE) == HAL_UART_ERROR_FE )
      {
	 // Framing error probably from a received BREAK
	 goodMsg = false;
      }

      if( __HAL_UART_GET_FLAG(&huart4, UART_CLEAR_RTOF) )		// UART_CLEAR_RTOF
      {
	 printf( "HAL_UART_RxCpltCallback() RTO happened\r\n" );
      }

      if( goodMsg )
      {
	 // This message timed out so it is a complete message or an error
	 retVal = fifo_push(
		     &rs485_buffer, 		// ptr to control structure
		     rs485_msg_buffer,		// ptr to data received
		     huart->RxXferCount );	// number of bytes received
	 if( retVal != FIFO_SUCCESS )
	 {
	    // Couldn't put the message on the FIFO
	    printf( "fifo_push() error at line %d in file %s\r\n", __LINE__, __FILE__ );
	 }

	 // Make sure the receive processing thread is awake
	 if( rs485_thread_sleeping )
	 {
	    // Send wake up to RS485 msg processing task
	    TX_THREAD	*id;

	    id = getID_rs485_rx_thread();
	    tx_thread_resume( id );
	    rs485_thread_sleeping = false;
	 }
      }
      else
      {
	 // Maybe should keep a count of bad bytes received?
      }

      // Setup for next message
      memset( rs485_msg_buffer, 0, MAX_FIFO_ENTRY_LEN );

      // rs485_rx() will call rs485_receive_msg() if huart->RxState == HAL_UART_STATE_BUSY_RX

   } // end of if (huart->Instance == UART4 )

   return;
} // end of HAL_UART_RxCpltCallback()

/*
 **********************************************************************
 * @brief console_receive_char() receives a single character with
 * receive interrupt enabled
 *
 * @param none
 *
 * @return none
 *
 * @note Everything is done in other routines as side effects of this
 * function
 *
 * Defining this routine means the rx_byte variable and the uart4
 * variable aren't exposed to other functions.
 **********************************************************************
 */
void
   console_receive_char(
      void )
{

   HAL_UART_Receive_IT( &huart1, &rx_byte, 1);

   return;
} // end of console_receive_char()

/*
 **********************************************************************
 * @brief rs485_receive_msg() receives a Modbus-RTU message
 *
 * @param none
 *
 * @return none
 *
 * @note Everything is done in other routines as side effects of this
 * function
 *
 **********************************************************************
 */
HAL_StatusTypeDef
   rs485_receive_msg(
      void )
{
   HAL_StatusTypeDef		retVal;

   rs485_disable_tx();

   rs485_enable_rx();

   HAL_Delay( 10 );	// Give transceiver time to switch modes

   retVal = HAL_UARTEx_ReceiveToTimeout_DMA(
	    &huart4,
	    (uint8_t *) &rs485_msg_buffer,
	    MAX_FIFO_ENTRY_LEN,
	    35 );
   if( retVal != HAL_OK )
   {
      printf( "HAL_UARTEx_ReceiveToTimeout_DMA() failed with error %d in %s at %d\r\n",
	       retVal,
	       __FILE__,
	       __LINE__ );
      printf( "huart4.ErrorCode = %ld (0x%ld)\r\n", huart4.ErrorCode, huart4.ErrorCode );
      fflush( stdout );
   }

   return( retVal );
} // end of rs485_receive_msg()

/*
 **********************************************************************
 * @brief HAL_UART_TxCpltCallback() handles transmit interrupts
 *
 *
 * @param huart - prt to UART structure
 *
 * @return none
 *
 *
 **********************************************************************
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

   if (huart->Instance == USART1)
   {
      // USART1 -- debug console

   }
   else if (huart->Instance == UART4 )
   {
      // UART4 -- RS485 interface to Controller / Power Supply
      transmit_msg.tx_active = false;

      // Make sure the receive processing thread is awake
      if( rs485_thread_sleeping )
      {
	 // Send wake up to RS485 msg processing task
	 TX_THREAD	*id;

	 id = getID_rs485_rx_thread();
	 tx_thread_resume( id );
	 rs485_thread_sleeping = false;
      }

      //printf( "HAL_UART_TxCpltCallback() at line %d\r\n", __LINE__ );

   } // end of if (huart->Instance == UART4 )


   return;
} // end of HAL_UART_TxCpltCallback()

/*
 **********************************************************************
 * @brief HAL_UART_ErrorCallback() handles receive time out
 *
 *
 * @param huart - prt to UART structure
 *
 * @return none
 *
 *
 **********************************************************************
 */
void
   HAL_UART_ErrorCallback(
      UART_HandleTypeDef 		*huart )
{

   HAL_StatusTypeDef		retVal;
   bool				dataAvailable;
   uint32_t			length;

   if( huart->Instance == UART4 )
   {
      // This is the RS485 / Modbus UART
      dataAvailable = false;

      if( huart->ErrorCode == HAL_UART_ERROR_RTO )
      {
	 // This is a receiver time out, clear the interrupt
	 ATOMIC_SET_BIT( huart->Instance->ICR, USART_ICR_RTOCF );
	 dataAvailable = true;
      }

//      if( (huart->Instance->ISR & USART_ISR_IDLE) == USART_ISR_IDLE )
//      {
//	 // Receive idle
//	 dataAvailable = true;
//      }

      if( dataAvailable )
      {
	 length = MAX_FIFO_ENTRY_LEN - __HAL_DMA_GET_COUNTER(huart->hdmarx );

	 if( length != 0 )
	 {
	    retVal = fifo_push( &rs485_buffer, rs485_msg_buffer, length );
	    if( retVal != FIFO_SUCCESS )
	    {
	       // Couldn't put the message on the FIFO
	       printf( "fifo_push() error at line %d in file %s\r\n", __LINE__, __FILE__ );
	    }

	    // Make sure the receive processing thread is awake to process this msg
	    if( rs485_thread_sleeping )
	    {
	       // Send wake up to RS485 msg processing task
	       TX_THREAD	*id;

	       id = getID_rs485_rx_thread();
	       tx_thread_resume( id );
	       rs485_thread_sleeping = false;
	    }
	 }
      }
      // Setup for next message
      // TODO: remove the comment below
      //memset( rs485_msg_buffer, 0, MAX_FIFO_ENTRY_LEN );
   }

   //printf( "HAL_UART_ErrorCallback() at line %d\r\n", __LINE__ );

   return;
} // end of HAL_UART_ErrorCallback()

/**
  * @brief Receive an amount of data in DMA mode till either the expected number
  *        of data is received or an Receive Time Out event occurs.
  * @note  Reception is initiated by this function call. Further progress of reception is achieved thanks
  *        to DMA services, transferring automatically received data elements in user reception buffer and
  *        calling registered callbacks at half/end of reception. UART RTO events are also used to consider
  *        reception phase as ended. In all cases, callback execution will indicate number of received data elements.
  * @note  When the UART parity is enabled (PCE = 1), the received data contain
  *        the parity bit (MSB position).
  * @note  When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *        the received data is handled as a set of uint16_t. In this case, Size must indicate the number
  *        of uint16_t available through pData.
  * @note  When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *        address of user data buffer for storing data to be received, should be aligned on a half word frontier
  *        (16 bits) (as received data will be handled by DMA from halfword frontier). Depending on compilation chain,
  *        use of specific alignment compilation directives or pragmas might be required
  *        to ensure proper alignment for pData.
  * @param huart UART handle.
  * @param pData Pointer to data buffer (uint8_t or uint16_t data elements).
  * @param Size  Amount of data elements (uint8_t or uint16_t) to be received.
  * @param timeout	Number of bit times of idle before receive timeout
  * @retval HAL status
  */
HAL_StatusTypeDef
   HAL_UARTEx_ReceiveToTimeout_DMA(
      UART_HandleTypeDef 	*huart,
	  uint8_t 		*pData,
	  uint16_t 		Size,
	  uint16_t		timeout )
{
   HAL_StatusTypeDef status;

   /* Check that a Rx process is not already ongoing */
   if (huart->RxState == HAL_UART_STATE_READY)
   {
      if ((pData == NULL) || (Size == 0U))
      {
	 return HAL_ERROR;
      }

      /* In case of 9bits/No Parity transfer, pData buffer provided as input parameter
       should be aligned on a uint16_t frontier, as data copy from RDR will be
       handled by DMA from a uint16_t frontier. */
      if ((huart->Init.WordLength == UART_WORDLENGTH_9B) && (huart->Init.Parity == UART_PARITY_NONE))
      {
	 if ((((uint32_t)pData) & 1U) != 0U)
	 {
	    return  HAL_ERROR;
	 }
      }

      /* Set Reception type to reception till Receive Time Out Event*/
      huart->ReceptionType = HAL_UART_RECEPTION_TORTO;
      huart->RxEventType = HAL_UART_RXEVENT_TC;

      HAL_UART_ReceiverTimeout_Config( huart, timeout );

      HAL_UART_EnableReceiverTimeout( huart );

      status =  UART_Start_Receive_DMA(huart, pData, Size);

      /* Check Rx process has been successfully started */
      if (status == HAL_OK)
      {
	 if (huart->ReceptionType == HAL_UART_RECEPTION_TORTO)
	 {
	    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_RTOF);
	    ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_RTOIE);
	 }
	 else
	 {
	    /* In case of errors already pending when reception is started,
           Interrupts may have already been raised and lead to reception abortion.
           (Overrun error for instance).
           In such case Reception Type has been reset to HAL_UART_RECEPTION_STANDARD. */
	    status = HAL_ERROR;
	 }
      }

      return status;
   }
   else
   {
      return HAL_BUSY;
   }
} // end of HAL_UARTEx_ReceiveToTimeout_DMA()

//*********************************************************************
void
   display_message(
      uint8_t		*data,
      uint16_t		len )
{

   const uint8_t	COLS = 16;

   uint16_t		remain;

   uint16_t		cols;
   uint16_t		row;

   remain = len;
   row = 0;

   printf( "\r\n" );
   printf( "display_message( 0x%8.8lx, 0x%4.4x )\r\n", (uint32_t) data, len );
   while( remain )
   {
      cols = 0;
      printf( "0x%4.4x  ", row );
      while( (cols < COLS) && remain )
      {
	 if( cols != 0 )
	 {
	    printf( ", " );
	 }
	 printf( "%2.2x", *(data++) );
	 remain--;
	 cols++;
      }
      printf( "\r\n" );
      row++;
   }

   return;
} // end of display_message()

/* USER CODE END 1 */
