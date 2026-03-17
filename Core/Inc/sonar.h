/**
 **********************************************************************
 * @file sonar.h
 *
 * @brief This file holds the definitions related to sonar processing
 *
 * @author grunzas
 *
 * @date Nov 11, 2025
 *
 **********************************************************************
 */
#ifndef INC_SONAR_H_
#define INC_SONAR_H_

// Possible values for bRange
// The bRange value is used as an index to look up information that is range specific
#define RANGE_5_FT	0
#define RANGE_10_FT	1
#define RANGE_20_FT	2
#define RANGE_30_FT	3

#define		SONAR_SAMPLES		1024

typedef struct SONAR_SAMPLE_T
{
   uint32_t	time;
   uint16_t	sample;
} SONAR_SAMPLE_T;


typedef enum SONAR_STATE_E
{
   SONAR_PING_ACTIVE,
   SONAR_PING_LISTEN,
   SONAR_IDLE
} SONAR_STATE_E;

#define SONAR_FREQ_HZ		672500
#define TIM1_BASE_FREQ_KHZ	125000


typedef struct SONAR_STATS_T
{
   uint16_t	max;
   uint16_t	min;
   double	stdev;
   double	average;
} SONAR_STATS_T;


#ifndef SRC_SONAR_C_
extern
#endif
SONAR_SAMPLE_T		SONAR_sample_data[ SONAR_SAMPLES ];

#ifndef SRC_SONAR_C_
extern
#endif
SONAR_STATE_E	SONAR_state
#ifdef SRC_SONAR_C_
= SONAR_IDLE
#endif
;

#ifndef SRC_SONAR_C_
extern
#endif
uint16_t		SONAR_sample_index;

#ifndef SRC_SONAR_C_
extern
#endif
const uint8_t	GAIN_TABLE_INDEX[4]
#ifdef SRC_SONAR_C_
= {32-1,16-1,8-1,4-1}
#endif
;

#ifndef SRC_SONAR_C_
extern
#endif
const uint8_t	GAIN_TABLE_SHIFT_INDEX[4]
#ifdef SRC_SONAR_C_
= {5,4,3,2}
#endif
;

#ifndef SRC_SONAR_C_
extern
#endif
const int16_t	RX_GAIN_TABLE[ SONAR_SAMPLES /16]
#ifdef SRC_SONAR_C_
= {
	0x0000, 0x0000, 0x0000, 0x0010, 0x0010, 0x0020, 0x0020, 0x0020,	/* Entries 0-7 Samples 0-127 */
	0x0040, 0x0040, 0x0040, 0x0040, 0x0080, 0x0080, 0x0080, 0x0080,	/* Entries 8-15 Samples 128-255 */
	0x0100, 0x0100, 0x0100, 0x0100, 0x0200, 0x0200, 0x0200, 0x0200,	/* Entries 16-23 Samples 256-383 */
	0x0300, 0x0300, 0x0300, 0x0300, 0x0400, 0x0400, 0x0400, 0x0400,	/* Entries 24-31 Samples 384-511 */
	0x0500, 0x0500, 0x0500, 0x0500, 0x0600, 0x0600, 0x0600, 0x0600,	/* Entries 32-39 Samples 512-639 */
	0x0700, 0x0700, 0x0700, 0x0700, 0x0800, 0x0800, 0x0800, 0x0800,	/* Entries 40-47 Samples 640-767 */
	0x0900, 0x0900, 0x0900, 0x0900, 0x0a00, 0x0a00, 0x0b00, 0x0b00,	/* Entries 48-55 Samples 768-895 */
	0x0c00, 0x0c00, 0x0d00, 0x0d00, 0x0e00, 0x0e00, 0x0fff, 0x0fff	/* Entries 56-63 Samples 896-1023 */
	}
#endif
;


void	ping( char * );
void	sonar_calc_stats(
	    SONAR_SAMPLE_T 	*data,
	    uint16_t		len,
	    SONAR_STATS_T 	*stats );

#endif /* INC_SONAR_H_ */
