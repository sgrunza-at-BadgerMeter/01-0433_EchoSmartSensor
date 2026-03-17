/**
 **********************************************************************
 * @file wiper.h
 *
 * @brief This file contains Wiper constants, definitions, and
 * structures
 *
 * @author grunzas
 *
 * @date Oct 20, 2025
 *
 **********************************************************************
 */
#ifndef INC_WIPER_H_
#define INC_WIPER_H_


/**
 * @brief Structure to hold various wiper status counts
 */
typedef struct WIPER_ERR_COUNT_T
{
   uint16_t	overCurrent;		///< number of times over current detected
   uint16_t	underVoltage;		///< number of times under voltage detected
   uint16_t	maxRunCount;		///< number of times fail safe timer used to turn off motor
   uint16_t	wiperInitCount;		///< number of times wiper is initiated
} WIPER_ERR_COUNT_T;

/**
 * @brief Number of minutes of signal loss for Wiper Timer Test
 *
 * 	Note: accuracy is +/- 1 minute
 * 	Range 1 - 255
 *
 */
#define SIGNAL_LOSS_TIMEOUT		5

/**
 * @brief Max Number of times to try and recover from Loss of
 * Signal Timeout by running motor
 * 	Range 1 - 255
 */
#define MAX_NBR_OF_SIGNAL_LOSS_TO	4

/**
 * @brief Percentage of the 50 NTU turbidity level to trigger a High
 * Average Turbidity condition
 * 	Range 1 - 100
 */
#define HIGH_AVERAGE_TURBIDITY_PCT	85

/**
 * Number of minutes to try clearing high average turbidity using wiper
 * 	Range 1 - 255
 */
#define HIGH_TURBIDITY_TIMEOUT		30


/**
 * @brief Number of seconds to wait between runs of the Wiper motor to
 * clear a High Average Turbidity reading
 *	Range 1 - 255
 */
#define TURBIDITY_INTERGAP_TIMEOUT	30


#define HIGH_AVERAGE_TURBIDITY_THRESHOLD (50 * HIGH_AVERAGE_TURBIDITY_PCT)

/**** Notes ****
*
* The Wiper Motor Current measurement is divided into 2 timed stages.
* This is to allow a higher limit during the startup phase when in-rush
* current to the motor is expected, but to provide a lower limit during
* the run phase. Phase 1 runs from the time that the motor is energized
* and lasts WIPER_STAGE_1_TIME long (timer is in mS). During this stage
* the max current limit is MAX_WIPER_ON_CURRENT_STAGE_1 (value is in mA).
* Stage 2 starts at the end of stage 1 and lasts until the wiper motor
* is turned off. During this time the current limit is set by
* MAX_WIPER_ON_CURRENT_STAGE_2.
*
*****/

/**
 * @brief Run time for wiper is 2 seconds 20 * 2 (Timer multiplied by 2
 * during run time)
 */
#define WIPER_RUN_TIME			10

/**
 * @brief Number of mS for first stage of current measurement
 */
#define WIPER_STAGE_1_TIME		45

/**
 * @brief Maximum amount of time (in seconds) that a wiper can run
 * in all cases
 */
#define MAX_WIPER_RUN_TIME		25

/**
 * @brief In stage 1 - Maximum Wiper On Current is 2.0Amps
 * (ie 2.000 volts)
 */
#define MAX_WIPER_ON_CURRENT_STAGE_1	2400

/**
 * @brief In stage 2 - Maximum Wiper On Current is 750mAmps
 */
#define MAX_WIPER_ON_CURRENT_STAGE_2	750

/**
 * @brief Minimum Wiper Off Voltage is 3.000 volts
 */
#define MIN_WIPER_OFF_VOLTAGE	2000

/**
 * @brief Maximum number of times to exceed max current before a fault
 * is declared
 */
#define MAX_OVER_I		3

/**
 * @brief Maximum number of times to be under voltage before a fault
 * is declared
 */
#define MAX_UNDER_V		254

#define MAX_WIPER_ERRORS	30

/**
 * @brief Turbidity detector value for high point
 * 	Range 0-4096
 */
#define DETECTOR_HIGH_PT	2000

/**
 * @brief Turbidity detector value for low point
 * 	Range 0-4096
 */
#define DETECTOR_LOW_PT		50

/**
 * @brief Number of samples turbidity detector has high output till we stop motor
 * 	Range 0-255
 */
#define HIGH_OUTPUT_COUNT	20

#ifndef SRC_WIPER_C_
extern
#endif
WIPER_ERR_COUNT_T	wiperStats
#ifdef SRC_WIPER_C_
 = { 0 }
#endif
;


void wiperInitMotor( void );

#endif /* INC_WIPER_H_ */

// end of file wiper.h
