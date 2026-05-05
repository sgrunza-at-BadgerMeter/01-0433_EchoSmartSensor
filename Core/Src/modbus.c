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
#include "version.h"
#include "system_io.h"
#include "waveform.h"

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

//*********************************************************************
//*********************************************************************
//*
//* End of the Coil code, beginning of the Register code
//*
//*********************************************************************
//*********************************************************************


//*********************************************************************
//* Calculated interface position
//*
//* Current Interface (Register 40001)
//* The Current Interface register is used to indicate the location of
//* the detected interface level.
//*
//* Read only
//*********************************************************************
int16_t
   MB_Reg40001(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;;
   float		fVal;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 switch( SSP_configuration.units )
	 {
	    // Assumes that fTrackMeasurement is already calibrated for Level/range and is in inches
	    case UNITS_INCH:
	       fVal = (SSP_status.fTrackMeasurement) * 10;	/* Convert to 10 * inch measurement */
	       break;
	    case UNITS_METER:
	       fVal = (SSP_status.fTrackMeasurement * 2.54);	/* Convert to 10 * meter measurement */
	       break;
	    case UNITS_CM:
	       fVal = (SSP_status.fTrackMeasurement * 25.4);	/* Convert to 10 * cm measurement */
	       break;
	    case UNITS_FEET:
	    default:
	       fVal = (SSP_status.fTrackMeasurement *10)/12;	/* Convert to 10 * ft measurement */
	       break;
	 }
	 value = (int16_t) fVal;		// Get integer portion
	 if( modff(fVal,&fVal) >= 0.50 )
	 {
	    ++value;
	 }
	 *errCode = MBUS_RESPONSE_OK;
      }
      else
      {
	 *errCode = MBUS_RESPONSE_ILLEGAL_FUNCTION;
	 value = 0;
      }
   }

   return( value );
} // end of MB_Reg40001()

//*********************************************************************
//* Units
//*
//* The Units value determines what units the UUI, USI, USUI, and
//* BUI number (see section 4.4 of the EchoSmart Modbus Manual for
//* definitions of each) will represent
//*
//* 0 = feet
//* 1 = inches
//* 2 = meters
//* 3 = cm
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40002(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.units;

      }
      else
      {
	 // This is a write
	 if( ( val >= 0) && (val <= 3) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.units = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40002

//*********************************************************************
//* Tank Depth (Register 40003)
//*
//* Tank Depth is the distance from the surface of the water to the
//* bottom of the tank. It is a binary number in either inches or
//* centimeters. This is the complete span over which the sensor will
//* be calibrated.
//*
//* If the Units register (4.6.2) is set for feet or inches then the
//* binary number represents the measurement in inches. If the Units
//* register (4.6.2) is set for meters or centimeters then the number
//* represents the measurement in centimeters.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40003(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.tankDepth;
      }
      else
      {
	 // This is a write
	 *errCode = MBUS_RESPONSE_OK;
	 SSP_configuration.tankDepth = val;
      }
   }

   return( value );
} // end of MB_Reg40003()

//*********************************************************************
//* Zero Adjust (Register 40004)
//*
//* Zero Adjust is an offset value which locates the face of the sensor
//* above or below the surface of the water level of the tank. This
//* value modifies the level/range measurement to adjust for the sensor
//* location. The value can be positive (sensor face installed below
//* the water level) or negative (sensor face installed above the water
//* level – i.e. in a standpipe).
//*
//* If the Units register (4.6.2) is set for feet or inches then the
//* binary number represents the measurement in inches. If the Units
//* register (4.6.2) is set for meters or centimeters then the number
//* represents the measurement in centimeters.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40004(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.zeroAdjust;
      }
      else
      {
	 // This is a write
	 *errCode = MBUS_RESPONSE_OK;
	 SSP_configuration.zeroAdjust = val;
      }
   }

   return( value );
} // end of MB_Reg40004()

