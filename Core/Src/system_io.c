/**
 **********************************************************************
 * @file system_io.c
 *
 * @brief This file handles communication with the system over the
 * RS-485 / Modbus RTU connection
 *
 * @author grunzas
 *
 * @date Oct 9, 2025
 *
 **********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "tx_api.h"
#include "app_threadx.h"
#include "modbus.h"
#include "system_io.h"
#include "usart.h"
#include "fifo.h"

// Status flag to show status of the RS485 message processing task
volatile bool	rs485_thread_sleeping = false;

// Console message reception buffer, plus space for the control structure QUEUE_T
FIFO_T				rs485_buffer;

// Data structure to hold message to be sent to RS485 system
RS485_TRANSMIT_MSG_T		transmit_msg;

// Data structure to hold RS-485 statistics counters
RS485_COMM_STATISTICS_T		rs485_stats;

// Defined in usart.c
extern UART_HandleTypeDef	huart4;

// Defined in tim.c
// used during debugging the Modbus stuff but also in sonar
extern TIM_HandleTypeDef 	htim5;
/*
 **********************************************************************
 * @brief rs485_rx() has a FIFO to receive MODBUS RTU messages from the
 * system.
 *
 * These messages are then routed to the correct destination
  *
 * @param ulParameters - unused
 *
 * @return none
 **********************************************************************
 */
void rs485_rx(ULONG thread_input)
{

   TX_THREAD		*myID;
   FIFO_ENTRY_T		buffer;
   MODBUS_ADU_T		*pADU;
   int8_t		ret;

   HAL_StatusTypeDef	retVal;

   // Initialize queue to store incoming message from UART
   fifo_init( &rs485_buffer );

   // Start the asynchronous reception process by receiving one message
   rs485_receive_msg();

   myID = getID_rs485_rx_thread();

   // Setup TIM5 to monitor Modbus timing
   htim5.Instance->CNT = 0;	// start counter at zero

   ret = HAL_TIM_Base_Start( &htim5 );
   if( ret != HAL_OK )
   {
      printf("HAL_TIM_Base_Start() failed in file %s at line %d\r\n",
	       __FILE__, __LINE__ );
      fflush( stdout );
   }

   while( 1 )
   {
      retVal = HAL_BUSY;

      // Currently using time-slicing so process messages until there are none left
      while( rs485_buffer.entries > 0 )
      {
	 pADU = (MODBUS_ADU_T *)&buffer;

	 ret = fifo_pop( &rs485_buffer,		// ptr to queue
			 (char *)pADU);		// ptr into which to put extracted command
	 if( ret == FIFO_SUCCESS )
	 {
	    if( pADU->fc != '\0' )
	    {
	       // NOTE: Function Code 0 is not valid
	       process_rs485_msg( pADU );
	    }
	 }

	 if( !transmit_msg.tx_active )
	 {
	    // Not currently transmitting
	    if (huart4.RxState == HAL_UART_STATE_READY)
	    {
	       // Not receiving so start a receive
	       retVal = rs485_receive_msg();
	    }
	 }
      } // end of while( messages left to process )

      if( retVal == HAL_OK )
      {
	 // Nothing to do so go to sleep
	 rs485_thread_sleeping = true;
	 tx_thread_suspend( myID );	// need to restart with tx_thread_resume()
      }
      else
      {
	 // Modbus receive didn't start so just wait a little
	 tx_thread_sleep( 100 );
      }

      // TODO:  Handle logic case where a message was transmitting while processing
      // RS485 messages, then this thread went to sleep, then the transmit completed
      //
      // What will wake this thread up to start another receive?
      //
      // A health check thread could detect this condition or the transmit complete
      // ISR could check for the condition

      if( !transmit_msg.tx_active )
      {
	 // Not currently transmitting
	 if (huart4.RxState != HAL_UART_STATE_BUSY_RX )
	 {
	    // Not busy with receiving so start a receive
	    rs485_receive_msg();
	 }
      }


   }

   return;
} // end of rs485_rx()

/*
 **********************************************************************
 * @brief process_rs485_msg() handles a single MODBUS / SSP message
 * system.
 *
 * @param ulParameters - unused
 *
 * @return none
 **********************************************************************
 */
void
   process_rs485_msg(
      MODBUS_ADU_T	*msg )
{

   printf( "RS485 msg received with address 0x%2.2x, fc = 0x%2.2x, payload = 0x%2.2x\r\n",
	 msg->address,
	 msg->fc,
	 msg->payload );

   return;
} // end of process_rs485_msg()

/*
 **********************************************************************
 * @brief rs485_prepare_tx_buf() prepares a buffer for sending a
 * message through the RS485 interface
 *
 * @param addr -address to use for message
 * @param fc - 	function code to put into message
 *
 * @return Either RS485_SUCCESS or RS485_FAILURE
 *
 **********************************************************************
 */
