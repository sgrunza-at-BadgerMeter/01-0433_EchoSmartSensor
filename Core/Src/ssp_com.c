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
      MODBUS_ADU_T 	*msg )
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

//*********************************************************************
// SSP Extended ID Request / Response (0x42)
//
// This command/response is used to read extended ID information from
// the Smart Sensor:
//
// 8 bits: Destination SSP Unit Address
// 8 bits: Control Field - (0x42)
// 1 byte: Status / Access Mode (2 bits)
// 	bit:   76543210
//	value: x000000x
//
// 		bit0: Read / Write Control. 1 = Write, 0 = Read
//		bit 7: Response. 1 = Response from SSP, 0 = Command from host
//		Others: Reserved – Always 0
// 24 bytes: Probe Name - ASCII string for name of probe. (Note may
//	     not be null terminated)
// 8 bytes: Compile Date of firmware in String format of MMDDYYYY
// 16 bits: CRC-16
//
//*********************************************************************
void
   ExtIdRequestCmd(
      MODBUS_ADU_T 	*msg )
{

   bool			isWrite;
   uint8_t		i;
   int8_t		ret;
   char			buildDate[ BD_LEN ];
   char			preprocessorDate[] = __DATE__;

   MODBUS_EXTID_REQ_T	*extIDcmd;


   if( msg != NULL )
   {
      extIDcmd = (MODBUS_EXTID_REQ_T *) msg;

      if( (extIDcmd->sam & SAM_WRITE) == SAM_WRITE )
      {
	 isWrite = true;
      }
      else
      {
	 isWrite = false;
      }

      if( isWrite )
      {
	 // Update the configuration
	 memset( SSP_configuration.name, 0, MAX_NAME_LEN );

	 // Now that the name is cleared, put in the new data
	 for( i = 0; i < MAX_NAME_LEN; i++ )
	 {
	    SSP_configuration.name[ i ] = extIDcmd->name[ i ];
	 }
      }

      // Now return the current name
      ret = rs485_prepare_tx_buf(
   	    SSP_configuration.address,
	    CMD_EXT_ID_REQ );
      if( ret != RS485_SUCCESS )
      {
         printf( "rs485_prepare_tx_buf() failed in %s at line %d\r\n", __FILE__, __LINE__ );
      }
      else
      {
         // Add full name
	 for( i = 0; i < MAX_NAME_LEN; i++ )
	 {
	    ret = rs485_add_tx_word( SSP_configuration.name[i] );
	    if( ret != RS485_SUCCESS )
	    {
	       printf( "rs485_prepare_tx_buf() failed in %s at line %d\r\n", __FILE__, __LINE__ );
	    }
	 }
      }

      // Convert the MMM_DD_YYYY string to MMDDYYYY
      //
      // The hard part is converting Jan, Feb, Mar, Apr, May, Jun,
      // Jul, Aug, Sep, Oct, Nov, Dec into 1 - 12
      //

      buildDate[0] = '0';

      switch( preprocessorDate[0] )
      {
	 case 'J':	// Jan or Jun or Jul
	    if( preprocessorDate[1] == 'a' )
	    {
	       // January
	       buildDate[1] = '1';
	    }
	    else
	    {
	       if( preprocessorDate[2] == 'n' )
	       {
		  // June
		  buildDate[1] = '6';
	       }
	       else
	       {
		  // July
		  buildDate[1] = '7';
	       }
	    }
	    break;

	 case 'F':
	    // Feb
	    buildDate[1] = '2';
	    break;

	 case 'M':	// Mar or May
	    if( preprocessorDate[2] == 'r' )
	    {
	       // March
	       buildDate[1] = '3';
	    }
	    else
	    {
	       // May
	       buildDate[1] = '5';
	    }
	    break;

	 case 'A':	// Apr or Aug
	    if( preprocessorDate[1] == 'p' )
	    {
	       // April
	       buildDate[1] = '4';
	    }
	    else
	    {
	       // August
	       buildDate[1] = '8';
	    }
	    break;

	 case 'S':	// Sep
	    buildDate[1] = '9';
	    break;

	 case 'O':	// Oct
	    buildDate[0] = '1';
	    buildDate[1] = '0';
	    break;

	 case 'N':	// Nov
	    buildDate[0] = '1';
	    buildDate[1] = '1';
	    break;

	 default:
	    // has to be Dec
	    buildDate[0] = '1';
	    buildDate[1] = '2';
	    break;
      } // end of switch( month )

      // First digit of month
      buildDate[ 2 ] = preprocessorDate[ 4 ];
      if( buildDate[2] == ' ' )
      {
	 buildDate[2] = '0';
      }

      buildDate[ 3 ] = preprocessorDate[ 5 ];

      // Year
      buildDate[ 4 ] = preprocessorDate[ 7 ];
      buildDate[ 5 ] = preprocessorDate[ 8 ];
      buildDate[ 6 ] = preprocessorDate[ 9 ];
      buildDate[ 7 ] = preprocessorDate[ 10 ];

      // Add Build Date
      for( i = 0; i < BD_LEN; i++ )
      {
	 ret = rs485_add_tx_byte( buildDate[i] );
	 if( ret != RS485_SUCCESS )
	 {
	    printf( "rs485_prepare_tx_buf() failed in %s at line %d\r\n", __FILE__, __LINE__ );
	 }
      }

      // Add CRC and send it -- fire and forget style
      rs485_transmit_now();
   }
   else
   {
      printf( "ExtIdRequestCmd() passed null ptr in %s at line %d\r\n",
	       __FILE__, __LINE__ );
   }

   return;
} // end of ExtIdRequestCmd()