//*********************************************************************
//* Min Range (Register 40005)
//*
//* Minimum Range is an adjustable value that determines the minimum
//* distance that the sensor will measure. This number is always
//* positive and must be less than the Tank Depth value.
//*
//* If the Units register (4.6.2) is set for feet or inches then the
//* binary number represents the measurement in inches. If the Units
//* register (4.6.2) is set for meters or centimeters then the number
//* represents the measurement in centimeters.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40005(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.minLevel;
      }
      else
      {
	 // This is a write
	 if( (val > 0) && (val <= SSP_configuration.tankDepth) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.minLevel = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40005()

//*********************************************************************
//* Max Range (Register 40006)
//*
//* Maximum Range is an adjustable value that determines the maximum
//* distance that the sensor will measure. This number is always
//* positive and must be less than 382 inches (9715 cm). When the Tank
//* Depth parameter (4.6.3) is changed, this number is automatically
//* adjusted to 110% of the Tank Depth value.
//*
//* If the Units register (4.6.2) is set for feet or inches then the
//* binary number represents the measurement in inches. If the Units
//* register (4.6.2) is set for meters or centimeters then the number
//* represents the measurement in centimeters.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40006(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.maxLevel;
      }
      else
      {
	 // This is a write
	 if( val > 0 )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.maxLevel = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40006()

//*********************************************************************
//* Sensitivity (Register 40007)
//*
//* Interfaces are detected by analysis of wave form derivatives.
//* Sensitivity is an adjustable threshold that determines which
//* derivatives are valid for tracking. Valid range of numbers is from
//* 0 to 100.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40007(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.sensitivity;
      }
      else
      {
	 // This is a write
	 if( (val >= 0) && (val <= 100) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.sensitivity = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40007()

//*********************************************************************
//* Wall Zone (Register 40008)
//*
//* The Wall Zone value determines the size of the adjustable distance
//* above the tank bottom.
//*
//* If the Units register (4.6.2) is set for feet or inches then the
//* binary number represents the measurement in inches. If the Units
//* register (4.6.2) is set for meters or centimeters then the number
//* represents the measurement in centimeters.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40008(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.wallZone;
      }
      else
      {
	 // This is a write
	 if( (val >= 0) && (val <= 100) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.wallZone = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40008()

//*********************************************************************
//* Gate Rate (Register 40009)
//*
//* The Gate Rate value determines the maximum movement of the gates in
//* any one update. Valid range of the number is from 0 to 50.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40009(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.cellLimit;
      }
      else
      {
	 // This is a write
	 if( (val >= 0) && (val <= 50) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.cellLimit = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40009()

//*********************************************************************
//* LG Min (Register 40010)
//*
//* The LG Min value determines the minimum distance that the left gate
//* can be placed from the current track value.
//*
//* If the Units register (4.6.2) is set for feet or inches then the
//* binary number represents the measurement in inches. If the Units
//* register (4.6.2) is set for meters or centimeters then the number
//* represents the measurement in centimeters.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40010(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.lgMin;
      }
      else
      {
	 // This is a write
	 *errCode = MBUS_RESPONSE_OK;
	 SSP_configuration.lgMin = val;
      }
   }

   return( value );
} // end of MB_Reg40010()

//*********************************************************************
//* RG Min (Register 40011)
//*
//* The RG Min value determines the minimum distance that the right
//* gate can be placed from the current track value.
//*
//* If the Units register (4.6.2) is set for feet or inches then the
//* binary number represents the measurement in inches. If the Units
//* register (4.6.2) is set for meters or centimeters then the number
//* represents the measurement in centimeters.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40011(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.rgMin;
      }
      else
      {
	 // This is a write
	 *errCode = MBUS_RESPONSE_OK;
	 SSP_configuration.rgMin = val;
      }
   }

   return( value );
} // end of MB_Reg40011()

//*********************************************************************
//* Dampening (Register 40012)
//*
//* The Dampening register is used to average the current track
//* location (reported interface location). The dampening value
//* determines the number of updates that will be averaged together to
//* provide a track location. Valid range of the number is from 1 to 250.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40012(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.history;
      }
      else
      {
	 // This is a write
	 if( (val >= 1) && (val <= 250) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.history = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40012()

//*********************************************************************
//* Sound Speed (Register 40013)
//*
//* The Sound Speed register is used to adjust the speed of sound used
//* for all sensor distance measurements.
//*
//* If the Units register (4.6.2) is set for feet or inches then the
//* binary number represents the measurement in feet per second (FPS).
//* If the Units register (4.6.2) is set for meters or centimeters then
//* the number represents the measurement in meters per second (MPS).
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40013(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.speedSound;
      }
      else
      {
	 // This is a write
	 *errCode = MBUS_RESPONSE_OK;
	 SSP_configuration.speedSound = val;
	 buildRangeTable( &fRangeResolution[0], SSP_configuration.speedSound );
      }
   }

   return( value );
} // end of MB_Reg40013()