int8_t
   rs485_prepare_tx_buf(
      char		addr,
      char		fc )
{

   int8_t	ret;

   ret = RS485_SUCCESS;
   while( transmit_msg.tx_active )
   {
      ;	// NOP -- TODO:  should probably be a timeout check here
   }

   transmit_msg.tx_active = false;
   transmit_msg.index = 0;
   transmit_msg.crc = 0xFFFF;

   memset( &transmit_msg.msg[0], 0, RS485_MAX_MSG_LEN );

   ret = rs485_add_tx_byte( addr );
   if( ret == RS485_SUCCESS )
   {
      ret = rs485_add_tx_byte( fc );
   }

   return( ret );
} // end of rs485_tx_buf()

/*
 **********************************************************************
 * @brief rs485_add_tx_byte() adds a byte to the RS485 message currently
 * being assembled
 *
 * @param b - byte to add to message
 *
 * @return Either RS485_SUCCESS or RS485_FAILURE
 *
 **********************************************************************
 */
int8_t
   rs485_add_tx_byte(
      uint8_t		b )
{

   int8_t		ret;


   ret = RS485_SUCCESS;
   if( transmit_msg.index >= (RS485_MAX_MSG_LEN - 2) )
   {
      // No room left
      ret = RS485_FAILURE;
   }

   if( transmit_msg.tx_active )
   {
      // Currently sending this message so do not change it
      ret = RS485_FAILURE;
   }

   if( ret == RS485_SUCCESS )
   {
      // Add byte to buffer
      transmit_msg.msg[ transmit_msg.index++ ] = b;

      // Update the CRC
      calculate_crc16_modbus( &transmit_msg );
   }

   return( ret );
} // end of rs485_add_tx_byte()

/*
 **********************************************************************
 * @brief rs485_add_tx_word() adds a 16-bit word to the RS485 message
 * currently being assembled
 *
 * @param none
 *
 * @return Either RS485_SUCCESS or RS485_FAILURE
 *
 **********************************************************************
 */
int8_t
   rs485_add_tx_word(
      uint16_t		w )
{
   int8_t		ret;

   ret = rs485_add_tx_byte( (uint8_t)(w >> 8) & 0x00ff );
   if( ret == RS485_SUCCESS )
   {
      ret = rs485_add_tx_byte( (uint8_t)(w       & 0x00ff) );
   }

   return( ret );
} // end of rs485_add_tx_word()

/*
 **********************************************************************
 * @brief rs485_transmit_now() transmits the currently being assembled
 * message over the RS485 interface
 *
 * @param none
 *
 * @return none
 *
 * @notes Normal MODBUS / SSP interactions are the master (Controller)
 * sending a message to the slave (this or another sensor) and then
 * the master waits for the response.  There shouldn't be any active
 * receive data on the wire so it should be acceptable to just disable
 * the currently active receive without checking if the bus is busy
 **********************************************************************
 */
void
   rs485_transmit_now(
      void )
{
   HAL_StatusTypeDef	ret;

   HAL_UART_AbortReceive_IT( &huart4 );

   // Copy CRC to the end of the message
   transmit_msg.msg[transmit_msg.index++] = (transmit_msg.crc >> 8) & 0xFF;
   transmit_msg.msg[transmit_msg.index++] = (transmit_msg.crc & 0xFF);

   transmit_msg.tx_active = true;

   rs485_disable_rx();
   rs485_enable_tx();

   HAL_Delay( 1 );	// Give transceiver time to switch modes

   // Start transmission
   ret = HAL_UART_Transmit_IT( &huart4,
	    transmit_msg.msg,
	    transmit_msg.index );
   if( ret != HAL_OK )
   {
      // TODO: Error handling
   }

   return;
} // end of rs485_transmit_now()

/*
 **********************************************************************
 * @brief calculate_crc16_modbus() calculate CCITT CRC-16-MODBUS 16-bit
 * checksum
 *
 * @param p - ptr to transmit_msg structure
 *
 * @return none
 **********************************************************************
 */
void
   calculate_crc16_modbus(
      RS485_TRANSMIT_MSG_T	*p )
{

   uint8_t *data;
   uint16_t length;
   uint16_t crc = 0xFFFF; // Initial value for Modbus CRC-16
   uint16_t polynomial = 0xA001; // Reversed polynomial for Modbus CRC-16 (0x8005 reflected)

   data = (uint8_t *)(p->msg);
   length = p->index;

   for( uint16_t i = 0; i < length; i++ )
   {
      crc ^= data[i]; // XOR current byte with CRC
      for( uint8_t j = 0; j < 8; j++ )
      {
	 if (crc & 0x0001 )
	 {
	    // Check if LSB is 1
	    crc >>= 1; 		// Shift right
	    crc ^= polynomial; 	// XOR with polynomial
	 }
	 else
	 {
	    crc >>= 1; // Shift right
	 }
      }
   }

   p->crc = crc;

   return;
} // end of calculate_crc16_modbus()

/*
 **********************************************************************
 * @brief rs485_stat_clear() clears the statistic counter
  *
 * @param p - ptr to statistics structure
 *
 * @return none
 **********************************************************************
 */
