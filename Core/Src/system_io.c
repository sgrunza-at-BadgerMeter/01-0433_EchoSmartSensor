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
#include "crc.h"
#include "ssp_com.h"
#include "config.h"


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

// Defined in crc.c
extern CRC_HandleTypeDef 	hcrc;
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
	 // Check on the UART status
	 if( !transmit_msg.tx_active )
	 {
	    // Not currently transmitting
	    if (huart4.RxState != HAL_UART_STATE_BUSY_RX )
	    {
	       // Not busy with receiving so start a receive
	       rs485_receive_msg();
	    }
	 }

	 pADU = (MODBUS_ADU_T *)&buffer;

	 ret = fifo_pop( &rs485_buffer,			// ptr to queue
			 (char *)&(pADU->address), 	// ptr into which to put extracted command
			 (uint16_t *) &(pADU->length) );// ptr into which to put command length
	 if( ret == FIFO_SUCCESS )
	 {
	    process_rs485_msg( pADU );
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

   }

   return;
} // end of rs485_rx() thread

/*
 **********************************************************************
 * @brief process_rs485_msg() handles a single MODBUS / SSP message
 *
 * No data checks have been done at this point.  The CRC should be
 * verified
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

   uint16_t	crc_received;
   uint32_t	crc_calculated;

   uint16_t	length;
   union
   {
      uint8_t	*as8;
      uint32_t	*as32;
   } 	data;

   uint8_t	*crcPtr;

   if( msg->length > 3 )
   {
      // Messages smaller than 4 bytes are not valid

      data.as8 = &(msg->address);
      length = msg->length;

      length -= 2;	// remove the 2 bytes of CRC from the length

      crcPtr = data.as8 + length; // point to where the CRC should be
      crc_received = *crcPtr + (*(crcPtr+1)<<8);

      crc_calculated = HAL_CRC_Calculate(
	       &hcrc,
	       data.as32,		// ptr to data
	       length );		// number of data bytes

#if 1
      printf( "RS485 msg received with address 0x%2.2x, fc = 0x%2.2x, payload[0] = 0x%2.2x, length = 0x%4.4x, CRC = 0x%4.4x\r\n",
	      msg->address,
	      msg->fc,
	      msg->payload[0],
	      msg->length,
	      crc_received );

      display_message( &(msg->address), msg->length );
#endif

      if( crc_received != (uint16_t)crc_calculated )
      {
	 printf( "\r\nCRC error:  calculated CRC is 0x%4.4x\r\n", (uint16_t)crc_calculated) ;
      }
      else
      {
	 rs485_decode_command( msg );
      }
   }

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
} // end of rs485_prepare_tx_buf()

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

   display_message( transmit_msg.msg, transmit_msg.index );


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
 * checksum using STM32H523 Hardware CRC unit
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

   uint32_t			crc_result;

   crc_result = HAL_CRC_Calculate(
	    &hcrc,
	    (uint32_t *)p->msg,	// ptr to data
	    p->index );		// number of data bytes

   p->crc = (uint16_t) crc_result;

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
      fflush( stdout );
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
void updateLevelLoop( uint16_t counts )
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
void updateTurbLoop( uint16_t counts )
{

   return;
} // end of updateTurbLoop()

/*
 **********************************************************************
 * @brief rs485_decode_command() is passed a CRC valid message to process
 *
 * @param msg - pointer to a msg with a valid CRC
 *
 * @return none
 **********************************************************************
 */
