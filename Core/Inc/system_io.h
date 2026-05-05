/**
 **********************************************************************
 * @file system_io.h
 *
 * @brief A brief description of this file's purpose
 *
 * @author grunzas
 *
 * @date Oct 9, 2025
 *
 **********************************************************************
 */
#ifndef INC_SYSTEM_IO_H_
#define INC_SYSTEM_IO_H_

#include "tx_api.h"
#include "modbus.h"
#include <stdbool.h>

/**
 * @brief This is the maximum number of bytes in an RS-485 MODBUS
 * or SSP message
 *
 * Need to have large messages for sending waveforms to controller
 *
 */
#define RS485_MAX_MSG_LEN	520

// Define result codes for the rs485_ functions
#define RS485_FAILURE		-1
#define RS485_SUCCESS		0

typedef struct RS485_TRANSMIT_MSG_T
{
   bool		tx_active;			/**< 1 = transmission currently active */
   uint16_t	index;				/**< number of bytes currently in msg */
   uint16_t	crc;				///< 16-bit CRC
   uint8_t	msg[ RS485_MAX_MSG_LEN ];	///< message storage location
} RS485_TRANSMIT_MSG_T;


/**
 * This structure holds statistical information about the RS-485
 * message traffic
 */
typedef struct RS485_COMM_STATISTICS_T
{
   uint32_t		rx_frame_errors;	///< Number of receive frame errors
   uint32_t		recvd_frames;		///< Number of frames received
   uint32_t		tx_frames;		///< Number of transmitted frames
} RS485_COMM_STATISTICS_T;


/**
 * @brief Definition of the number of ms between MODBUS / SSP messages
 *
 * To ensure frame integrity during the transmission, the time interval
 * between two frames must be at least the transmission time of 3.5
 * characters, and the time interval between two consecutive characters
 * must be no more than the transmission time of 1.5 characters. -- Wikipedia
 *
 * t3.5 = 3.5 * ( 10 bits ) / 9600 bps = 3.6458 ms
 *
 * t1.5 = 1.5 * ( 10 bits ) / 9600 bps = 1.5625 ms
 *
 * Each character should arrive within 1.5 to 3.6 ms so any character later
 * than 4 ms is part of a new message
 */
#define RS485_MESSAGE_GAP	4

// Define the receive thread function
void rs485_rx(ULONG thread_input);


void rs485_receive_char( void );

void
   process_rs485_msg(
      MODBUS_ADU_T	*msg );

void
   calculate_crc16_modbus(
      RS485_TRANSMIT_MSG_T	*p );

void
   rs485_transmit_now(
      void );

int8_t
   rs485_add_tx_word(
      uint16_t		w );

int8_t
   rs485_add_tx_byte(
      uint8_t		b );

int8_t
   rs485_prepare_tx_buf(
      char		addr,
      char		fc );

TX_THREAD *
   rs485_get_threadID(
      void );

void
   rs485_stat_clear(
      RS485_COMM_STATISTICS_T	*p );

void
   rs485_send_raw(
      char	*cmd );

void
   rs485_enable_rx(
      void );

void
   rs485_enable_tx(
      void );

void
   rs485_disable_rx(
      void );

void
   rs485_disable_tx(
      void );

void updateLevelLoop( uint16_t	dacCounts );
void updateTurbLoop( uint16_t	dacCounts );
void rs485_readCoil( MODBUS_ADU_T *msg );
void rs485_decode_command( MODBUS_ADU_T *msg );
void rs485_network_poll_cmd( MODBUS_ADU_T *msg );
void rs485_readRegs( MODBUS_ADU_T *msg );	// multiple registers
void rs485_readReg( MODBUS_ADU_T *msg );	// single register
void rs485_readCoil( MODBUS_ADU_T *msg );	// one or more coils

COIL_CMD_T
   rs485_find_coil_function(
      uint16_t		coilNum );

void
   rs485_writeMultipleRegs(
     MODBUS_ADU_T 	*msg );

void
  rs485_writeReg(
     MODBUS_ADU_T 	*msg );

REG_CMD_T
   rs485_find_reg_function(
      uint16_t		regNum );

void
   rs485_writeCoil(
      MODBUS_ADU_T 	*msg );

void
   rs485_diagnostics(
      MODBUS_ADU_T 		*msg );


#endif /* INC_SYSTEM_IO_H_ */