//*********************************************************************
//* Gain Increment (Register 40014)
//*
//* The Gain Increment register determines the step size in the
//* auto-gain adjustment. Valid range of the number is from 5 to 50.
//* The reported binary value is times 10 the value utilized by the
//* sensor (i.e. 5 = 0.5).
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40014(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.gainIncrement;
      }
      else
      {
	 // This is a write
	 if( (val >= 1) && (val <= 50) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.gainIncrement = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40014()

//*********************************************************************
//* Wall Zone AG (Register 40015)
//*
//* The Wall Zone AG controls the amount of gain applied to amplify the
//* waveform in the Wall Zone (4.6.8) when the auto gain coil is set to
//* 1 (ON) (4.5.1). The valid range of the register is 0 to 100 with 10
//* as default. Lowering this number will decrease the gain applied to
//* the waveform. Raising this number will increase the amount of gain.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40015(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.wallZoneAG;
      }
      else
      {
	 // This is a write
	 if( (val >= 0) && (val <= 100) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.wallZoneAG = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40015()

//*********************************************************************
//* AG Set Point (Register 40016)
//*
//* The AG Set Point controls the amount of gain applied to amplify the
//* waveform not in the Wall Zone (4.6.8) when the auto gain coil is
//* set to 1 (ON) (4.5.1). The valid range of the register is 0 to 50
//* with 10 as default. Lowering this number will decrease the gain
//* applied to the waveform. Raising this number will increase the
//* amount of gain.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40016(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.AGsetPoint;
      }
      else
      {
	 // This is a write
	 if( (val >= 0) && (val <= 50) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.AGsetPoint = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40016()

//*********************************************************************
//* Gain Band (Register 40017)
//*
//* The Gain Band register is the range in which the gain is permitted
//* to fluctuate ± the Gain Band Mid Point value (4.6.31). Valid range
//* is from 5 to 30.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40017(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.gainBand;
      }
      else
      {
	 // This is a write
	 if( (val >= 5) && (val <= 30) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.gainBand = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40017()

//*********************************************************************
//* SP 4mA (Register 40018)
//*
//* The 4mA Set Point (SP) register is used to specify the value that
//* will be used as the 4mA set point for the analog output scaling.
//* This value is typically set to zero.
//*
//* If the Measure coil (4.5.2) is set for Level, the number
//* represents the Level measurement. If the Measure coil is set for
//* Range, the number represents the Range measurement.  If the Units
//* register (4.6.2) is set for feet or inches then the binary number
//* represents the measurement in inches. If the Units register
//* (4.6.2) is set for meters or centimeters then the number represents
//* the measurement in centimeters.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40018(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.setPoint4ma;
      }
      else
      {
	 // This is a write
	 if( (val >= 5) && (val <= 30) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.setPoint4ma = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40018()

//*********************************************************************
//* SP 20mA (Register 40019)
//*
//* The 20mA Set Point (SP) register is used to specify the value that
//* will be used as the 20mA set point for the analog output scaling.
//* This value is typically set to the Tank Depth value (4.6.3).
//*
//* If the Measure coil (4.5.2) is set for Level, the number represents
//* the Level measurement. If the Measure coil is set for Range, the
//* number represents the Range measurement.
//*
//* If the Units register (4.6.2) is set for feet or inches then the
//* binary number represents the measurement in inches. If the Units
//* register (4.6.2) is set for meters or centimeters then the number
//* represents the measurement in centimeters.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40019(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.setPoint20mA;
      }
      else
      {
	 // This is a write
	 *errCode = MBUS_RESPONSE_OK;
	 SSP_configuration.setPoint20mA = val;
      }
   }

   return( value );
} // end of MB_Reg40019()

//*********************************************************************
//* EL Action (Register 40020)
//*
//* The Echo Loss (EL) Action register determines what action will be
//* taken on the Level current loop if there is a loss of echo for the
//* duration of the Echo Delay (4.6.21). For this action to be valid
//* the Echo Loss coil (4.5.4) must be set ON.
//*
//* Value 	Action
//* 0		Force 4 ma level (Drive Low)
//* 1		Force 20 ma level (Drive High)
//* 2		Cycle – Alternate Low and High
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40020(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.echoLossAction;
      }
      else
      {
	 // This is a write
	 if( (val >= 0) && (val <= 2) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.echoLossAction = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40020()

//*********************************************************************
//* Echo Delay (Register 40021)
//*
//* The Echo Delay register determines the number of minutes the sensor
//* must experience a loss of signal condition before the sensor
//* initiates the Echo Loss Action (4.6.20). If the Echo Loss coil
//* (4.5.4) is turned OFF, the value in the Echo Delay register is
//* ignored and the sensor will not initiate the Echo Loss Action.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40021(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.echoDelay;
      }
      else
      {
	 // This is a write
	 *errCode = MBUS_RESPONSE_OK;
	 SSP_configuration.echoDelay = val;
      }
   }

   return( value );
} // end of MB_Reg40021()

//*********************************************************************
//* CP 4mA (Register 40022)
//*
//* The 4mA Calibration Point (CP) register contains the binary number
//* necessary to write to the hardware to generate a 4mA signal on the
//* current loop used for the level.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40022(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.levelLoopMin;
      }
      else
      {
	 // This is a write
	 *errCode = MBUS_RESPONSE_OK;
	 SSP_configuration.levelLoopMin = val;
      }
   }

   return( value );
} // end of MB_Reg40022()

