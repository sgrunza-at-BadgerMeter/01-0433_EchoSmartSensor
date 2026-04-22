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


typedef enum CFG_LOAD_TYPE_E
{
   LOAD_ALL_SETTINGS = 0x01,
   LOAD_USER_SETTINGS,
   LOAD_FROM_FLASH
} CFG_LOAD_TYPE_E;


/*
 * @brief Default values used if Flash config is invalid
 *
 */

#define TRACK_FIRST		0
#define TRACK_LAST		1

#define TANK_DEPTH_INCR_FT	12		// Tank depth increment is 1.0"


#define DEFAULT_TANK_DEPTH	10		// Default tank is 10 ft deep
#define DEFAULT_MIN_LEVEL	3		// Default min level is 3 ft
#define DEFAULT_PING_DELAY	5		// 50 ms delay
#define DEFAULT_UPDATE_RATE	10		// number of WF groups in WF average
#define DEFAULT_SMOOTHING	5		// number of cells to run smoothing average
#define DEFAULT_SENSITIVITY	20		// percentage for derivate sensitivity
#define DEFAULT_ALGORITHM	TRACK_FIRST	// algorithm for tracking
#define DEFAULT_CANDIDATES	5		// candidates to track, 5 is max number
#define DEFAULT_WALL_ZONE	(TANK_DEPTH_INCR_FT/2)	// 1/2 Foot from Tank Depth
#define DEFAULT_SETTLING_ZONE	1		// 1 is on, 0 is off
#define DEFAULT_CELL_LIMIT	1
#define DEFAULT_DELTA_Y		10		// Delay Y Coeff - percentage
#define DEFAULT_HISTORY		130
#define DEFAULT_GATE_MIN	(TANK_DEPTH_INCR_FT * 2)	// 2 Foot from track
#define DEFAULT_GATE_MAX	(TANK_DEPTH_INCR_FT * 2)	// 2 Foot from track
#define DEFAULT_SOUND_SPEED	4862		// Speed in Ft per second
#define DEFAULT_GAIN_INCREMENT	1		// Amount of change in auto gain * 10
#define DEFAULT_WALLZONE_AG	40		// Percentage of auto gain in wall zone
#define DEFAULT_AG_SETPOINT	10		// Set point for auto gain
#define DEFAULT_GAIN_BAND	20

#define DEFAULT_WIPER_DELAY	240		// Delay time in Minutes for Wiper Activation


/* Define for Units Field */
#define UNITS_FEET			0
#define UNITS_INCH			1
#define UNITS_METER			2
#define UNITS_CM			3

/* Defines for Echo Loss Action */
#define ECHO_LOSS_ACTION_LOW		0	// Set output at 40mA on loss of signal or during init
#define ECHO_LOSS_ACTION_HIGH		1	// Set output at 20mA on loss of signal or during init
#define ECHO_LOSS_ACTION_CYCLE		2	// Cycle output between 4-20mA on loss of signal or during init

#define I_LOOP_MIN			330
#define I_LOOP_MAX			3800

#define DEFAULT_NAME			"Sensor Name"
#define DEFAULT_NAME_LEN		strlen(DEFAULT_NAME)
#define MAX_NAME_LEN			24

/******* Bit Definitions for bTrackStatus ****/
#define LOOKING_RIGHT		0x01	/* Track is moving right */
#define LOOKING_LEFT		0x02	/* Track is moving left */
#define TRACK_STABLE		0x04	/* Track is stable */
#define NO_REPORT_VALUE		0x20	/* No track value to report out of Init */
#define	LOS_INDICATOR		0x40	/* Loss of Echo for more than Echo Delay Time - to be displayed on ESC and Modbus ONLY */
#define SIGNAL_LOST		0x80	/* Track is lost */

/******* Bit Definitions for bInitStatus *********/
#define INIT_STAB_GAIN		0x01	/* Stablize Gain */
#define INIT_STAB_TRACK		0x02	/* Stablize Track */
#define INIT_STAB_GATES		0x04	/* Adjust Gates */
#define INIT_DONE		0x08	/* Initilization done */
#define INIT_NEED_SETUP		0x80	/* Unit needs set up - on used in reporting not in code */

/******* Bit Definitions for bLoopStatus *******/
#define LEVEL_LOOP_ACTIVE		0x01	/* Excite Voltage on Level Loop */
#define TURBIDITY_LOOP_ACTIVE		0x02	/* Excite Voltage on TUrbidity Loop */
#define LEVEL_DRIVER_ERROR		0x04	/* Drive Voltage to current loop out of range */
#define TURBIDITY_DRIVER_ERROR		0x08	/* Drive Voltage to current loop out of range */
#define ECHO_DELAY_TIMER_STARTED 	0x10	/* Loss of Signal Timer Started */

