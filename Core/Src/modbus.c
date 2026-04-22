/**
 **********************************************************************
 * @file modbus.c
 *
 * @brief This file contains modbus related code for the Smart Sensor
 * and Filter Smart Probe
 *
 * @author grunzas
 *
 * @date Oct 7, 2025
 *
 **********************************************************************
 */

/* 9/11/07 Initial Release V0.01 */
/* 8/31/10 Corrected bug to write to multiple registers */

#define _MODBUS_C_

#include  <ctype.h>           	/* Standard C character functions */
#include  <stdio.h>           	/* Standard C IO function definitions */
#include  <stdlib.h>          	/* Standard Library */
#include  <string.h>          	/* Standard String Library */
#include  <math.h>		/* Math Routines */

#include "main.h"
#include "modbus.h"
#include "config.h"

//*********************************************************************
uint16_t modbus_reg_first_reg( MODBUS_ADU_T *msg )
{

   uint16_t	reg16;

   reg16 = msg->payload[0] << 8;
   reg16 |= msg->payload[1];

   return( reg16 );
} // end of modbus_reg_first_reg()

//*********************************************************************
uint16_t modbus_number_of_regs( MODBUS_ADU_T *msg )
{
   uint16_t	reg16;

   reg16 = msg->payload[2] << 8;
   reg16 |= msg->payload[3];

   return( reg16 );
} // end of modbus_number_of_regs()

//*********************************************************************
bool MB_Coil001( bool isWrite, bool val )
{
   bool		retVal;
   bool		tmp;


   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.auto_gain;
   }
   else
   {
      // This is a write command
      tmp = SSP_status.auto_gain;

      SSP_status.auto_gain = val;

      retVal = val;

      if( tmp != val )
      {
	 // TODO: Save configuration to flash
      }
      retVal = true;	// write worked
   }

   return( retVal );
} // end of MB_Coil001()

//*********************************************************************
bool MB_Coil002( bool isWrite, bool val )
{

   bool		retVal;
   bool		tmp;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.measure_range;
   }
   else
   {
      // This is a write command
      tmp = SSP_status.measure_range;

      SSP_status.measure_range = val;

      retVal = val;

      if( tmp != val )
      {
	 // TODO: Save configuration to flash
      }
      retVal = true;	// write worked
   }

   return( retVal );
} // end of MB_Coil002()

//*********************************************************************
bool MB_Coil003( bool isWrite, bool val )
{
   bool		retVal;
   bool		tmp;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.settling_zone;
   }
   else
   {
      // This is a write command
      tmp = SSP_status.settling_zone;

      SSP_status.settling_zone = val;

      retVal = val;

      if( tmp != val )
      {
	 // TODO: Save configuration to flash
      }
      retVal = true;	// write worked
   }

   return( retVal );
} // end of MB_Coil003

//*********************************************************************
bool MB_Coil004( bool isWrite, bool val )
{
   bool		retVal;
   bool		tmp;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.echo_loss;
   }
   else
   {
      // This is a write command
      tmp = SSP_status.echo_loss;

      SSP_status.echo_loss = val;

      retVal = val;

      if( tmp != val )
      {
	 // TODO: Save configuration to flash
      }
      retVal = true;	// write worked
   }

   return( retVal );
} // end of MB_Coil004

//*********************************************************************
bool MB_Coil005( bool isWrite, bool val )
{
   bool		retVal;
   bool		tmp;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.force_level_4ma;
   }
   else
   {
      // This is a write command
      tmp = SSP_status.force_level_4ma;

      SSP_status.force_level_4ma = val;

      retVal = val;

      if( tmp != val )
      {
	 // TODO: Save configuration to flash
      }
      retVal = true;	// write worked
   }

   return( retVal );
} // end of MB_Coil005

//*********************************************************************
bool MB_Coil006( bool isWrite, bool val )
{
   bool		retVal;
   bool		tmp;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.force_level_20ma;
   }
   else
   {
      // This is a write command
      tmp = SSP_status.force_level_20ma;

      SSP_status.force_level_20ma = val;

      retVal = val;

      if( tmp != val )
      {
	 // TODO: Save configuration to flash
      }
      retVal = true;	// write worked
   }

   return( retVal );
} // end of MB_Coil006

//*********************************************************************
bool MB_Coil007( bool isWrite, bool val )
{
   bool		retVal;
   bool		tmp;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.force_turb_4ma;
   }
   else
   {
      // This is a write command
      tmp = SSP_status.force_turb_4ma;

      SSP_status.force_turb_4ma = val;

      retVal = val;

      if( tmp != val )
      {
	 // TODO: Save configuration to flash
      }
      retVal = true;	// write worked
   }

   return( retVal );
} // end of MB_Coil007

//*********************************************************************
bool MB_Coil008( bool isWrite, bool val )
{
   bool		retVal;
   bool		tmp;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.force_turb_20ma;
   }
   else
   {
      // This is a write command
      tmp = SSP_status.force_turb_20ma;

      SSP_status.force_turb_20ma = val;

      retVal = val;

      if( tmp != val )
      {
	 // TODO: Save configuration to flash
      }
      retVal = true;	// write worked
   }

   return( retVal );
} // end of MB_Coil008

//*********************************************************************
bool MB_Coil009( bool isWrite, bool val )
{
   bool		retVal;
   bool		tmp;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.composite_wf;
   }
   else
   {
      // This is a write command
      tmp = SSP_status.composite_wf;

      SSP_status.composite_wf = val;

      retVal = val;

      if( tmp != val )
      {
	 // TODO: Save configuration to flash
      }
      retVal = true;	// write worked
   }

   return( retVal );
} // end of MB_Coil009

//*********************************************************************
bool MB_Coil010( bool isWrite, bool val )
{
   bool		retVal;
   bool		tmp;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.enable_tx_pulse;
   }
   else
   {
      // This is a write command
      tmp = SSP_status.enable_tx_pulse;

      SSP_status.enable_tx_pulse = val;

      retVal = val;

      if( tmp != val )
      {
	 // TODO: Save configuration to flash
      }
      retVal = true;	// write worked
   }

   return( retVal );
} // end of MB_Coil010

//*********************************************************************
bool MB_Coil011( bool isWrite, bool val )
{
   bool		retVal;
   bool		tmp;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.enable_ping;
   }
   else
   {
      // This is a write command
      tmp = SSP_status.enable_ping;

      SSP_status.enable_ping = val;

      retVal = val;

      if( tmp != val )
      {
	 SSP_status.waveform_valid = false;

	 // TODO: Save configuration to flash
      }
      retVal = true;	// write worked
   }

   // TODO: Figure out what the Ping Timer / Timeout logic did originally

   return( retVal );
} // end of MB_Coil011

//*********************************************************************
bool MB_Coil012( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = false;	// This is a LowPowerMode command -- this system does not support lowPower
   }
   else
   {
      // This is a write command
      retVal = false;	// write failed, this is read-only
   }

   return( retVal );
} // end of MB_Coil012