//*********************************************************************
//* CP 4mA (Register 40022)
//*
//* The 20mA Calibration Point (CP) register contains the binary number
//* necessary to write to the hardware to generate a 20mA signal on the
//* current loop used for the level.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40023(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.levelLoopMax;
      }
      else
      {
	 // This is a write
	 *errCode = MBUS_RESPONSE_OK;
	 SSP_configuration.levelLoopMax = val;
      }
   }

   return( value );
} // end of MB_Reg40023()

//*********************************************************************
//* Current Gain (Register 40024)
//*
//* When the Auto Gain coil (4.5.1) is OFF, the Current Gain register
//* is used to control the gain amplification value. In this mode the
//* Current Gain register provides read/write capability. When the Auto
//* Gain coil is ON, the Current Gain register is read only and provides
//* an indication of the current gain value. Valid Range of number is
//* from -4095 to +4095.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40024(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.gainAdjust;
      }
      else
      {
	 // This is a write
	 if( (val >= -4095) && (val <= 4095) )
	 {
	    if( !SSP_status.auto_gain )
	    {
	       *errCode = MBUS_RESPONSE_OK;
	       SSP_configuration.gainAdjust = val;
	    }
	    else
	    {
	       // read-only auto gain is on
	       *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	    }
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40024()

//*********************************************************************
//* Ping Rate Delay (Register 40025)
//*
//* The sensor normally collects waveform data at a pre-programmed rate.
//* The ping rate delay register is used to add a delay into the
//* waveform collection sequence. Valid range of the register is 0 to 100.
//* The rate for each number is in 10mS units. If the ping rate delay
//* register is set to “0” there is no delay added. If the ping rate
//* delay register is set to “5” there would be a 50mS delay between
//* the collection of each waveform.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40025(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.pingDelay;
      }
      else
      {
	 // This is a write
	 if( (val >= 0) && (val <= 100) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.pingDelay = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40025()

//*********************************************************************
//* Update Rate (Register 40026)
//*
//* The Update Rate register is used to determine how many waveforms
//* will be combined into a single waveform that is used for analysis.
//* With a setting of 1, the sensor will average 8 waveforms. The
//* maximum setting of 20 results in a total of 160 waveforms combined
//* into the resulting final waveform. (Note: An update rate of 0
//* provides a single non-combined waveform).
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40026(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.updateRate;
      }
      else
      {
	 // This is a write
	 if( (val >= 1) && (val <= 20) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.updateRate = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40026()

//*********************************************************************
//* Smoothing Rate (Register 40027)
//*
//* The Smoothing Rate register is used to smooth the 1024 cells to
//* produce the final waveform for analysis. Valid range of numbers is
//* from 0 to 25. A value of 0 provides no smoothing.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40027(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.smoothing;
      }
      else
      {
	 // This is a write
	 if( (val >= 0) && (val <= 25) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.smoothing = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40027()

//*********************************************************************
//* Delta Smoothing (Register 40028)
//*
//* The Delta Smoothing Rate register is used to smooth the derivative
//* waveform in the same manner as the Smoothing Rate (Register 40027)
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40028(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.deltaSmoothing;
      }
      else
      {
	 // This is a write
	 if( (val >= 0) && (val <= 25) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.deltaSmoothing = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40028()

//*********************************************************************
//* Wiper Delay (Register 40029)
//*
//* The Wiper Delay register determines the number of seconds that the
//* wiper (when installed) is off between cycles. Valid range of the
//* number is from 0 to 240. A value of 0 indicates that the wiper is
//* OFF and should not be cycled
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40029(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.wiperDelay;
      }
      else
      {
	 // This is a write
	 if( (val >= 0) && (val <= 240) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.wiperDelay = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40029()

//*********************************************************************
//* Interface (Register 40030)
//*
//* The Interface register determines how the sensor interprets the
//* waveform data. If the register is set to 0 then the sensor selects
//* the “first” (closest to the sensor) derivative which satisfies the
//* Sensitivity threshold (4.6.7). If the register value is set to 1
//* then the sensor selects the “last” (farthest from the sensor)
//* interface.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40030(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.algorithm;
      }
      else
      {
	 // This is a write
	 if( (val == 0) || (val == 1) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.algorithm = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40030()

//*********************************************************************
//* Gain Band MP (Register 40031)
//*
//* The Gain Band Mid Point (MP) register is used to position the gain
//* band. During initialization, the sensor calculates a gain band mid
//* point. When Auto Gain is enabled (4.5.1) the sensor will
//* automatically adjust the gain about this mid point. The amount of
//* adjustment is determined by the Gain Band register (4.6.17).
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40031(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.fixedGainBandMidPoint;
      }
      else
      {
	 // This is a write
	 if( (val == 0) || (val == 1) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.fixedGainBandMidPoint = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40031()

//*********************************************************************
//* Sensor Address (Register 40032)
//*
//* The Sensor Address register contains the network address for the
//* sensor. Once this register is changed the sensor will then
//* communicate using the new network address.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40032(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.address;
      }
      else
      {
	 // This is a write
	 if( (val >= 1) && (val <= 248) )
	 {
	    *errCode = MBUS_RESPONSE_OK;
	    SSP_configuration.address = val;
	 }
	 else
	 {
	    *errCode = MBUS_RESPONSE_ILLEGAL_DATA_VALUE;
	 }
      }
   }

   return( value );
} // end of MB_Reg40032()

//*********************************************************************
//* Op Range Index (Register 40033)
//*
//* This read-only register is used to indicate the operating range
//* that the sensor is currently using. Since the operating range
//* determines the basic sensor sample timing, this register is used to
//* determine the timing used for sample points in the waveform data.
//*
//* Index 	Operating Range 	Sample Time
//*	0	5 ft (1.524 m)		2.501687886 uSec
//*	1	10 ft (3.048 m)		5.003375771 uSec
//*	2	20 ft (6.096 m)		10.00675154 uSec
//*	3	30 ft (9.144 m)		15.01012731 uSec
//*
//* Read-only
//*********************************************************************
int16_t
   MB_Reg40033(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.address;
      }
      else
      {
	 // This is a write
	 *errCode = MBUS_RESPONSE_ILLEGAL_FUNCTION;
      }
   }

   return( value );
} // end of MB_Reg40033()

//*********************************************************************
//* TCP 4mA (Register 40034)
//*
//* The 4mA Turbidity Calibration Point (TCP) register contains the
//* binary number necessary to write to the hardware to generate a 4mA
//* signal on the current loop used for the turbidity.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40034(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.turbLoopMin;
      }
      else
      {
	 // This is a write
	 *errCode = MBUS_RESPONSE_OK;
	 SSP_configuration.turbLoopMin = val;
      }
   }

   return( value );
} // end of MB_Reg40034()

//*********************************************************************
//* TCP 20mA (Register 40035)
//*
//* The 20mA Turbidity Calibration Point (TCP) register contains the
//* binary number necessary to write to the hardware to generate a 20mA
//* signal on the current loop used for the turbidity.
//*
//* Read/Write
//*********************************************************************
int16_t
   MB_Reg40035(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 value = SSP_configuration.turbLoopMax;
      }
      else
      {
	 // This is a write
	 *errCode = MBUS_RESPONSE_OK;
	 SSP_configuration.turbLoopMax = val;
      }
   }

   return( value );
} // end of MB_Reg40035()

//*********************************************************************
//* Turbidity NTU (Register 40036)
//*
//* The Turbidity NTU register returns the current turbidity NTU value
//* times 100 (0-5000). Divide the returned number by 100 to get the
//* actual NTU value. If the turbidity option is not installed and
//* enabled (See 4.5.12) then the register returns a 0 value. Trying
//* to write to this register returns an error.
//*
//* Read only
//*********************************************************************
int16_t
   MB_Reg40036(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 if( SSP_configuration.hasTurbidity )
	 {
	    value = SSP_status.ntu;
	 }
	 else
	 {
	    value = 0;
	 }
      }
      else
      {
	 // This is a write to a read-only register
	 *errCode = MBUS_RESPONSE_ILLEGAL_FUNCTION;
      }
   }

   return( value );
} // end of MB_Reg40036()

//*********************************************************************
//* Track Percentage (Register 40037)
//*
//* The Track Percentage register returns the current Level as a
//* percentage of Tank Depth. The value in the register is 10 times
//* the actual percentage (0-10000). Divide the returned number by 10
//* to get the actual percentage. Trying to write to this register
//* returns an error.
//*
//* Read only
//*********************************************************************
int16_t
   MB_Reg40037(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{
   int16_t		i16Val = val;

   float		fVal;



   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;

	 switch( SSP_configuration.units )
	 {
	    // Assumes that fTrackMeasurement is already calibrated for Level/range and is in inches
	    case UNITS_INCH:
	       fVal = (SSP_status.fTrackMeasurement * 10000) / SSP_configuration.tankDepth;
	       break;

	    case UNITS_METER:
	       fVal = (SSP_status.fTrackMeasurement * 2.54 * 10000) / SSP_configuration.tankDepth;
	       break;

	    case UNITS_CM:
	       fVal = (SSP_status.fTrackMeasurement * 2.54 * 10000) / SSP_configuration.tankDepth;
	       break;

	    case UNITS_FEET:
	    default:
	       fVal = (SSP_status.fTrackMeasurement * 10000) / SSP_configuration.tankDepth;
	       break;
	 }

	 i16Val = fVal;
	 if( modff( fVal, &fVal ) >= 0.50 )
	 {
	    ++i16Val;
	 }
      }
      else
      {
	 // This is a write to a read-only register
	 *errCode = MBUS_RESPONSE_ILLEGAL_FUNCTION;
      }
   }

   return( i16Val );
} // end of MB_Reg40037()