void rs485_decode_command( MODBUS_ADU_T *msg )
{
   SSP_status.bus_msg_count++;	// If the CRC fits, you must admit you received it
   if( (msg->address == SSP_configuration.address) ||	// msg for this unit
       (msg->address == 0) )				// broadcast msg
   {
      SSP_status.slave_msg_count++;

      if( msg->address == 0 )
      {
	 // For broadcast messages only do a limited sub-set of commands
	 switch( msg->fc )
	 {
	    case CMD_NETWORK_POLL:
	       rs485_network_poll_cmd( msg );
	       break;

	    default:
	       asm("nop");	// Do nothing
	       break;
	 } // end of switch(fc) for broadcast messages
      }
      else
      {
	 // Non-broadcast message
	 switch( msg->fc )
	 {
	    case MODBUS_READ_COIL:	// function code 1
	       rs485_readCoil( msg );
	       break;

	    case MODBUS_READ_HOLDING_REG:
	       rs485_readHoldingReg( msg );
	       break;
	 }
      }

   }

   return;
} // end of rs485_decode_command()

/*
 **********************************************************************
 * @brief rs485_network_poll_cmd()
 *
 * @param msg - pointer to a msg with a valid CRC
 *
 * @return none
 **********************************************************************
 */
void rs485_network_poll_cmd( MODBUS_ADU_T *msg )
{
   return;
} // end of rs485_network_poll_cmd()

/*
 **********************************************************************
 * @brief rs485_readHoldingReg()
 *
 * @param msg - pointer to a msg with a valid CRC
 *
 * @return none
 **********************************************************************
 */void rs485_readHoldingReg( MODBUS_ADU_T *msg )
{
   return;
} // end of rs485_readHoldingReg()

 /*
  **********************************************************************
  * @brief rs485_readCoil() returns one or more single bit coil values
  * in a packed together byte format.  A single coil is returned as a
  * left justified bit with seven trailing zeros.
  *
  * @param msg - pointer to a msg with a valid CRC
  *
  * @return none
  **********************************************************************
  */
 void
   rs485_readCoil(
      MODBUS_ADU_T 	*msg )
 {

     uint16_t		firstReg;
     uint16_t		numRegs;
     uint8_t		numBytes;

     uint8_t		coilNumber;
     uint8_t		numCoils;

     bool		singleCoil;
     uint8_t		packedCoil;

     COIL_CMD_T 	coilFunction;

     firstReg = modbus_reg_first_reg( msg );
     numRegs = modbus_number_of_regs( msg );

     // Do some error checks
     if( ( (numRegs + firstReg) > NBR_MB_COILS ) ||
	   ( numRegs > MAX_MB_COILS_COMM) )
     {
	// coils requested beyond range
	// build an error response
	rs485_prepare_tx_buf( msg->address, msg->fc | 0x80 );
	rs485_add_tx_byte( MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS ); // code 2
     }
     else
     {
	rs485_prepare_tx_buf( msg->address, msg->fc );
	numBytes = numRegs / 8;	// number of bytes returned
	if( numRegs % 8 )
	{
	   // there is a remainder so add a byte
	   numBytes++;
	}
	rs485_add_tx_byte( numBytes );

	numCoils = 0;
	coilNumber = firstReg;
	packedCoil = 0;

	while( numRegs-- > 0 )
	{
	   coilFunction = rs485_find_coil_function( coilNumber++ );
	   if( coilFunction != NULL )
	   {
	      // There is a function for this coil
	      singleCoil = coil_commands[ coilNumber++ ].function( false, false );	// read command
	   }
	   else
	   {
	      // No function associated with this coil, return 0
	      singleCoil = 0;
	   }

	   packedCoil = (packedCoil >> 1);
	   if( singleCoil )
	   {
	      packedCoil |= 0x80;
	   }

	   if( ++numCoils >= 8 )
	   {
	      rs485_add_tx_byte( packedCoil );
	      packedCoil = 0;
	      numCoils = 0;
	   }
	} // end of while()
     }

     // Actually send response
     rs485_transmit_now();	// send the message

     return;
 } // end of rs485_readCoil()

 /*
   **********************************************************************
   * @brief rs485_writeCoil() Write to a single coil
   *
   * @param msg - pointer to a msg with a valid CRC
   *
   * @return none
   **********************************************************************
   */
 void
    rs485_writeCoil(
       MODBUS_ADU_T 	*msg )
  {
    uint16_t		coilNumber;
    bool		result;
    bool		data;

    COIL_CMD_T		coilFunction;


    coilNumber = modbus_reg_first_reg( msg );

    // Do some error checks
    if( coilNumber > NBR_MB_COILS )
    {
	// coils requested beyond range
	// build an error response
	rs485_prepare_tx_buf( msg->address, msg->fc | 0x80 );
	rs485_add_tx_byte( MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS ); // code 2
    }
    else
    {
       // perform the write
       if( msg->payload[2] )	// fifth byte of message
       {
	  data = true;
       }
       else
       {
	  data = false;
       }

       coilFunction = rs485_find_coil_function( coilNumber );
       if( coilFunction != NULL )
       {
	  // There is a function for this coil
	  result = coil_commands[ coilNumber ].function( true, data );	// write command
       }
       else
       {
	  // No function associated with this coil, return something to create an error msg
	  result = !data;
       }

       if( result == data )
       {
	  // write was ok
	  rs485_prepare_tx_buf( msg->address, msg->fc );
	  rs485_add_tx_byte( msg->payload[0] );	// coil MSB
	  rs485_add_tx_byte( msg->payload[1] );	// coil LSB
	  rs485_add_tx_byte( msg->payload[2] );	// 0xFF or 0x00 from msg
	  rs485_add_tx_byte( 0x00 );
       }
       else
       {
	  // build an error response
	  rs485_prepare_tx_buf( msg->address, msg->fc | 0x80 );
	  rs485_add_tx_byte( MBUS_RESPONSE_SERVICE_DEVICE_FAILURE ); // code 4
       }
    }

    // Actually send response
    rs485_transmit_now();	// send the message

    return;
  } // end of rs485_writeCoil()