//*********************************************************************
// Echo Control Command (0x68)
//
// This command is used manually control how echo data is processed by
// the probe. The command has four separate parts: Echo Control, Ping
// Rate, Update Rate, & Smoothing Rate. When the command is sent to
// the SSP some, none or all of the parts may be updated with a
// single command. The content of the SAM byte determines which parts
// (i.e. bytes) contain new information to be saved and which parts
// are only being read. The command to the SSP must always include
// all bytes in the message even if those bytes are only being read.
//
// If bytes are being read by the command then the contents of the
// byte in the message to the SSP is a don’t care.
//
// 8 bits: Destination SSP Unit Address
// 8 bits: Control Field – (0x68)
// 1 byte: Status / Access Mode (6 bits)
//	bit: 76543210
//	value: x00xxxxx
//	bit0: Read / Write Control for Echo Control Byte. 1 = Write, 0 = Read
//	bit1: Read / Write Control for Ping Rate Byte. 1 = Write, 0 = Read
//	bit2: Read / Write Control for Update Rate Byte. 1 = Write, 0 = Read
//	bit3: Read / Write Control for Smoothing Rate Byte. 1 = Write, 0 = Read
//	bit4: Read / Write Control for Delta Smoothing Rate Byte. 1 = Write, 0 = Read
//	bit7: Response. 1 = Response from SSP, 0 = Command from host
//	Others: Reserved – Always 0
// 1 byte: Echo Control
//	bit: 76543210
//	value: 0000xxxx
//	bit0: Enable next ping and capture cycle. 1 = Enable, 0 = Disable
//	bit1: Enable TX pulse blanking period. 1 = Enable, 0 - Disable
//	bit2: Enable Composite Waveform. 1 = Composite, 0 = Use average Waveform.
//	Bit3: Enable Single Step Mode, 1 = Enable, 0 = Disable
//	Others: Reserved – Always 0
// 1 byte: Ping Rate valid range 0 – 255.
// 1 byte: Update Rate valid range 0 – 20.
// 1 byte: Smoothing Rate valid range 0 – 25.
// 1 byte: Delta Smoothing Rate valid range 0 – 25.
// 1 byte: Save Sensor Waveform
// 16 bits: CRC-16
//
// If the SSP receives the Echo Control Command with bit 0 of the SAM
// (status/access mode) byte set then the value in the echo control
// byte will be saved by the SSP.
//
//*********************************************************************
void
   EchoControlCmd(
      MODBUS_ADU_T 	*msg )
{
   MODBUS_ECHO_CTRL_CMD_T	*m;
   int8_t			ret;

   if( msg != NULL )
   {
      m = (MODBUS_ECHO_CTRL_CMD_T *) msg;

      if( (m->sam & EC_SAM_ECB) == EC_SAM_ECB )
      {
	 // save the Echo Control byte

	 // Check if the EC_COMPOSITE_WF bit has changed
	 if( (SSP_status.bEchoControl & EC_COMPOSITE_WF) != (m->echoCtrl & EC_COMPOSITE_WF) )
	 {
	    // Current bit is different than the message's bit
	    if( m->echoCtrl & EC_COMPOSITE_WF )
	    {
	       // is set in message
	       SSP_status.bitFlags |= COMPOSITE_WF;
	    }
	    else
	    {
	       // is clear in message
	       SSP_status.bitFlags &= ~(COMPOSITE_WF);

	       // TODO: check if this is something that should be in SSP_configuration
	    }
	 }

	 // Check the Ping bit
	 if( (SSP_status.enable_ping == 0) && (m->echoCtrl & EC_ENABLE_PING) )
	 {
	    // Ech control was off but this message is turning it on
	    SSP_status.bProbeStatus &= ~(WAVEFORM_VALID);	// clear flag to force a new capture
	    SSP_status.nextWF = 1;	// Set flag to capture next wf if in single step mode
	 }
	 SSP_status.bEchoControl = m->echoCtrl;
      }

      if( m->echoCtrl & EC_SAM_PRB )
      {
	 SSP_configuration.pingDelay = m->pingRate;
      }

      if( m->echoCtrl & EC_SAM_URB )
      {
	 SSP_configuration.updateRate = m->updateRate;
      }

      if( m->echoCtrl & EC_SAM_SRB )
      {
	 SSP_configuration.smoothing = m->smoothingRate;
      }

      if( m->echoCtrl & EC_SAM_DSR )
      {
	 SSP_configuration.deltaSmoothing = m->deltaSmoothingRate;
      }

      // Build a response packet
      ret = rs485_prepare_tx_buf(
   	    SSP_configuration.address,
	    CMD_ECHO_CONTROL );
      if( ret != RS485_SUCCESS )
      {
         printf( "rs485_prepare_tx_buf() failed in %s at line %d\r\n", __FILE__, __LINE__ );
      }
      else
      {
	 rs485_add_tx_byte( SSP_status.bEchoControl );
	 rs485_add_tx_byte( SSP_configuration.pingDelay );
	 rs485_add_tx_byte( SSP_configuration.updateRate );
	 rs485_add_tx_byte( SSP_configuration.smoothing );
	 rs485_add_tx_byte( SSP_configuration.deltaSmoothing );

	 // Add CRC and send it -- fire and forget style
	 rs485_transmit_now();
      }
   }

   // TODO:  Make sure the system is running with the new bit settings


//   if(bitEnablePing)			/* If Ping is enabled */
//      EnablePinTimeOutSec = 0;	/* Disable Timer */
//   if(bitSingleStepMode)			/* If single step mode */
//      SingleStepTimeOutSec = SINGLE_STEP_TIMEOUT;	/* Reset Timer */
//   else
//      SingleStepTimeOutSec = 0;	/* Stop Timer */

   return;
} // end of EchoControlCmd()