//*********************************************************************
//* Power Value (Register 40038)
//*
//* The Power Value register returns the power required for the current
//* mode – Standby or High Power (4.5.11). The value in the register is
//* in milliwatts (mW). Trying to write to this register returns an
//* error.
//*
//* Read only
//*********************************************************************
int16_t
   MB_Reg40038(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{

   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;
	 value = SYS_POWER_BASE_VAL;

	 if( SSP_configuration.hasTurbidity )
	 {
	    value += SYS_POWER_TURB_VAL;
	 }

	 if( SSP_configuration.hasWiper )
	 {
	    value += SYS_POWER_WIPER_VAL;
	 }
      }
      else
      {
	 // This is a write to a read-only register
	 *errCode = MBUS_RESPONSE_ILLEGAL_FUNCTION;
      }
   }

   return( value );
} // end of MB_Reg40038()

//*********************************************************************
//* Power Value (Register 40039)
//*
//* Returns wiper motor voltage in mV
//*
//* Read only
//*********************************************************************
int16_t
   MB_Reg40039(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{

   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;
	 value = SSP_status.wiperMotorVoltage;
      }
      else
      {
	 // This is a write to a read-only register
	 *errCode = MBUS_RESPONSE_ILLEGAL_FUNCTION;
      }
   }

   return( value );
} // end of MB_Reg40039()

