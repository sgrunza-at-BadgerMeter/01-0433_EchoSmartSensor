/**
 **********************************************************************
 * @file turbidity.h
 *
 * @brief Definitions and structure for the turbidity measurements
 *
 * @author grunzas
 *
 * @date Dec 23, 2025
 *
 **********************************************************************
 */
#ifndef INC_TURBIDITY_H_
#define INC_TURBIDITY_H_


#define TURB_LED_CYCLE_TIME	8		/* LED cycle time is 80mS - 50% duty */

#define MAX_TLED_ON_VOLTAGE	350	/* Maximum voltage (0.350 Volts) when LED is ON before fault */
#define MIN_TLED_OFF_VOLTAGE	3800	/* Minimum voltage (3.800 Volts) when LED is OFF before fault */

#define DFT_0_NTU_CAL_VALUE	0		/* Default Value from A/D to indicate 0 NTU */
#define DFT_50_NTU_CAL_VALUE	0x0fff	/* Default Value from A/D to indicate 50 NTU */

#define TURBIDITY_NUM_SAMPLES	90		/* Number of samples used in iAvgTurb Value (range 1-254)*/


typedef struct TURB_DATA_T
{
   int32_t		turbAvg;	// average turbidity
   int32_t		turbPrev;	// last reading of turbidity
   int32_t		turbSum;	// summation of turbidity readings
   double		ntu;
} TURB_DATA_T;

void	displayTurbidity( TURB_DATA_T *t );

TURB_DATA_T	*getTurbidityPtr( void );

void
   buildAverageTurbidity(
      TURB_DATA_T		*t,
      uint16_t			value );

void turbidityInit( void );
void UpdateTurbidity( void );

#endif /* INC_TURBIDITY_H_ */

// end of file tubidity.h
