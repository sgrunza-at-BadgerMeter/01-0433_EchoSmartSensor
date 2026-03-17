/**
 **********************************************************************
 * @file config.h
 *
 * @brief This file holds to configuration information for the
 * EchoSmart Sensor
 *
 * @author grunzas
 *
 * @date Oct 14, 2025
 *
 **********************************************************************
 */
#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_


#include "sonar.h"
#include "crc.h"

// Defined in crc.c
extern CRC_HandleTypeDef hcrc;

/**
 * @brief Definition of the types of equipment in the system
 *
 */
typedef enum SSP_CFG_EQIP_E : unsigned char
{
   EQ_TYPE_PROBE = 1,		///< Probe
   EQ_TYPE_PWR_SUPPY = 2,	///< Power Supply / Converter
   EQ_TYPE_LCD_CTRL = 3		///< LCD Controller
} SSP_CFG_EQIP_E;


/**
 * @brief This structure holds the universally unique ID for the
 * microcontroller located on this sensor board.
 */
typedef struct SSP_CONFIG_UUID_T
{
   uint32_t	first;
   uint32_t	second;
   uint32_t	third;
} SSP_CONFIG_UUID_T;

/**
 * @brief This structure holds the non-volatile information about the configuration
 * of this sensor.  It is loaded from Flash during code startup.  It
 * can be changed by writing new values to Flash and then issuing a
 * reset command.
 */
typedef struct SSP_CONFIG_T
{
   uint16_t		serialNumber;		///< Serial number
   uint8_t		hw_id;			///< Hardware ID
   uint8_t		fw_ver;			///< Firmware version
   SSP_CFG_EQIP_E	eq_type;		///< equipment type
   uint8_t		address;		///< This device's address on system bus
   bool			hasWiper;		///< This sensor has a wiper
   bool			hasTurbidity;		///< This sensor has a Turbidity system
   bool			hasReedSwitch;		///< This sensor has a reed switch
   uint8_t		range;			///< The bRange value [0,3] is used as an index to look up information that is range specific
   int16_t		gainAdjust;		///< Gain adjustment [-4095, 4095] applied to RX_GAIN_TABLE
   SSP_CONFIG_UUID_T	uuid;			///< Unique ID number - 96-bits long
   uint16_t		turb_50_ntu_cal;	///< value of ADC for 50 NTU
   uint16_t		turb_0_ntu_cal;		///< value of ADC for 0 NTU
   uint32_t		crc32;			///< Check-value from CRC HW
} SSP_CONFIG_T;

#ifndef SRC_CONFIG_C_
extern
#endif
const char	PROBE_STRING[]
#ifdef SRC_CONFIG_C_
= "Probe"
#endif
;

#ifndef SRC_CONFIG_C_
extern
#endif
const char	PWR_SUPPLY_STRING[]
#ifdef SRC_CONFIG_C_
= "Power Supply"
#endif
;

#ifndef SRC_CONFIG_C_
extern
#endif
const char	LCD_CTRL_STRING[]
#ifdef SRC_CONFIG_C_
= "LCD Controller"
#endif
;

/**
 * @brief this structure holds the current status of the system
 *
 */
typedef struct SSP_STATUS_T
{
   uint16_t		ntu;			///< current turbidity measurement
   uint16_t		previousNTU;		///< last turbidity measurement
   uint8_t		turbidityTimeout;	///< set to HIGH_TURBIDITY_TIMEOUT when wiper started
   uint8_t		turbidityHoldOffTimer;	///< seconds to hold off starting wiper
   bool			wiperActive;		///< true if wiper is on

} SSP_STATUS_T;


/*
 * @brief Magic numbers related to sudden jumps in turbidity
 */
#define TURB_INCREASE_1		1000
#define TURB_INCREASE_2		2500


#ifndef SRC_CONFIG_C_
extern
#endif
SSP_CONFIG_T *	SSP_nvcfg_addr
#ifdef SRC_CONFIG_C_
= (SSP_CONFIG_T *) 0x0803E000
#endif
;


#ifndef SRC_CONFIG_C_
extern
#endif
SSP_CONFIG_T	SSP_configuration
#ifdef SRC_CONFIG_C_
    = { 0 }
#endif
;


#ifndef SRC_CONFIG_C_
extern
#endif
SSP_STATUS_T	SSP_status
#ifdef SRC_CONFIG_C_
    = { 0 }
#endif
;

char *
   cfg_equip_name(
      SSP_CFG_EQIP_E 	t );

void
   cfg_load_default(
      SSP_CONFIG_T	*cfg );

bool
   cfg_verify_nvconfig(
      SSP_CONFIG_T 	*cfg,
      uint32_t 		length );

bool
   cfg_write_nvconfig(
      SSP_CONFIG_T 	*cfg,
      SSP_CONFIG_T 	*addr,
      uint32_t 		length );

#endif /* INC_CONFIG_H_ */

// end of file config.h
