/**
 **********************************************************************
 * @file ssp_com.c
 *
 * @brief This file contains code for sending SmartSensorProbe commands
 *
 * @author grunzas
 *
 * @date Oct 14, 2025
 *
 **********************************************************************
 */

#include <stdio.h>
#include "main.h"
#include "modbus.h"
#include "ssp_com.h"
#include "config.h"
#include "system_io.h"


/*
 **********************************************************************
 * @brief IdRequestCmd() sends ID information
 *
 * @param none
 *
 * @return none
 *
 * Background affect: Creates and sends a system message
 *
 **********************************************************************
 */

void
   IdRequestCmd(
      void )
{
   int8_t		ret;

   ret = rs485_prepare_tx_buf(
	    SSP_configuration.address,
	    CMD_ID_REQ );
   if( ret != RS485_SUCCESS )
   {
      printf( "rs485_prepare_tx_buf() failed in %s at line %d\r\n", __FILE__, __LINE__ );
   }
   else
   {
      // Add serial number
      ret = rs485_add_tx_word( SSP_configuration.sn) ;
   }

   if( ret != RS485_SUCCESS )
   {
      printf( "rs485_add_tx_word() failed in %s at line %d\r\n", __FILE__, __LINE__ );
   }
   else
   {
      // Add HW ID
      ret = rs485_add_tx_byte( SSP_configuration.hw_id );
   }

   if( ret != RS485_SUCCESS )
   {
      printf( "rs485_add_tx_byte() failed in %s at line %d\r\n", __FILE__, __LINE__ );
   }
   else
   {
      // Add firmware version
      ret = rs485_add_tx_byte( SSP_configuration.fw_ver );
   }

   if( ret != RS485_SUCCESS )
   {
      printf( "rs485_add_tx_byte() failed in %s at line %d\r\n", __FILE__, __LINE__ );
   }
   else
   {
      // Add equipment type
      ret = rs485_add_tx_byte( SSP_configuration.eq_type );
   }

   if( ret != RS485_SUCCESS )
   {
      printf( "rs485_add_tx_byte() failed in %s at line %d\r\n", __FILE__, __LINE__ );
   }
   else
   {
      // Add CRC and send it -- fire and forget style
      rs485_transmit_now();
   }

   return;
} // end of IdRequestCmd()


















// end of file ssp_com.c