//*********************************************************************
bool MB_Coil017( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_configuration.hasTurbidity;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil017

//*********************************************************************
bool MB_Coil018( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_configuration.hasWiper;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil018

//*********************************************************************
bool MB_Coil019( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.wiperDriverError;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil019

//*********************************************************************
bool MB_Coil020( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.wiperConnectionError;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil020

//*********************************************************************
bool MB_Coil021( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = false;
      if( SSP_status.bTrackStatus & LOS_INDICATOR )
      {
	 retVal = true;
      }

      if( SSP_status.los_alarm )
      {
	 retVal = true;
      }

      if( (SSP_status.bTrackStatus & NO_REPORT_VALUE) &&
          (SSP_status.bInitStatus & INIT_DONE) )
      {
	 retVal = true;
      }
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil021

//*********************************************************************
bool MB_Coil022( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = (SSP_status.bInitStatus & INIT_STAB_GAIN) ? true: false;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil022

//*********************************************************************
bool MB_Coil023( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = (SSP_status.bInitStatus & INIT_STAB_TRACK) ? true : false;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil023

//*********************************************************************
bool MB_Coil024( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = (SSP_status.bInitStatus & INIT_STAB_GATES) ? true : false;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil024

//*********************************************************************
bool MB_Coil025( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = (SSP_status.bInitStatus & INIT_DONE) ? true : false;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil025

//*********************************************************************
bool MB_Coil026( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = (SSP_status.bInitStatus & INIT_NEED_SETUP) ? true : false;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil026

//*********************************************************************
bool MB_Coil027( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = (SSP_status.bLoopStatus & LEVEL_LOOP_ACTIVE) ? true : false;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil027

//*********************************************************************
bool MB_Coil028( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = (SSP_status.bLoopStatus & TURBIDITY_LOOP_ACTIVE) ? true : false;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil028

//*********************************************************************
bool MB_Coil029( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = (SSP_status.bLoopStatus & LEVEL_DRIVER_ERROR) ? true : false;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil029

//*********************************************************************
bool MB_Coil030( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = (SSP_status.bLoopStatus & TURBIDITY_DRIVER_ERROR) ? true : false;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil030

//*********************************************************************
bool MB_Coil031( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = (SSP_status.bLoopStatus & ECHO_DELAY_TIMER_STARTED) ? true : false;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil031

//*********************************************************************
bool MB_Coil032( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = SSP_status.waveform_valid;
   }
   else
   {
      // This is a write command
      retVal = false;	// read-only
   }

   return( retVal );
} // end of MB_Coil032

//*********************************************************************
bool MB_Coil033( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = false;
   }
   else
   {
      // This is a write command
      retVal = true;	// read-only
      // Load the configuration structure with default values
      cfg_load_default( &SSP_configuration, LOAD_USER_SETTINGS );
   }

   return( retVal );
} // end of MB_Coil033

//*********************************************************************
bool MB_Coil034( bool isWrite, bool val )
{
   bool		retVal;
   bool		configOK;

   if( !isWrite )
   {
      // This is a read command
      retVal = false;
   }
   else
   {
      // This is a write command
      retVal = true;

      // Load the configuration structure with Flash values

      // Load a configuration into the sensor
      configOK = cfg_verify_nvconfig( SSP_nvcfg_addr, sizeof(SSP_CONFIG_T) );
      if( configOK )
      {
         memcpy( &SSP_configuration, SSP_nvcfg_addr, sizeof( SSP_CONFIG_T) );
      }
      else
      {
         cfg_load_default( &SSP_configuration, LOAD_ALL_SETTINGS );
         printf( "Flash-based configuration is not valid, using defaults\r\n" );
      }

      printf( "Device address is 0x%2.2x\r\n", SSP_configuration.address );
   }

   return( retVal );
} // end of MB_Coil034

//*********************************************************************
bool MB_Coil035( bool isWrite, bool val )
{
   bool		retVal;
   bool		writeOk;

   if( !isWrite )
   {
      // This is a read command
      retVal = false;
   }
   else
   {
      // This is a write command
      retVal = true;

      // Save the configuration structure to Flash
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

      printf( "Device address is 0x%2.2x\r\n", SSP_configuration.address );
   }

   return( retVal );
} // end of MB_Coil035

//*********************************************************************
bool MB_Coil036( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = false;
   }
   else
   {
      // This is a write command
      retVal = true;
      NVIC_SystemReset();
   }

   return( retVal );
} // end of MB_Coil036

//*********************************************************************
bool MB_Coil037( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = (SSP_status.bProbeStatus & TURB_LED_ERROR) ? true : false;
   }
   else
   {
      // This is a write command
      retVal = false;		// read-only
   }

   return( retVal );
} // end of MB_Coil037

//*********************************************************************
bool MB_Coil038( bool isWrite, bool val )
{
   bool		retVal;

   if( !isWrite )
   {
      // This is a read command
      retVal = (SSP_status.bProbeStatus & TURB_DRIVER_ERROR) ? true : false;
   }
   else
   {
      // This is a write command
      retVal = false;		// read-only
   }

   return( retVal );
} // end of MB_Coil038


#if 0
#include "HARDWARE.H"        	/* Hardware Port Definitions */
#include "SS_PROBE.H"        	/* Main Program Definitions */
#include "MODBUS.H"           	/* ModBus Structures and function prototypes */
#include "SSPIO.H"           	/* I/O Structures and function prototypes */
#include "SSPCOM.H"		/* Com Definitions */
#include "SSP_Gain.H"		/* Gain Parameters */
#include "SSP_TURBIDITY.H"      /* Turbidity Structures and function prototypes */
#include "SSPWAVEFORM.H"      	/* Waveform Structures and function prototypes */
#include "SSP_WIPER.H"
#endif

/**** Internal Constants ****/
#if(0)
/***** Function Prototypes Internal to this module ***/

/***** Internal Variables ******/

/* Local Globals */
//uint16_t	wFirstReg;	/* First Register */
//uint16_t	wNbrRegs;	/* Number of Registers Requested */

uint8_t		*pRegData;	/* Pointer to data */

//uint8_t	bMbError;	/* Error Code for last operation */
uint8_t		bMbFlags = 0;	/* ModBuf Flags */
uint8_t		bCoil;		/* Coil Status */

#define MAX_MANUFACTURER_NAME_LEN	13					// Max Size of Mfg Name in Slave ID
#define MAX_SSP_MODEL_LEN		5						// Max Size	of SSP Model in Slave ID

const char MANUFACTURER_NAME[13] = 	"Badger Meter ";	// Must be 13 characters long
const char SSP_MODEL[5] = 		"SSP2 ";				// Must be 5 characters long

/* Bit Defintions for bMbFlags */
#define MB_READ_DATA	0x01	/* Flag for Read/ Write operation */
#define MB_LISTEN_ONLY	0x02	/* Flag to be in listen only mode */





/*
 **********************************************************************
 * @brief MB_WriteCoil() Decodes FC 5 Cmd
 *
 * @param msg - ptr to first byte of Modbus command
 *
 * @return none
 **********************************************************************
 */
void MB_WriteCoil(uint8_t *msg)
{

   uint8_t	x;
   uint8_t	nbrCoils;

   uint8_t	bErr;		// Error Code for last operation
   uint8_t	bFlags;		// ModBuf Flags

   uint16_t	firstReg;
   uint16_t	numberRegs;

   if(bMbFlags & MB_LISTEN_ONLY)
   {
      return;	// Listen only mode - no respose
   }

   firstReg = GetFirstReg( msg );
   bMbFlags &= ~(MB_READ_DATA);	// Clear flag
   pRegData = &InMsgBuf[4];	/* Point to data location */
   if(wFirstReg > NBR_MB_COILS) // If user requested coils outside of scope
   {
      GetTxBufMin(MODBUS_WRITE_COIL | 0x80);		// Return response with error
      SendByte((uchar)0x02);		// Return exception code 2
   }
   else
   {
      x = (uint8_t)firstReg;			// Start at register
      MB_COIL_MAP[x]();					// Update Requested Coils
      if(bMbError)	// Error When saving data
      {
	 GetTxBufMin(MODBUS_WRITE_COIL | 0x80);		// Return response with error
	 SendByte((uchar)bMbError);		// Return exception code
      }
      else	// Saved Data OK
      {
	 GetTxBufMin(MODBUS_WRITE_COIL);		// Return response
	 for( x=0 ; x < 4 ; ++x)					// Return Coil Location and value written
	 {
	    SendByte((uchar)InMsgBuf[2+x]);
	 }
      }
   }
   SendCS();             /* Compute CS and add to buffer */
   SendDataNow();        /* Send it */

   return;
} // end of MB_WriteCoil()

/*********************************************************************
 *                                                                    *
 *    Function Name: ReadHoldingRegEx                                 *
 *    Change Info:   10/25/10                                         *
 *    Description:   Added as a sub routine to save memory            *
 *    Parameters:    Starting and ending ModBus register              *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void ReadHoldingRegEx(WORD wStartReg, WORD wEndReg)
{
   BYTE x;
   X_WORD ix;

   if((wNbrRegs > MAX_MB_REG_COMM) || ((wNbrRegs + wFirstReg) > (wEndReg + 1)))
   {
      GetTxBufMin(MODBUS_READ_HOLDING_REG | 0x80);		// Return response with error
      SendByte((uchar)0x02);		// Return exception code 2
   }
   else
   {
      GetTxBufMin(MODBUS_READ_HOLDING_REG);		// Return response
      SendByte((uchar)(wNbrRegs * 2));		// Return number of bytes sent
      x = (uchar)wFirstReg - wStartReg;					// Start at first register
      ix = wFirstReg - wStartReg;
      while(wNbrRegs-- > 0)
      {
	 if(wStartReg == SENSOR_NAME_MB_REG_START)
	    MB_Reg40101(x++);					// Send Requested Registers
	 else if(wStartReg == SENSOR_SN_MB_REG_START)
	    MB_Reg40113(x++);
	 else if(wStartReg == 200)
	    MB_Reg40201(ix++);					// Send Requested Registers
	 else if(wStartReg == USER_DATA1_MB_REG_START)
	    MB_Reg40118(x++);
	 else if(wStartReg == USER_DATA2_MB_REG_START)
	    MB_Reg40128(x++);
	 else if(wStartReg == USER_DATA3_MB_REG_START)
	    MB_Reg40801(x++);					// Send Requested Registers
	 else if(wStartReg == USER_DATA4_MB_REG_START)
	    MB_Reg40848(x++);					// Send Requested Registers
	 else if(wStartReg == 0)
	    MB_REG_MAP[x++]();					// Send Requested Registers
      }
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_ReadHoldingRegister                           *
 *    Change Info:   06/23/08                                         *
 *    Description:   Decodes FC 3 Cmd                                 *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_ReadHoldingReg(void)
{
   if(bMbFlags & MB_LISTEN_ONLY) return;	// Listen only mode - no respose
   GetFirstReg();
   GetNbrRegs();
   bMbFlags |= MB_READ_DATA;	// Set flag
   if((wFirstReg >= SENSOR_NAME_MB_REG_START) && ( wFirstReg <= SENSOR_NAME_MB_REG_STOP))	// Sensor name request	Registers 40101 - 40112
      ReadHoldingRegEx(SENSOR_NAME_MB_REG_START,SENSOR_NAME_MB_REG_STOP);
   else if((wFirstReg >= SENSOR_SN_MB_REG_START) && ( wFirstReg <= SENSOR_SN_MB_REG_STOP))	// Sensor SN request	Registers 40113 - 40118
      ReadHoldingRegEx(SENSOR_SN_MB_REG_START,SENSOR_SN_MB_REG_STOP);
   else if((wFirstReg >= 200) && ( wFirstReg <= 712))	// Waveform data request	Registers 40201 - 40712
      ReadHoldingRegEx(200,712);
   else if((wFirstReg >= USER_DATA1_MB_REG_START) && ( wFirstReg <= USER_DATA1_MB_REG_STOP))	// User spific data field 1 40118 - 40134
      ReadHoldingRegEx(USER_DATA1_MB_REG_START,USER_DATA1_MB_REG_STOP);
   else if((wFirstReg >= USER_DATA2_MB_REG_START) && ( wFirstReg <= USER_DATA2_MB_REG_STOP))	// User spific data field 2 40134 - 40153
      ReadHoldingRegEx(USER_DATA2_MB_REG_START,USER_DATA2_MB_REG_STOP);
   else if((wFirstReg >= USER_DATA3_MB_REG_START) && ( wFirstReg <= USER_DATA3_MB_REG_STOP))	// User Specific Data Field 3 3x32 byte array. Registers 40801 - 40896
      ReadHoldingRegEx(USER_DATA3_MB_REG_START,USER_DATA3_MB_REG_STOP);
   else if((wFirstReg >= USER_DATA4_MB_REG_START) && ( wFirstReg <= USER_DATA4_MB_REG_STOP))	// User Specific Data Field 4 8x2 byte array. Registers 40901 - 40932
      ReadHoldingRegEx(USER_DATA4_MB_REG_START,USER_DATA4_MB_REG_STOP);
   else	/* Registers 40001 - 40045 */
      ReadHoldingRegEx(0,NBR_MB_REGS);
   SendCS();             /* Compute CS and add to buffer */
   SendDataNow();        /* Send it */
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_ReadInputRegister	                           *
 *    Change Info:   06/23/08                                         *
 *    Description:   Decodes FC 4 Cmd                                 *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_ReadInputReg(void)
{
   if(bMbFlags & MB_LISTEN_ONLY) return;	// Listen only mode - no respose
   GetTxBufMin(MODBUS_READ_INPUT_REG | 0x80);		// Return response with error
   SendByte((uchar)0x01);		// Return exception code 1 - illegal function
   SendCS();             /* Compute CS and add to buffer */
   SendDataNow();        /* Send it */
}


/*********************************************************************
 *                                                                    *
 *    Function Name: MB_WriteSingleRegister                           *
 *    Change Info:   06/23/08                                         *
 *    Description:   Decodes FC 6 Cmd                                 *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_WriteSingleReg(void)
{
   BYTE x;

   if(bMbFlags & MB_LISTEN_ONLY) return;	// Listen only mode - no respose
   GetFirstReg();
   bMbFlags &= ~(MB_READ_DATA);	// Clear flag
   pRegData = &InMsgBuf[4];	/* Point to data location */
   if((wFirstReg > NBR_MB_REGS) &&
	    (!((wFirstReg >= 100) && (wFirstReg <= 111))) && // If user requested registers outside of scope
	    (!((wFirstReg >= USER_DATA1_MB_REG_START) && (wFirstReg <= USER_DATA1_MB_REG_STOP))) && // If user requested registers outside of scope
	    (!((wFirstReg >= USER_DATA2_MB_REG_START) && (wFirstReg <= USER_DATA2_MB_REG_STOP))) && // If user requested registers outside of scope
	    (!((wFirstReg >= USER_DATA3_MB_REG_START) && (wFirstReg <= USER_DATA3_MB_REG_STOP))) && // If user requested registers outside of scope
	    (!((wFirstReg >= USER_DATA4_MB_REG_START) && (wFirstReg <= USER_DATA4_MB_REG_STOP)))) // If user requested registers outside of scope
   {
      GetTxBufMin(MODBUS_WRITE_SINGLE_REG | 0x80);		// Return response with error
      SendByte((uchar)0x02);		// Return exception code 2
   }
   else
   {
      if((wFirstReg >= 100) && ( wFirstReg <= 111))	// Sensor name req
      {
	 x = (uchar)wFirstReg - 100;			// Start at register
	 MB_Reg40101(x);					// Send Requested Registers
      }
      else if((wFirstReg >= USER_DATA1_MB_REG_START) && ( wFirstReg <= USER_DATA1_MB_REG_STOP))
      {
	 x = (uchar)wFirstReg - USER_DATA1_MB_REG_START;			// Start at register
	 MB_Reg40118(x);					// Send Requested Registers
      }
      else if((wFirstReg >= USER_DATA2_MB_REG_START) && ( wFirstReg <= USER_DATA2_MB_REG_STOP))
      {
	 x = (uchar)wFirstReg - USER_DATA2_MB_REG_START;			// Start at register
	 MB_Reg40128(x);					// Send Requested Registers
      }
      else if((wFirstReg >= USER_DATA3_MB_REG_START) && ( wFirstReg <= USER_DATA3_MB_REG_STOP))
      {
	 x = (uchar)wFirstReg - USER_DATA3_MB_REG_START;			// Start at register
	 MB_Reg40801(x);					// Send Requested Registers
      }
      else if((wFirstReg >= USER_DATA4_MB_REG_START) && ( wFirstReg <= USER_DATA4_MB_REG_STOP))
      {
	 x = (uchar)wFirstReg - USER_DATA4_MB_REG_START;			// Start at register
	 MB_Reg40848(x);					// Send Requested Registers
      }
      else
      {
	 x = (uchar)wFirstReg;			// Start at register
	 MB_REG_MAP[x]();					// Send Requested Registers
      }
      if(bMbError)	// Error When saving data
      {
	 GetTxBufMin(MODBUS_WRITE_SINGLE_REG | 0x80);		// Return response with error
	 SendByte((uchar)bMbError);		// Return exception code
      }
      else	// Saved Data OK
      {
	 GetTxBufMin(MODBUS_WRITE_SINGLE_REG);		// Return response
	 for( x=0 ; x < 4 ; ++x)					// Return Reg Location and value written
	    SendByte((uchar)InMsgBuf[2+x]);
      }
   }
   SendCS();             /* Compute CS and add to buffer */
   SendDataNow();        /* Send it */
}

/*********************************************************************
 *                                                                    *
 *    Function Name: WriteHoldingRegEx                                *
 *    Change Info:   10/25/10                                         *
 *    Description:   Added as a sub routine to save memory            *
 *    Parameters:    Starting and ending ModBus register              *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void WriteHoldingRegEx(WORD wStartReg, WORD wEndReg)
{
   X_WORD x;

   if((wNbrRegs > MAX_MB_REG_COMM) || ((wNbrRegs + wFirstReg) > (wEndReg + 1)))
   {
      GetTxBufMin(MODBUS_WRITE_MULT_REG | 0x80);		// Return response with error
      SendByte((uchar)0x02);		// Return exception code 2
   }
   else
   {
      x = wFirstReg - wStartReg;					// Start at first register
      while(wNbrRegs-- > 0)
      {
	 if(wStartReg == SENSOR_NAME_MB_REG_START)
	    MB_Reg40101(x++);					// Send Requested Registers
	 else if(wStartReg == USER_DATA1_MB_REG_START)
	    MB_Reg40118(x++);
	 else if(wStartReg == USER_DATA2_MB_REG_START)
	    MB_Reg40128(x++);
	 else if(wStartReg == USER_DATA3_MB_REG_START)
	    MB_Reg40801(x++);					// Send Requested Registers
	 else if(wStartReg == USER_DATA4_MB_REG_START)
	    MB_Reg40848(x++);					// Send Requested Registers
	 else if(wStartReg == 0)
	    MB_REG_MAP[x++]();					// Send Requested Registers
      }
      if(bMbError)	// Error When saving data
      {
	 GetTxBufMin(MODBUS_WRITE_MULT_REG | 0x80);		// Return response with error
	 SendByte((uchar)bMbError);		// Return exception code
      }
      else	// Saved Data OK
      {
	 GetTxBufMin(MODBUS_WRITE_MULT_REG);		// Return response
	 for( x=0 ; x < 4 ; ++x)					// Return Reg Location and number written
	    SendByte((uchar)InMsgBuf[2+x]);
      }
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_WriteMultRegister	                           *
 *    Change Info:   06/23/08                                         *
 *    Description:   Decodes FC 10 Cmd                                *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_WriteMultReg(void)
{
   if(bMbFlags & MB_LISTEN_ONLY) return;	// Listen only mode - no respose
   GetFirstReg();
   GetNbrRegs();
   bMbFlags &= ~(MB_READ_DATA);	// Clear flag
   pRegData = &InMsgBuf[7];	/* Point to data location */
   if((wFirstReg >= SENSOR_NAME_MB_REG_START) && ( wFirstReg <= SENSOR_NAME_MB_REG_STOP))	// Sensor name request	Registers 40101 - 40112
      WriteHoldingRegEx(SENSOR_NAME_MB_REG_START,SENSOR_NAME_MB_REG_STOP);
   else if((wFirstReg >= USER_DATA1_MB_REG_START) && ( wFirstReg <= USER_DATA1_MB_REG_STOP))	// User spific data field 1 40118 - 40134
      WriteHoldingRegEx(USER_DATA1_MB_REG_START,USER_DATA1_MB_REG_STOP);
   else if((wFirstReg >= USER_DATA2_MB_REG_START) && ( wFirstReg <= USER_DATA2_MB_REG_STOP))	// User spific data field 2 40134 - 40153
      WriteHoldingRegEx(USER_DATA2_MB_REG_START,USER_DATA2_MB_REG_STOP);
   else if((wFirstReg >= USER_DATA3_MB_REG_START) && ( wFirstReg <= USER_DATA3_MB_REG_STOP))	// User Specific Data Field 3 3x32 byte array. Registers 40801 - 40896
      WriteHoldingRegEx(USER_DATA3_MB_REG_START,USER_DATA3_MB_REG_STOP);
   else if((wFirstReg >= USER_DATA4_MB_REG_START) && ( wFirstReg <= USER_DATA4_MB_REG_STOP))	// User Specific Data Field 4 8x2 byte array. Registers 40901 - 40932
      WriteHoldingRegEx(USER_DATA4_MB_REG_START,USER_DATA4_MB_REG_STOP);
   else	/* Registers 40001 - 40043 */
      WriteHoldingRegEx(0,NBR_MB_REGS);
   SendCS();             /* Compute CS and add to buffer */
   SendDataNow();        /* Send it */
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Diag					                           *
 *    Change Info:   06/23/08                                         *
 *    Description:   Decodes FC 8 Cmd                                 *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Diag(void)
{
   GetFirstReg();	// wFirstReg now has sub function
   if((bMbFlags & MB_LISTEN_ONLY) && (wFirstReg != 1))
      return;	// Listen only mode - no respose except sub function 1
   switch (wFirstReg)
   {
      case 1:	// Restart comm
      case 10:	//Clear all counters
	 wBusMsgCount = 0;
	 wSlaveMsgCount = 0;
	 wCRCCount = 0;
	 if(!(bMbFlags & MB_LISTEN_ONLY) || (wFirstReg == 10))
	 {
	    GetTxBufMin(MODBUS_DIAG);		// Return response
	    SendWord(wFirstReg);
	    SendWord(0);
	 }
	 if(wFirstReg == 1)
	    bMbFlags &= ~(MB_LISTEN_ONLY);
	 break;
      case 4:	// Force Listen only mode
	 bMbFlags |= MB_LISTEN_ONLY;
	 break;
      case 11: // Return Bus Message Count
	 GetTxBufMin(MODBUS_DIAG);		// Return response
	 SendWord(wFirstReg);
	 SendWord(wBusMsgCount);
	 break;
      case 12: // Return CRC Count
	 GetTxBufMin(MODBUS_DIAG);		// Return response
	 SendWord(wFirstReg);
	 SendWord(wCRCCount);
	 break;
      case 14: // Return Slave Message Count
	 GetTxBufMin(MODBUS_DIAG);		// Return response
	 SendWord(wFirstReg);
	 SendWord(wSlaveMsgCount);
	 break;
      case 13: // Return Bus Exception Count
      case 15: // Return Slave no response message Count
      case 16: // Return Slave NAK Message Count
      case 17: // Return Slave Busy Message Count
      case 18: // Return Character overrun Message Count
	 GetTxBufMin(MODBUS_DIAG);		// Return response
	 SendWord(wFirstReg);
	 SendWord(0);
	 break;
      default:
	 GetTxBufMin(MODBUS_DIAG | 0x80);		// Return response with error
	 SendByte((uchar)0x01);		// Return exception code 1 - illegal function
	 break;
   }
   SendCS();             /* Compute CS and add to buffer */
   SendDataNow();        /* Send it */
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_SlaveID				                           *
 *    Change Info:   06/23/08                                         *
 *    Description:   Decodes FC 17 Cmd                                *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_SlaveID(void)
{
   BYTE x;

   GetTxBufMin(MODBUS_SLAVE_ID);		// Return response
   SendByte(MAX_MANUFACTURER_NAME_LEN + MAX_SSP_MODEL_LEN + sizeof(param.UserData_Field1) + 1);
   for ( x = 0 ; x < MAX_MANUFACTURER_NAME_LEN ; ++x )
      SendByte(MANUFACTURER_NAME[x]);
   for ( x = 0 ; x < MAX_SSP_MODEL_LEN ; ++x )
      SendByte(SSP_MODEL[x]);
   for ( x = 0 ; x < sizeof(param.UserData_Field1) ; ++x )
      SendByte(param.UserData_Field1[x]);	// Send Name
   SendByte(0xff);	// Always ON
   SendCS();             /* Compute CS and add to buffer */
   SendDataNow();        /* Send it */
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40001                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40001               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40001(void)
{
   X_INT	iVal;
   X_FLOAT	fVal;

   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      switch(param.Units)
      {		/* Assumes that fTrackMeasurement is already calibrated for Level/range and is in inches */
	 case UNITS_INCH:
	    fVal = (fTrackMeasurement) * 10;	/* Convert to 10 * inch measurement */
	    break;
	 case UNITS_METER:
	    fVal = (fTrackMeasurement * 2.54);	/* Convert to 10 * meter measurement */
	    break;
	 case UNITS_CM:
	    fVal = (fTrackMeasurement * 25.4);	/* Convert to 10 * cm measurement */
	    break;
	 case UNITS_FEET:
	 default:
	    fVal = (fTrackMeasurement *10)/12;	/* Convert to 10 * ft measurement */
	    break;
      }
      iVal = fVal;	// Get integer portion
      if(modf(fVal,&fVal) >= 0.50) ++iVal;	// Round value
      SendWord(iVal);
   }
   else
   {
      bMbError = 0x01;		// Error Code 1 - Write not allowed
      pRegData += 2;			// Advance pointer
   }
}


/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40002                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40002               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40002(void)
{
   BYTE uTmp;

   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.Units);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData > UNITS_CM)	// If invalid data
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 uTmp = param.Units;					// Save for later comparison
	 param.Units = *pRegData;	// Save Data
	 if(uTmp != param.Units)		// Did units change
	    UpdateNewUnits(uTmp);	// Update units
	 SaveSettings();
      }
      ++pRegData;			// Move to next byte for subsequent registers
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40003                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40003               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40003(void)
{
   uint iTmp;

   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.TankDepth);
   }
   else
   {
      iTmp = GetWord(pRegData);		// Get word value
      if(RangeScale(param.TankDepth) != RangeScale(iTmp))
	 warmBoot = TRUE;
      param.TankDepth = iTmp;
      SaveSettings();
      pRegData += 2;	// Advance pointer
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40004                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40004               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40004(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.ZeroAdjust);
   }
   else
   {
      param.ZeroAdjust = GetWord(pRegData);		// Get word value
      SaveSettings();
      pRegData += 2;		// Advance pointer
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40005                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40005               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40005(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.MinLevel);
   }
   else
   {
      param.MinLevel = GetWord(pRegData);		// Get word value
      SaveSettings();
      pRegData += 2;		// Adjust pointer to keep it correct
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40006                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40006               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40006(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.MaxLevel);
   }
   else
   {
      param.MaxLevel = GetWord(pRegData);		// Get word value
      SaveSettings();
      pRegData += 2;		// Adjust pointer
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40007                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40007               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40007(void)
{
#if(0)
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      if(param.BitFlags & AUTO_GAIN_ON)
	 SendByte(0x01);	// Show it's ON
      else
	 SendByte(0x00);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData)		// IF auto Gain on requested
	 param.BitFlags |= AUTO_GAIN_ON;	// Set flag
      else
	 param.BitFlags &= ~(AUTO_GAIN_ON);	// Clear flag
      SaveSettings();
   }
#endif
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.Sensitivity);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData >	100)
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.Sensitivity = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;			// Move to next byte for subsequent registers
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40008                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40008               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40008(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.WallZone);
   }
   else
   {
      param.WallZone = GetWord(pRegData);		// Get word value
      SaveSettings();
      pRegData += 2;	// Advance pointer
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40009                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40009               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40009(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.CellLimit);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData >	50)
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.CellLimit = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;			// Move to next byte for subsequent registers
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40010                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40010               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40010(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.Gmin);
   }
   else
   {
      param.Gmin = GetWord(pRegData);		// Get word value
      SaveSettings();
      pRegData += 2;		// Advance pointer
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40011                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40011               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40011(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.Gmax);
   }
   else
   {
      param.Gmax = GetWord(pRegData);		// Get word value
      SaveSettings();
      pRegData += 2;			// Advance pointer to keep it correct
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40012                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40012               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40012(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.History);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData >	250)
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.History = *pRegData;	// Save new data
	 if(param.History == 0)
	    param.History = 1;	// Check for zero
	 SetHistory(param.History);
	 SaveSettings();
      }
      ++pRegData;		// Advance pointer
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40013                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40013               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40013(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.SpeedSound);
   }
   else
   {
      param.SpeedSound = GetWord(pRegData);		// Get word value
      BuildRangeTable();	// Rebuild speed table
      SaveSettings();
      pRegData += 2;		// Advance pointer
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40014                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40014               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40014(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.GainIncrement);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if((*pRegData < 1) || (*pRegData >	50))
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.GainIncrement = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;		// Advance pointer
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40015                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40015               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40015(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.WallZoneAG);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData >	100)
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.WallZoneAG = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;			// Move to next byte for subsequent registers
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40016                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40016               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40016(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.AGSetPoint);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData >	50)
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.AGSetPoint = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;			// Move to next byte for subsequent registers
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40017                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40017               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40017(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.GainBand);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if((*pRegData < 5) || (*pRegData >	30))
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.GainBand = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;			// Move to next byte for subsequent registers
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40018                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40018               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40018(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.SetPoint4ma);
   }
   else
   {
      param.SetPoint4ma = GetWord(pRegData);		// Get word value
      SaveSettings();
      pRegData += 2;			// Advance pointer to keep it correct
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40019                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40019               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40019(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.SetPoint20ma);
   }
   else
   {
      param.SetPoint20ma = GetWord(pRegData);		// Get word value
      SaveSettings();
      pRegData += 2;			// Advance pointer
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40020                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40020               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40020(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.EchoLossAction);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData >	4)
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.EchoLossAction = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;		// Advance pointer
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40021                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40021               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40021(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.EchoDelay);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      param.EchoDelay = *pRegData;	// Save new data
      SaveSettings();
      ++pRegData;			// Advance pointer
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40022                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40022               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40022(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.LoopMin);
   }
   else
   {
      param.LoopMin = GetWord(pRegData);		// Get word value
      SaveSettings();
      pRegData += 2;			// Advance pointer
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40023                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40023               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40023(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.LoopMax);
   }
   else
   {
      param.LoopMax = GetWord(pRegData);		// Get word value
      SaveSettings();
      pRegData += 2;			// Advance pointer to keep it correct
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40024                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40024               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40024(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(iGainAdjust);
   }
   else
   {
      if((param.BitFlags & AUTO_GAIN_BIT) == AUTO_GAIN_OFF)	// If auto Gain is off
	 iGainAdjust = GetWord(pRegData);		// Get word value
      else
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      pRegData += 2;			// Advance pointer to keep it correct
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40025                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40025               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40025(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.PingDelay);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData > 100)	// If invalid data
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.PingDelay = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;			// Move to next byte for subsequent registers
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40026                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40026               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40026(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.UpdateRate);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData > 20)	// If invalid data
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.UpdateRate = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;		// Advance Pointer
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40027                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40027               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40027(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.Smoothing);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData > 25)	// If invalid data
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.Smoothing = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;			// Move to next byte for subsequent registers
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40028                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40028               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40028(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.DeltaSmoothing);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData > 25)	// If invalid data
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.DeltaSmoothing = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;			// Move to next byte for subsequent registers
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40029                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40029               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40029(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.WiperDelay);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData >	240)
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.WiperDelay = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;		// Advance pointer
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40030                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40030               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40030(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.Algorithum);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData > 1)	// If invalid data
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.Algorithum = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;			// Move to next byte for subsequent registers
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40031                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40031               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40031(void)

