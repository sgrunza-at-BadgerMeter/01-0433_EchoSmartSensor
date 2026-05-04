/**
 **********************************************************************
 * @file ssp_com.h
 *
 * @brief This file contains code for sending SmartSensorProbe commands
 *
 * @author grunzas
 *
 * @date Oct 14, 2025
 *
 **********************************************************************
 */
#ifndef INC_SSP_COM_H_
#define INC_SSP_COM_H_

/* Modified to co-exist with ModBus Function Codes. SSP Protocol Uses Function Codes 65-72, 100-119 */

#define SAM_RESPONSE	0x80		/* Bit b7 set to show data is a response */

/**
 * @brief This enumeration holds the function codes for SSP messages
 *
 * This is the second byte in an RS-485 message from the controller
 */
typedef enum SSP_CMDS_E : unsigned char
{
   CMD_ID_REQ		= 0x41,		// ID Request
   CMD_EXT_ID_REQ	= 0x42,		// Extended ID Request

   CMD_TANK_CFG		= 0x43,		// Defines tank configuration
   CMD_TRACKING_CFG	= 0x44,		// Defines tracking configuration
   CMD_ACOUSTIC_CFG	= 0x45,		// Defines acoustic configuration
   CMD_WIPER_CFG	= 0x46,		// Defines wiper configuration
   CMD_ANALOG_CFG	= 0x47,		// Defines analog (4-20mA) configuration
   CMD_LOOP_CFG		= 0x48,		// Defines loop calibration constants

   CMD_TURB_CFG		= 0x64,		// Defines turbidity configuration

   CMD_TRACK_INFO	= 0x65,		// Read Track info

   CMD_ECHO_DATA	= 0x66,		// Get RAW echo data
   CMD_MANUAL_GAIN	= 0x67,		// Manual Gain setting
   CMD_ECHO_CONTROL	= 0x68,		// Echo control
   CMD_CANDIDATE	= 0x69,		// Get candidate selection
   CMD_PROBE_STATUS	= 0x70,		// Get probe status
   CMD_POWERSUPPLY_STATUS = 0x71,	// Get probe status

   CMD_UNITS		= 0x72,		// Units support

   CMD_FLASH_PARAM	= 0x73,		// Flash parameter control
   CMD_MEMORY_ACCESS	= 0x74,		// Memory access
   CMD_NETWORK_POLL	= 0x75,		// Network poll
   CMD_SET_ADDRESS	= 0x76, 	// Set unit address
   CMD_SET_SN		= 0x77,		// Set SN Command
   CMD_PROGRAM_UPLOAD	= 0x78,		// Upload program

   CMD_COMM_FORMAT	= 0x7D		// Communication format
} SSP_CMDS_E;


void IdRequestCmd( void );

#endif /* INC_SSP_COM_H_ */