//*********************************************************************
COIL_CMD_T
   rs485_find_coil_function(
      uint16_t		coilNum )
{
   int16_t			index;
   COIL_CMD_T			func;
   static uint16_t		lastIndex = 0;

   if( lastIndex == 0 )
   {
      // Need to find the last index
      while( coil_commands[lastIndex].coilNumber != MB_INVALID_COIL )
      {
	 lastIndex++;
      }
   }

   if( coilNum > lastIndex )
   {
      // coilNum is large so probably near the end of list
      index = lastIndex;
   }
   else
   {
      // start at the given coilNum as an index and look backwards
      index = coilNum;
   }

   while( coil_commands[index].coilNumber != coilNum )
   {
      index--;
      if( index < 0 )
      {
	 // Not found and accessing array[-1] is bad
	 break;
      }
   }

   if( index < 0 )
   {
      func = NULL;
   }
   else
   {
      func = coil_commands[index].function;
   }

   return( func );
} // end of rs485_find_coil_function()

//*********************************************************************
REG_CMD_T
   rs485_find_reg_function(
      uint16_t		regNum )
{
   int16_t			index;
   REG_CMD_T			func;
   static uint16_t		lastIndex = 0;

   if( lastIndex == 0 )
   {
      // Need to find the last index
      while( reg_commands[lastIndex].regNumber != MB_INVALID_REG )
      {
	 lastIndex++;
      }
   }

   if( regNum > lastIndex )
   {
      // coilNum is large so probably near the end of list
      index = lastIndex;
   }
   else
   {
      // start at the given coilNum as an index and look backwards
      index = regNum;
   }

   while( reg_commands[index].regNumber != regNum )
   {
      index--;
      if( index < 0 )
      {
	 // Not found and accessing array[-1] is bad
	 break;
      }
   }

   if( index < 0 )
   {
      func = NULL;
   }
   else
   {
      func = reg_commands[index].function;
   }

   return( func );
} // end of rs485_find_reg_function()

/*
 **********************************************************************
 * @brief rs485_readReg() puts one or more 16-bit register values into
 * the transmit buffer.
 *
 * Decodes FC command 0x03
 *
 * @param msg - pointer to a msg with a valid CRC
 *
 * @return none
 **********************************************************************
 */