{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(iGainBandMidPoint);
   }
   else
   {
      iGainBandMidPoint = GetWord(pRegData);		// Get word value
      pRegData += 2;			// Advance pointer to keep it correct
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40032                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40032               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40032(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(param.UnitAddress);
   }
   else
   {
      ++pRegData;			// Skip first byte - always 0
      if(*pRegData > 240)	// If invalid data
	 bMbError = 0x03;		// Error Code 3 - Data not valid
      else
      {
	 param.UnitAddress = *pRegData;	// Save new data
	 SaveSettings();
      }
      ++pRegData;			// Move to next byte for subsequent registers
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40033                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40033               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40033(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0x00);
      SendByte(bRange);
   }
   else
   {
      bMbError = 0x03;		// Error Code 3 - Data not valid	 - Read Only
      pRegData += 2;			// Advance pointer
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40034                           				*
 *    Change Info:   06/23/09                                         *
 *    Description:   Process request for Register 40034               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40034(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.TurbLoopMin);
   }
   else
   {
      param.TurbLoopMin = GetWord(pRegData);		// Get word value
      SaveSettings();
      pRegData += 2;			// Advance pointer to keep it correct
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40035                           				*
 *    Change Info:   06/23/09                                         *
 *    Description:   Process request for Register 40035               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40035(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(param.TurbLoopMax);
   }
   else
   {
      param.TurbLoopMax = GetWord(pRegData);		// Get word value
      SaveSettings();
      pRegData += 2;			// Advance pointer
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40036                           				*
 *    Change Info:   06/23/09                                         *
 *    Description:   Process request for Register 40036               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40036(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      if(param.BitFlags & TURBIDITY_INSTALLED)
	 SendWord(iNTU);
      else
	 SendWord(0x00);	// Return 0 if no turbidity installed
   }
   else
   {
      bMbError = 0x01;		// Error Code 1 - Write not allowed
      pRegData += 2;			// Advance pointer
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40037                           				*
 *    Change Info:   09/16/08                                         *
 *    Description:   Process request for Register 40037               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40037(void)
{
   X_INT	iVal;
   float	fVal;

   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      switch(param.Units)
      {		/* Assumes that fTrackMeasurement is already calibrated for Level/range and is in inches */
	 case UNITS_INCH:
	    fVal = (fTrackMeasurement *10000)/param.TankDepth;	/* Convert to 0-10000 for AGM DC */
	    break;							 /*param.TankDepth = in or cm*/
	 case UNITS_METER:
	    fVal = ((fTrackMeasurement *2.54) *10000)/param.TankDepth;	/* Convert to 0-10000 for AGM DC */
	    break;									 /*param.TankDepth = in or cm*/
	 case UNITS_CM:
	    fVal = ((fTrackMeasurement *2.54)*10000)/param.TankDepth;	/* Convert to 0-10000 for AGM DC */
	    break;									/*param.TankDepth = in or cm*/
	 case UNITS_FEET:
	 default:
	    fVal = (fTrackMeasurement *10000)/param.TankDepth;	/* Convert to 0-10000 for AGM DC */
	    break;							 /*param.TankDepth = in or cm*/
      }
      iVal = fVal;	// Get integer portion
      if(modf(fVal,&fVal) >= 0.50) ++iVal;	// Round value
      SendWord(iVal);
   }
   else
   {
      bMbError = 0x01;		// Error Code 1 - Write not allowed
      pRegData += 2;			// Advance pointer
   }
}
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40038                           				*
 *    Change Info:   03/10/10                                         *
 *    Description:   Process request for Register 40038               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40038(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      if(param.BitFlags & TURBIDITY_INSTALLED)
	 SendWord(7000);	// Return Turbidity Sensor Wattage (7000 mW)
      else if(param.BitFlags & WIPER_INSTALLED)
	 SendWord(6000);	// Return Wiper Sensor Wattage (6000 mW)
      else
	 SendWord(5000);	// Return Standard Sensor Wattage (5000 mW)
   }
   else
   {
      bMbError = 0x01;		// Error Code 1 - Write not allowed
      pRegData += 2;			// Advance pointer
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40039                           				*
 *    Change Info:   03/10/10                                         *
 *    Description:   Process request for Register 40039               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40039(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendWord(iWiperMotorVolt);
   }
   else
   {
      bMbError = 0x01;		// Error Code 1 - Write not allowed
      pRegData += 2;			// Advance pointer
   }
}

#if(1)	/* To save code space unused registers are mapped in MB_REG_MAP[] to MB_Reg40039() which is also unused */
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40040                           				*
 *    Change Info:   03/10/10                                         *
 *    Description:   Process request for Register 40040               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40040(void)
{
   // Future nothing to do
   MB_Reg40039();
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40041                           				*
 *    Change Info:   03/10/10                                         *
 *    Description:   Process request for Register 40041               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40041(void)
{
   // Future nothing to do
   MB_Reg40039();
}
#endif

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40042                           				*
 *    Change Info:   03/10/10                                         *
 *    Description:   Process request for Register 40042               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40042(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0); /* filler */
      SendByte(param.HWID); /* Add HW ID */
   }
   else
   {
      bMbError = 0x01;		// Error Code 1 - Write not allowed
      pRegData += 2;			// Advance pointer
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40043                           			 *
 *    Change Info:   03/10/10                                         *
 *    Description:   Process request for Register 40043               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40043(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(0); /* filler */
      SendByte(FW_VERSION); /* Add FW Version */
   }
   else
   {
      bMbError = 0x01;		// Error Code 1 - Write not allowed
      pRegData += 2;			// Advance pointer
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40044                           			 *
 *    Change Info:   06/01/10                                         *
 *    Description:   Process request for Register 40044               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40044(void)
{
   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      if(param.BitFlags & TURBIDITY_INSTALLED)
	 SendWord(iNTU*2);
      else
	 SendWord(0x00);	// Return 0 if no turbidity installed
   }
   else
   {
      bMbError = 0x01;		// Error Code 1 - Write not allowed
      pRegData += 2;			// Advance pointer
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40045                           			 *
 *    Change Info:   06/01/10                                         *
 *    Description:   Process request for Register 40045               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40045(void)
{
   X_INT	iVal;
   X_FLOAT	fVal;

   if(bMbFlags & MB_READ_DATA)	// If Read
   {
      switch(param.Units)
      {		/* Assumes that fTrackMeasurement is already calibrated for Level/range and is in inches */
	 case UNITS_INCH:
	    fVal = (fDensity) * 10;	/* Convert to 10 * inch measurement */
	    break;
	 case UNITS_METER:
	    fVal = (fDensity * 2.54);	/* Convert to 10 * meter measurement */
	    break;
	 case UNITS_CM:
	    fVal = (fDensity * 25.4);	/* Convert to 10 * cm measurement */
	    break;
	 case UNITS_FEET:
	 default:
	    fVal = (fDensity *10)/12;	/* Convert to 10 * ft measurement */
	    break;
      }
      iVal = fVal;	// Get integer portion
      if(modf(fVal,&fVal) >= 0.50) ++iVal;	// Round value
      SendWord(iVal);
   }
   else
   {
      bMbError = 0x01;		// Error Code 1 - Write not allowed
      pRegData += 2;			// Advance pointer to keep it correct
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40101                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40101               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40101(uchar bReg)		// Sensor Name
{
   if(bReg > MAX_NAME_LEN)
      bMbError = 0x02;		// Error Code 2 - Address not valid
   else if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(param.Name[bReg * 2]);
      SendByte(param.Name[(bReg * 2) + 1]);
   }
   else
   {
      param.Name[bReg * 2] = *pRegData++;	// Get first character
      param.Name[(bReg * 2) + 1] = *pRegData++;	// Save new data
      SaveSettings();
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40113                           				*
 *    Change Info:   03/10/10                                         *
 *    Description:   Process request for Register 40113               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40113(uchar bReg)		// Sensor Serial Number
{
   X_BYTE cBuf[12];		// temp storage
   X_BYTE uTmp;
   X_WORD iSN;

   //	sprintf(cBuf,"%0d",param.SN);	// format string - insufficient space for sprintf
   // using alternate method of generating number in ASCII form
   iSN = param.SN;
   for(uTmp = 0 ; uTmp < 12 ; ++uTmp)	// fill buffer with zeros
      cBuf[uTmp] = '0';
   uTmp = iSN/10000;	// get 10,000's nbr
   // note string is inserted in memory with last element in MSB so that
   // ModBus commands display string correctly
   cBuf[7] = uTmp + 0x30;	// convert to ASCII
   iSN -= (unsigned int)uTmp * 10000;						// subtract
   uTmp = iSN/1000;	// get 1,000's nbr
   cBuf[8] = uTmp + 0x30;	// convert to ASCII
   iSN -= (unsigned int)uTmp * 1000;						// subtract
   uTmp = iSN/100;	// get 100's nbr
   cBuf[9] = uTmp + 0x30;	// convert to ASCII
   iSN -= (unsigned int)uTmp * 100;						// subtract
   uTmp = iSN/10;	// get 10's nbr
   cBuf[10] = uTmp + 0x30;	// convert to ASCII
   iSN -= (unsigned int)uTmp * 10;						// subtract
   cBuf[11] = iSN + 0x30;	// get 1's nbr and convert to ASCII
   if(bReg > 6)
      bMbError = 0x02;		// Error Code 2 - Address not valid
   else if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(cBuf[bReg * 2]);
      SendByte(cBuf[(bReg * 2) + 1]);
   }
   else
   {
      bMbError = 0x03;		// Error Code 3 - Data not valid	 - Read Only
      pRegData += 2;			// Advance pointer
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40118                           				*
 *    Change Info:   10/19/10                                         *
 *    Description:   Process request for Register 40118               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40118(uchar bReg)		// User Specific Data - Field 1.
{
   if(bReg > MAX_USER_DATA_FIELD1)
      bMbError = 0x02;		// Error Code 2 - Address not valid
   else if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(param.UserData_Field1[bReg * 2]);
      SendByte(param.UserData_Field1[(bReg * 2) + 1]);
   }
   else
   {
      param.UserData_Field1[bReg * 2] = *pRegData++;	// Get first character
      param.UserData_Field1[(bReg * 2) + 1] = *pRegData++;	// Save new data
      SaveSettings();
   }

}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40128                           				*
 *    Change Info:   10/19/10                                         *
 *    Description:   Process request for Register 40118               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40128(uchar bReg)		// User Specific Data - Field 2.
{
   if(bReg > MAX_USER_DATA_FIELD2)
      bMbError = 0x02;		// Error Code 2 - Address not valid
   else if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(param.UserData_Field2[bReg * 2]);
      SendByte(param.UserData_Field2[(bReg * 2) + 1]);
   }
   else
   {
      param.UserData_Field2[bReg * 2] = *pRegData++;	// Get first character
      param.UserData_Field2[(bReg * 2) + 1] = *pRegData++;	// Save new data
      SaveSettings();
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40201                           				*
 *    Change Info:   06/23/08                                         *
 *    Description:   Process request for Register 40201               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40201(uint bReg)	// Waveform Data
{
   if(bReg >= SAMPLES_IN_WAVEFORM/2)
      bMbError = 0x02;		// Error Code 2 - Address not valid
   else if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(SavedRawWF[bReg*2]);		// Return Echo Data
      SendByte(SavedRawWF[bReg*2 + 1]);		// Return Echo Data
   }
   else
   {
      bMbError = 0x03;		// Error Code 3 - Data not valid	 - Read Only
      pRegData += 2;			// Advance pointer
   }
   if(bitEnablePing == 0)			/* If Ping is disabled */
      EnablePinTimeOutSec = ENABLE_PING_TIMEOUT;	/* Reset Timer */
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40801                           				*
 *    Change Info:   10/19/10                                         *
 *    Description:   Process request for Register 40801               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40801(uchar bReg)		// User Specific Data Field 3 (3x32 array)
{
   if(bReg > MAX_USER_DATA_FIELD3)
      bMbError = 0x02;		// Error Code 2 - Address not valid
   else if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(param.UserData_Field3[bReg * 2]);
      SendByte(param.UserData_Field3[(bReg * 2) + 1]);
   }
   else
   {
      param.UserData_Field3[bReg * 2] = *pRegData++;	// Get first character
      param.UserData_Field3[(bReg * 2) + 1] = *pRegData++;	// Save new data
      SaveSettings();
   }
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Req40848                           				*
 *    Change Info:   10/19/10                                         *
 *    Description:   Process request for Register 40901               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Reg40848(uchar bReg)		// User Specific Data Field 4 (8x2 array)
{
   if(bReg > MAX_USER_DATA_FIELD4)
      bMbError = 0x02;		// Error Code 2 - Address not valid
   else if(bMbFlags & MB_READ_DATA)	// If Read
   {
      SendByte(param.UserData_Field4[bReg * 2]);
      SendByte(param.UserData_Field4[(bReg * 2) + 1]);
   }
   else
   {
      param.UserData_Field4[bReg * 2] = *pRegData++;	// Get first character
      param.UserData_Field4[(bReg * 2) + 1] = *pRegData++;	// Save new data
      SaveSettings();
   }
}















/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Coil013                           				*
 *    Change Info:   07/23/08                                         *
 *    Description:   Process request for Coil 013		               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Coil013(void)
{
   // Future nothing to do
   if((bMbFlags & MB_READ_DATA) == 0)	// If Write
      bMbError = 0x03;		// Error Code 3 - Data not valid
}

#if(0)	/* TO save code space unused coils are mapped in MB_COIL_MAP[] to Coil010() which is also unused */
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Coil014                           				*
 *    Change Info:   07/23/08                                         *
 *    Description:   Process request for Coil 014		               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Coil014(void)
{
   // Future nothing to do
   if((bMbFlags & MB_READ_DATA) == 0)	// If Write
      bMbError = 0x03;		// Error Code 3 - Data not valid
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Coil015                           				*
 *    Change Info:   07/23/08                                         *
 *    Description:   Process request for Coil 015		               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Coil015(void)
{
   // Future nothing to do
   if((bMbFlags & MB_READ_DATA) == 0)	// If Write
      bMbError = 0x03;		// Error Code 3 - Data not valid
}

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Coil016                           				*
 *    Change Info:   07/23/08                                         *
 *    Description:   Process request for Coil 016		               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Coil016(void)
{
   // Future nothing to do
   if((bMbFlags & MB_READ_DATA) == 0)	// If Write
      bMbError = 0x03;		// Error Code 3 - Data not valid
}
#endif

























#if(1)	/* To save code space unused coils are mapped in MB_COIL_MAP[] to Coil040() which is also unused */
/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Coil039                           				*
 *    Change Info:   07/23/08                                         *
 *    Description:   Process request for Coil 039		               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Coil039(void)
{
   // Future nothing to do
   MB_Coil038();
}

#endif

/*********************************************************************
 *                                                                    *
 *    Function Name: MB_Coil040                           				*
 *    Change Info:   07/23/08                                         *
 *    Description:   Process request for Coil 040		               *
 *    Parameters:    None                                             *
 *    Returns:       None                                             *
 *                                                                    *
 **********************************************************************/
void MB_Coil040(void)
{
   // Future nothing to do
}

#endif

// end of file modbus.c