//*********************************************************************
//* Hardware Version (Register 40042)
//*
//* The Hardware Version register returns the version number of the
//* hardware. Trying to write to this register returns an error.
//*
//* Read only
//*********************************************************************
int16_t
   MB_Reg40042(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{

   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;
	 value = SSP_configuration.hw_id;
      }
      else
      {
	 // This is a write to a read-only register
	 *errCode = MBUS_RESPONSE_ILLEGAL_FUNCTION;
      }
   }

   return( value );
} // end of MB_Reg40042()

//*********************************************************************
//* Software Version (Register 40043)
//*
//* The Software Version register returns the version number of the
//* software. Trying to write to this register returns an error.
//*
//* Read only
//*********************************************************************
int16_t
   MB_Reg40043(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{

   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;
	 value = (FW_VERSION_MSB << 8) | FW_VERSION_LSB;
      }
      else
      {
	 // This is a write to a read-only register
	 *errCode = MBUS_RESPONSE_ILLEGAL_FUNCTION;
      }
   }

   return( value );
} // end of MB_Reg40043()

//*********************************************************************
//* Turbidity NTU Percentage (Register 40044)
//*
//* The Turbidity NTU Percentage register returns the current Turbidity
//* NTU percentage value times 100 (0-10000). Divide the returned number
//* by 100 to get the actual NTU percentage value. If the Turbidity
//* Installed coil (4.5.12) is not set, then the register returns a 0
//* value. Trying to write to this register returns an error.
//*
//* Read only
//*********************************************************************
int16_t
   MB_Reg40044(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{

   int16_t		value = val;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;
	 if( SSP_configuration.hasTurbidity )
	 {
	    value = SSP_status.ntu * 2;
	 }
	 else
	 {
	    value = 0;
	 }
      }
      else
      {
	 // This is a write to a read-only register
	 *errCode = MBUS_RESPONSE_ILLEGAL_FUNCTION;
      }
   }

   return( value );
} // end of MB_Reg40044()