void
  rs485_readReg(
     MODBUS_ADU_T 	*msg )
{

    uint16_t		firstReg;
    uint16_t		reg;
    uint16_t		numRegs;

    uint16_t		regResult;

    REG_CMD_T 		regFunction;

    firstReg = modbus_reg_first_reg( msg );
    numRegs = modbus_number_of_regs( msg );

    // Do some error checks
    if( ( firstReg <= WAVEFORM_MB_REG_STOP ) &&
	( numRegs <= MAX_MB_COILS_COMM) )
    {
       // Simple error checks are look ok
       rs485_prepare_tx_buf( msg->address, msg->fc );
       rs485_add_tx_byte( numRegs * 2 );	// 2 bytes per register

       reg = firstReg;

       while( numRegs-- > 0 )
       {
	  if( firstReg == SENSOR_NAME_MB_REG_START )
	  {
	     regResult = MB_Reg40100( reg++, false, false );
	  }
	  else if( firstReg == SENSOR_SN_MB_REG_START )
	  {
	     regResult = MB_Reg40112( reg++, false, false );
	  }
	  else
	  {
	     regFunction = rs485_find_reg_function( reg++ );
	     if( regFunction != NULL )
	     {
		regResult = regFunction( reg++, false, false );
	     }
	     else
	     {
		regResult = 0;	// need to return something
	     }
	  }

	  rs485_add_tx_byte( regResult >> 8 );
	  rs485_add_tx_byte( regResult & 0x00FF );

       } // end of while()
    }
    else
    {
       // coils requested beyond range
       // build an error response
       rs485_prepare_tx_buf( msg->address, msg->fc | 0x80 );
       rs485_add_tx_byte( MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS ); // code 2
    }

    // Actually send response
    rs485_transmit_now();	// send the message

    return;
} // end of rs485_readReg()

/*
 **********************************************************************
 * @brief rs485_writeReg() handles a single 16-bit register write
 *
 * Decodes FC command 0x06
 *
 * @param msg - pointer to a msg with a valid CRC
 *
 * @return none
 **********************************************************************
 */
void
  rs485_writeReg(
     MODBUS_ADU_T 	*msg )
{

    uint16_t			wrResult;
    REG_CMD_T 			regFunction;
    MODBUS_ADU_REG_WRITE_T	*m;

    if( msg != NULL )
    {
       m = (MODBUS_ADU_REG_WRITE_T *)msg;

       // Do some error checks
       if( m->regAddr <= WAVEFORM_MB_REG_STOP )
       {
	  // Simple error checks are look ok
	  rs485_prepare_tx_buf( msg->address, msg->fc );
	  rs485_add_tx_byte( m->regAddr >> 8 );
	  rs485_add_tx_byte( m->regAddr & 0x00FF );

	  if( (m->regAddr >= SENSOR_NAME_MB_REG_START) &&
              (m->regAddr <= SENSOR_NAME_MB_REG_STOP) )
	  {
	     // use the sensor name function
	     wrResult = MB_Reg40100( m->regAddr, true, m->regData );
	  }
	  else if( (m->regAddr >= SENSOR_SN_MB_REG_START) &&
		   (m->regAddr <= SENSOR_SN_MB_REG_STOP) )
	  {
	     // use the sensor serial number function
	     wrResult = MB_Reg40112( m->regAddr, true, m->regData );
	  }
	  else
	  {
	     // Need to find the correct function
	     regFunction = rs485_find_reg_function( m->regAddr );
	     if( regFunction != NULL )
	     {
		wrResult = regFunction( m->regAddr, true, m->regData );
	     }
	     else
	     {
		wrResult = 0;	// need to return something
	     }
	  }

	  rs485_add_tx_byte( wrResult >> 8 );
	  rs485_add_tx_byte( wrResult & 0x00FF );
       }
       else
       {
	  // coils requested beyond range
	  // build an error response
	  rs485_prepare_tx_buf( msg->address, msg->fc | 0x80 );
	  rs485_add_tx_byte( MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS ); // code 2
       }

       // Actually send response
       rs485_transmit_now();	// send the message
    }

    return;
} // end of rs485_writeReg()