/******* Bit Definitions for bProbeStatus *********/
#define WAVEFORM_VALID		0x01	/* Waveform data not being updated - smoothed data ready */
#define INIT_MODE		0x02	/* Probe in test mode */
#define WIPER_DRIVER_ERROR	0x04	/* Wiper Motor Drive Failure */
#define WIPER_CONNECT_ERROR	0x08	/* Wiper Motor Drive Not Connected Failure */
#define LOS_ALARM		0x10	/* Loss of Signal for more than Echo Delay */
#define TURB_LED_ERROR		0x20	/* Turbidity LED driver voltage */
#define TURB_DRIVER_ERROR	0x40	/* Driver for Turbidity LED bad */
#define LOW_POWER_M0DE		0x80	/* Sensor in stanby (Low Power) Mode */


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
typedef struct __attribute__((aligned(4))) SSP_CONFIG_T
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

   uint16_t		tankDepth;		///< tank depth in inches or cm
   uint16_t		zeroAdjust;		///< inches or cm
   uint8_t		dwellTime;
   uint16_t		minLevel;		///< inches or cm
   uint16_t		maxLevel;		///< inches or cm
   uint8_t		pingDelay;
   uint8_t		updateRate;
   uint8_t		smoothing;
   uint8_t		deltaSmoothing;
   uint8_t		sensitivity;
   uint8_t		algorithm;
   uint8_t		candidates;
   uint16_t		wallZone;		///< inches or cm
   uint8_t		settlingZone;
   uint8_t		cellLimit;
   uint16_t		gateMin;		///< min distance from left gate to track in basic increments
   uint16_t		gateMax;		///< min distance from right gate to track
   uint8_t		history;
   uint16_t		speedSound;		///< speed of sound in FPS: 1000 - 6000
   uint8_t		gainIncrement;		///< range 5 -50
   uint8_t		wallZoneAG;		///< desired raw candidate signal level if candidate in wall zone
   uint8_t		AGsetPoint;		///< desired raw candidate signal level if candidate not in wall zone
   uint8_t		gainBand;		///< percentage of maximum gain that gain can be adjusted from the Gain Band Mid Point
   uint8_t		units;			///< 0 = ft, 1 = inches, 2 = meters, 3 = centimeters
   uint8_t		wiperDelay;		///< 0 to 240
   uint8_t		setPoint4ma;		///< measured in tank units - based on Measure
   uint8_t		setPoint20mA;
   uint8_t		echoLossAction;		///< 0 = 4mA, 1 = 20mA, 2 = cycle (50% duty cycle)
   uint8_t		echoDelay;
   uint16_t		levelLoopMin;		///< value that provides 4mA loop current (12-bits)
   uint16_t		levelLoopMax;		///< value that provides 20mA loop current (12-bits)
   int16_t		minGain;		///< value for DAC to have minimum gain on signal amp
   int16_t		maxGain;		///< value for DAC to have maximum gain on signal amp
   char			name[ MAX_NAME_LEN ];
   uint16_t		turbLoopMin;		///< value that provides 4mA loop current (12-bits) (AuxLoop)
   uint16_t		turbLoopMax;		///< value that provides 20mA loop current (12-bits) (AuxLoop)

#define MAX_USER_DATA_FIELD1    16              //* 1x16 byte array
#define MAX_USER_DATA_FIELD2    18              //* 1x18 byte array
#define MAX_USER_DATA_FIELD3    96              //* 3x32 byte array
#define MAX_USER_DATA_FIELD4    32 		//* 8x4 byte array

   uint8_t		UserData_Field1[MAX_USER_DATA_FIELD1];	/* User Data - Field 1 */
   uint8_t		UserData_Field2[MAX_USER_DATA_FIELD2];	/* User Data - Field 2 */
   uint8_t		UserData_Field3[MAX_USER_DATA_FIELD3];	/* User Data - Field 3 */
   uint8_t		UserData_Field4[MAX_USER_DATA_FIELD4];	/* User Data - Field 4 */

   int16_t		fixedGainBandMidPoint;	///< fixed value


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
   bool			wiperDriverError;	///< true if wiper has a problem
   bool			wiperConnectionError;	///< true if there is a problem with wiper wiring
   //
   bool			auto_gain;		///< 1 = auto gain on, 0 = auto gain off
   bool			echo_loss;		///< 1 = echo loss, 0 = echo has not been lost
   bool			composite_wf;		///< 1 = use composite waveform, 0 = not composite wf
   bool			fix_gain_band;		///< 1 = use fixed gain band mid point

   uint8_t		bInitStatus;		///< bit-mapped status, see INIT_DONE

   bool			measure_range;		///< 1 = measure range, 0 = measure level
   bool			settling_zone;
   bool			force_level_4ma;	///< 1 = in calibration mode
   bool			force_level_20ma;	///< 1 = in calibration mode
   bool			force_turb_4ma;		///< 1 = in calibration mode
   bool			force_turb_20ma;	///< 1 = in calibration mode
   bool			enable_tx_pulse;	///< 1 = enable the TX pulse
   bool			waveform_valid;		///< 1 = valid waveform
   bool			enable_ping;		///< 1 = ping enabled
   bool			los_alarm;

   uint8_t		bTrackStatus;		///< Bit mapped field, including LOS_INDICATOR, etc
   uint8_t		bLoopStatus;		///< Bit mapped field, including LEVEL_LOOP_ACTIVE, etc
   uint8_t		bProbeStatus;		///< Bit mapped field, including TURB_LED_ERROR

   uint16_t		levelLoop_value;	///< Level 4-20 mA
   uint16_t		auxLoop_value;		///< Aux 4-20 mA

   int32_t		cpu_temperature;	///< sensor CPU temperature in Celcius

   uint32_t		bus_msg_count;		///< number of RS485 messages received
   uint32_t		slave_msg_count;	///< number of RS485 messages received for this device

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
      SSP_CONFIG_T	*cfg,
      CFG_LOAD_TYPE_E	type );

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