//*********************************************************************
//* Dispersed Solids (Register 40045)
//*
//* The Dispersed Solids register is used to indicate the detected
//* interface level for the dispersed solids. The setting of the Units
//* register (4.6.2) determines the base number representation as shown
//* in Figure 10. In all cases the binary number presented represents
//* times 10 the base measurement (i.e. 200 = 20.0 units). See
//* description of USUI format for additional information on the format
//* of the number contained in this register.
//*
//* Read only
//*********************************************************************
int16_t
   MB_Reg40045(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{

   int16_t		value = val;
   float		fVal;

   if( errCode != NULL )
   {
      if( !isWrite )
      {
	 // This is a read
	 *errCode = MBUS_RESPONSE_OK;


	 switch( SSP_configuration.units )
	 {		/* Assumes that fTrackMeasurement is already calibrated for Level/range and is in inches */
	    case UNITS_INCH:
	       fVal = (SSP_status.fDensity) * 10;	/* Convert to 10 * inch measurement */
	       break;

	    case UNITS_METER:
	       fVal = (SSP_status.fDensity * 2.54);	/* Convert to 10 * meter measurement */
	       break;

	    case UNITS_CM:
	       fVal = (SSP_status.fDensity * 25.4);	/* Convert to 10 * cm measurement */
	       break;

	    case UNITS_FEET:
	    default:
	       fVal = (SSP_status.fDensity *10)/12;	/* Convert to 10 * ft measurement */
	       break;
	 }

	 value = fVal;	// Get integer portion
	 if( modff(fVal,&fVal) >= 0.50 )
	 {
	    ++value;	// Round value
	 }
      }
      else
      {
	 // This is a write to a read-only register
	 *errCode = MBUS_RESPONSE_ILLEGAL_FUNCTION;
      }
   }

   return( value );
} // end of MB_Reg40045()

//*********************************************************************
//* Sensor Name (Registers 40101 – 40112)
//*
//* The Sensor Name is contained in a series of twelve (12) successive
//* registers. Each register contains two ASCII characters.
//*
//* Read / Write
//*********************************************************************
int16_t
   MB_SensorName(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{

   int16_t		retVal = 0;
   uint16_t		twoChars;
   uint8_t		index;


   if( errCode != NULL )
   {
      *errCode = MBUS_RESPONSE_OK;

      index = reg - SENSOR_NAME_MB_REG_START;

      // each register is two ASCII characters
      index = index * 2;

      if( !isWrite )
      {
	 // This is a read
	 twoChars = (uint16_t)(SSP_configuration.name[index] << 8);
	 twoChars = (uint16_t)twoChars | (uint8_t)SSP_configuration.name[index+1];
	 retVal = twoChars;
      }
      else
      {
	 // This is a write
	 SSP_configuration.name[index] = val >> 8;
	 SSP_configuration.name[index+1] = val & 0xFF;
      }
   }

   return( retVal );
} // end of MB_SensorName()

//*********************************************************************
//* Serial Number (Registers 40113 – 40118)
//*
//* The sensor Serial Number is contained in a series of six (6)
//* successive registers. Each register contains two ASCII characters.
//*
//* These registers are filled in from the uint16_t sn field of
//* the configuration structure
//*
//* Read only
//*********************************************************************
int16_t
   MB_SerialNum(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{

   int16_t		retVal = 0;
   uint16_t		twoChars;
   uint8_t		index;

   char			serialNumString[ MAX_SN_LEN ];


   if( errCode != NULL )
   {
      *errCode = MBUS_RESPONSE_OK;

      index = reg - SENSOR_SN_MB_REG_START;

      // each register is two ASCII characters
      index = index * 2;

      if( !isWrite )
      {
	 // This is a read
	 snprintf( serialNumString, MAX_SN_LEN, "%5.5d", SSP_configuration.sn );
	 twoChars = (uint16_t)(serialNumString[index] << 8);
	 twoChars = (uint16_t)twoChars | (uint8_t)serialNumString[index+1];
	 retVal = twoChars;
      }
      else
      {
	 // This is a write to a read-only register
	 *errCode = MBUS_RESPONSE_ILLEGAL_FUNCTION;
      }
   }

   return( retVal );
} // end of MB_SerialNum()

#if 0

This code is part of an OEM specific variant no longer built or supported

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
#endif


//*********************************************************************
//* Waveform Data (Registers 40201 – 40712)
//*
//* Waveform Data exists as a series of 1024 data points. Each data
//* point is a byte value with a valid range of 0-255. Each register
//* is a 16-bit value and contains 2 of the 1024 data points. Register
//* 40200 contains data points 0 & 1 and register 40711 contains data
//* points 1022 & 1023.
//*
//* Note: When reading multiple register values (ModBus Function Code
//* 03) you can only access a maximum of 15 consecutive registers.
//*
//* Prior to reading the Waveform Data registers, the host should
//* disable the Waveform Acquisition Control bit (4.5.10). Failure
//* to disable this control will corrupt the data in the Waveform Data
//* registers. The current status of the waveform data registers can be
//* determined by reading the Waveform Data Ready coil (4.5.27).
//*
//* Read only
//*********************************************************************
int16_t
   MB_Waveform(
      uint16_t 		reg,
      bool 		isWrite,
      int16_t 		val,
      int16_t		*errCode )
{

   int16_t		retVal = 0;
   uint16_t		two_uint8s;
   uint8_t		index;


   if( errCode != NULL )
   {
      *errCode = MBUS_RESPONSE_OK;

      index = reg - WAVEFORM_MB_REG_START;

      // each register is two 8-bit samples
      index = index * 2;

      if( !isWrite )
      {
	 // This is a read
	 two_uint8s = (uint16_t)((SONAR_sample_data[index].sample) & 0xFF << 8);
	 two_uint8s = (uint16_t)two_uint8s | (uint8_t)(SONAR_sample_data[index+1].sample & 0xFF);
	 retVal = two_uint8s;
      }
      else
      {
	 // This is a write to a read-only register
	 *errCode = MBUS_RESPONSE_ILLEGAL_FUNCTION;
      }
   }

   return( retVal );
} // end of MB_Waveform()


//*********************************************************************
// Diagnostic Function Code 0x80
//*********************************************************************
void
   rs485_diagnostics(
      MODBUS_ADU_T 		*msg )
{

   uint8_t		sub_fc;

   if( msg != NULL )
   {
      sub_fc = msg->payload[0];

      switch( sub_fc )
      {
	 case 1:	// Restart Communications
	 case 10:	// Clear Counters and Diagnostic register
	    SSP_status.bus_msg_count = 0;
	    SSP_status.slave_msg_count = 0;

	    if( sub_fc == 10 )
	    {
	       // Build the response message
		rs485_prepare_tx_buf( msg->address, msg->fc );
		rs485_add_tx_byte( sub_fc );
		rs485_transmit_now();	// send the message
		HAL_Delay( 10 );
		NVIC_SystemReset();
	    }
	    break;

	 case 4:	// Activate listen only mode
	    // Not supported
	    break;

	 case 11:	// Return bus message count
	    rs485_prepare_tx_buf( msg->address, msg->fc );
	    rs485_add_tx_byte( sub_fc );
	    rs485_add_tx_byte( (SSP_status.bus_msg_count & 0x0000FF00) >> 8 );
	    rs485_add_tx_byte( (SSP_status.bus_msg_count & 0x000000FF) );
	    break;

	 case 12:	// Return bus CRC error count
	    rs485_prepare_tx_buf( msg->address, msg->fc );
	    rs485_add_tx_byte( sub_fc );
	    rs485_add_tx_byte( (SSP_status.bus_crc_err_count & 0x0000FF00) >> 8 );
	    rs485_add_tx_byte( (SSP_status.bus_crc_err_count & 0x000000FF) );
	    break;

	 case 14:	// return slave message count
	    rs485_prepare_tx_buf( msg->address, msg->fc );
	    rs485_add_tx_byte( sub_fc );
	    rs485_add_tx_byte( (SSP_status.slave_msg_count & 0x0000FF00) >> 8 );
	    rs485_add_tx_byte( (SSP_status.slave_msg_count & 0x000000FF) );
	    break;

	 default:
	    rs485_prepare_tx_buf( msg->address, msg->fc | 0x80 );
	    rs485_add_tx_byte( MBUS_RESPONSE_ILLEGAL_FUNCTION );
	    break;
      } // end of switch()
      rs485_transmit_now();	// send the message
   }

   return;
} // end of rs485_diagnostics()


// end of file modbus.c