/*
 **********************************************************************
 * @brief rs485_writeMultipleRegs() handles multiple 16-bit register
 * writes
 *
 * Decodes FC command 0x10
 *
 * @param msg - pointer to a msg with a valid CRC
 *
 * @return none
 **********************************************************************
 */
void
   rs485_writeMultipleRegs(
     MODBUS_ADU_T 	*msg )
{

    uint16_t				wrResult;
    REG_CMD_T 				regFunction;
    MODBUS_ADU_MULTIREG_WRITE_T		*m;
    uint8_t				bytesWritten;

    uint16_t				regToWrite;
    uint16_t				dataToWrite;

    uint8_t				i;

    if( msg != NULL )
    {
       m = (MODBUS_ADU_MULTIREG_WRITE_T *)msg;

       // Do some error checks
       if( (m->regAddr <= WAVEFORM_MB_REG_STOP) &&
           (m->byteCount == m->regNum * 2) )
       {
	  // Simple error checks are look ok
	  rs485_prepare_tx_buf( msg->address, msg->fc );
	  rs485_add_tx_byte( m->regAddr >> 8 );
	  rs485_add_tx_byte( m->regAddr & 0x00FF );
	  // need to put number of bytes written next

	  bytesWritten = 0;
	  for( i = 0; i < m->regNum; i++ )
	  {
	     dataToWrite = m->regData[i];
	     regToWrite = m->regAddr + i;

	     if( (regToWrite >= SENSOR_NAME_MB_REG_START) &&
		 (regToWrite <= SENSOR_NAME_MB_REG_STOP) )
	     {
		// use the sensor name function
		wrResult = MB_Reg40100( regToWrite, true, dataToWrite );
	     }
	     else if( (regToWrite >= SENSOR_SN_MB_REG_START) &&
		      (regToWrite <= SENSOR_SN_MB_REG_STOP) )
	     {
		// use the sensor serial number function
		wrResult = MB_Reg40112( regToWrite, true, dataToWrite );
	     }
	     else
	     {
		// Need to find the correct function
		regFunction = rs485_find_reg_function( regToWrite );
		if( regFunction != NULL )
		{
		   wrResult = regFunction( regToWrite, true, dataToWrite );
		   if( wrResult == dataToWrite )
		   {
		      bytesWritten += 2;
		   }
		}
		else
		{
		   wrResult = 0;	// need to return something
		}
	     }
	  }

	  if( bytesWritten == m->byteCount )
	  {
	     rs485_add_tx_byte( m->regNum >> 8 );
	     rs485_add_tx_byte( m->regNum & 0x00FF );
	  }
	  else
	  {
	     // Something went wrong
	     rs485_prepare_tx_buf( msg->address, msg->fc | 0x80 );
	     rs485_add_tx_byte( MBUS_RESPONSE_ILLEGAL_DATA_VALUE ); // code 3
	  }
       }
       else
       {
	  // coils requested beyond range
	  // build an error response
	  rs485_prepare_tx_buf( msg->address, msg->fc | 0x80 );
	  if( m->byteCount != m->regNum * 2 )
	  {
	     rs485_add_tx_byte( MBUS_RESPONSE_ILLEGAL_DATA_VALUE ); // code 3
	  }
	  else
	  {
	     rs485_add_tx_byte( MBUS_RESPONSE_ILLEGAL_DATA_ADDRESS ); // code 2
	  }
       }

       // Actually send response
       rs485_transmit_now();	// send the message
    }

    return;
} // end of rs485_writeMultipleRegs()


// end of file system_io.c