void
   rs485_stat_clear(
      RS485_COMM_STATISTICS_T	*p )
{

   if( p == NULL )
   {
      return;
   }

   memset( p, 0, sizeof(RS485_COMM_STATISTICS_T) );

   return;
} // end of rs485_stat_clear()

/*
 **********************************************************************
 * @brief rs485_stat_print() clears the statistic counter
  *
 * @param p - ptr to statistics structure
 *
 * @return none
 **********************************************************************
 */
void
   rs485_stat_print(
      RS485_COMM_STATISTICS_T	*p )
{

   if( p == NULL )
   {
      return;
   }

   printf( "Frames received:  %ld \r\n", p->recvd_frames );
   printf( "Frames sent:      %ld\r\n", p->tx_frames );
   printf( "Rcv frame errors: %ld\r\n", p->rx_frame_errors );
   printf( "\r\n" );

   return;
} // end of rs485_stat_print()

/*
 **********************************************************************
 * @brief rs485_send_raw() accepts a string of bytes to send as an
 * unformatted message on the RS-485 interface
 *
 * @param p - ptr first ASCII character of byte string
 *
 * @note -- Expected format is 8-bit values seperated by a space such
 * as 00 01 0f 20 or 0x00 0x01 0x0f 0x20
 *
 * @return none
 **********************************************************************
 */
void
   rs485_send_raw(
      char	*bytes )
{

   uint8_t	buildAbyte;
   char		*c;
   char		*endPtr;
   uint8_t	interpretedBytes[ RS485_MAX_MSG_LEN ];
   uint16_t	i;
   uint16_t	numBytes;


   c = bytes;
   i = 0;
   while( *c != '\0' )
   {
      endPtr = NULL;
      buildAbyte = strtol( c, &endPtr, 0 );
      if( c != endPtr )
      {
	 // There were valid digits, limit the value to 8-bits
	 interpretedBytes[i++] = buildAbyte & 0xFF;
      }
      c = endPtr;
   }

   numBytes = i;
   if( numBytes > 1 )
   {
      // Actually found some bytes to send
      int8_t		ret;

      ret = rs485_prepare_tx_buf(
	       interpretedBytes[0],	// address byte
	       interpretedBytes[1] );	// function code
      i = 2;;
      if( ret != RS485_SUCCESS )
      {
	 printf( "rs485_prepare_tx_buf() failed in %s at line %d\r\n", __FILE__, __LINE__ );
      }
      else
      {
	 // Keep adding bytes
	 while( i < numBytes )
	 {
	    ret = rs485_add_tx_word( interpretedBytes[i++] );
	    if( ret != RS485_SUCCESS )
	    {
	       printf( "rs485_add_tx_word() failed in %s at line %d\r\n", __FILE__, __LINE__ );
	       printf( "Aborting message\r\n" );
	       return;
	    }
	 }
      }

      printf("About to send message (len = 0x%2.2x) as follows:\r\n", numBytes );
      for( i = 0; i < numBytes; i++ )
      {
	 if( !(i % 8) || (i == 0) )
	 {
	    printf( "\r\n" );
	    printf( "0x%2.2x:\t 0x%2.2x ", i, interpretedBytes[i] );
	 }
	 else
	 {
	    printf( "0x%2.2x ", interpretedBytes[i] );
	 }
      }
      printf( "CRC not shown but should be 0x%4.4x\r\n", transmit_msg.crc );

      // Add CRC and send it -- fire and forget style
      rs485_transmit_now();

   }

   return;
} // end of rs485_send_raw()

/*
 **********************************************************************
 * @brief rs485_enable_rx() enables the RS-485 transceiver receive
 * function
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void
   rs485_enable_rx(
      void )
{
   HAL_GPIO_WritePin( RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_RESET);

   return;
} // end of rs485_enable_rx()

/*
 **********************************************************************
 * @brief rs485_enable_tx() enables the RS-485 transceiver transmit
 * function
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void
   rs485_enable_tx(
      void )
{
   // UART4 hw should be controlling this pin
   //HAL_GPIO_WritePin( RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET);

   return;
} // end of rs485_enable_tx()

/*
 **********************************************************************
 * @brief rs485_disable_rx() disables the RS-485 transceiver receive
 * function
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void
   rs485_disable_rx(
      void )
{
   HAL_GPIO_WritePin( RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_SET);

   return;
} // end of rs485_disable_rx()

/*
 **********************************************************************
 * @brief rs485_disable_tx() disables the RS-485 transceiver transmit
 * function
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void
   rs485_disable_tx(
      void )
{
   // UART4 hw should be controlling this pin
   //HAL_GPIO_WritePin( RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET);

   return;
} // end of rs485_disable_tx()

/*
 **********************************************************************
 * @brief updateLevelLoop() sends 4-20mA command to system controller
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void updateLevelLoop( void )
{

   return;
} // end of updateLevelLoop()

/*
 **********************************************************************
 * @brief updateLevelLoop() sends 4-20mA command to system controller
 *
 * @param none
 *
 * @return none
 **********************************************************************
 */
void updateTurbLoop( void )
{

   return;
} // end of updateTurbLoop()


// end of file system_io.c