//*********************************************************************
// Tank Configuration Command (0x43)
This command is used to set and/or read the current configuration. Tank configuration includes: Tank Depth, Zero Adjust, Dwell Time, Min Range, Max Range & Measure. The Tank Depth, Min Range and Max Range are indicated in inch or cm increments depending on the Units setting in the Acoustics Configuration. Therefore, a Tank Depth of 12 feet would be indicated by a value of 144 (0x90) in the Tank Depth byte if the Units was set to feet or inches. (Max Tank Depth 32 feet)
To read the current tank configuration, the host sends the tank configuration command with bit 0 of the status/access mode byte set to 0. (Note: The bytes for Tank Depth, Zero Adjust, Dwell Time, Min Range, Max Range and Measure do not need to be included in the read tank configuration command since they are ignored by the probe on receipt of the read command.)
8 bits: Destination SSP Unit Address
8 bits: Control Field - (0x43)
1 byte: Status / Access Mode (2 bits)
bit: 76543210
value: x000000x
bit0: Read / Write Control. 1 = Write, 0 = Read
bit7: Response. 1 = Response from SSP, 0 = Command from host
Others: Reserved – Always 0
2 bytes: Tank Depth in inches or cm. Maximum depth is 32 feet
2 bytes: Zero Adjust in inches or cm. Range is -16 to +16 feet. If MSB is 0 then value is positive.
1 byte: Dwell Time – TBD
2 bytes: Min Range in inches or cm
2 bytes: Max Range in inches or cm
1 byte: Measure – 0 for Level, 1 for Range.
16 bits: CRC-16
//*********************************************************************
void TankConfigCmd(void)
{
if(InMsgBuf[2] & SAM_WRITE)
	{
	param.TankDepth = (unsigned char)InMsgBuf[3];	// Get MSB
	param.TankDepth = param.TankDepth << 8;	// Move to MSB
	param.TankDepth |= (unsigned char)InMsgBuf[4];	// Add LSB
	param.ZeroAdjust = (unsigned char)InMsgBuf[5];	// Get MSB
	param.ZeroAdjust = param.ZeroAdjust << 8;	// Move to MSB
	param.ZeroAdjust |= (unsigned char)InMsgBuf[6];	// Add LSB
	param.DwellTime = InMsgBuf[7];
	param.MinLevel = (unsigned char)InMsgBuf[8];	// Get MSB
	param.MinLevel = param.MinLevel << 8;	// Move to MSB
	param.MinLevel |= (unsigned char)InMsgBuf[9];	// Add LSB
	param.MaxLevel = (unsigned char)InMsgBuf[10];	// Get MSB
	param.MaxLevel = param.MaxLevel << 8;	// Move to MSB
	param.MaxLevel |= (unsigned char)InMsgBuf[11];	// Add LSB
	if(InMsgBuf[12])	/* If Range selected */
		param.BitFlags |= MEASURE_RANGE;	/* Set Range */
	else
		param.BitFlags &= ~(MEASURE_RANGE);	/* Set Level */
	param.BitFlags &= ~(NEED_SETUP);	/* Clear Flag since tank depth set */
	}
GetTxBuf(CMD_TANK_CFG);		// Return response
SendWord(param.TankDepth); /* Add LSB */
SendWord(param.ZeroAdjust); /* Add LSB */
SendByte(param.DwellTime);
SendWord(param.MinLevel);
SendWord(param.MaxLevel);
if(param.BitFlags & MEASURE_RANGE)	// If in Range Mode
	SendByte(0x01);	// Show range
else
	SendByte(0x00);	// Show Level
SendCS();             /* Compute CS and add to buffer */
SendDataNow();        /* Send it */
}










// end of file ssp_com.c
