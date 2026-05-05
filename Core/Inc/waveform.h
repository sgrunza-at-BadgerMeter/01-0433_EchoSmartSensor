/**
 **********************************************************************
 * @file waveform.h
 *
 * @brief This file contains definitions and data structures to handle
 * the waveform data
 *
 * @author grunzas
 *
 * @date May 4, 2026
 *
 **********************************************************************
 */
#ifndef INC_WAVEFORM_H_
#define INC_WAVEFORM_H_


// Original comment was /* Sample time in Probe @ 10 ft range */
// Might be related to 200 kHz sampling rate
//
// original value was 0.000005003375771
//
#define BASE_SAMPLE_TIME	(1.0 / 200000.0 )


#ifndef __SRC_WAVEFORM_C__
extern
#endif
float		fRangeResolution[ 4 ];


//*********************************************************************
//* Function prototypes
//*********************************************************************
void buildRangeTable( float *table, uint16_t speedOfsound );
float fCvtToFeet(uint16_t wVal);

#endif /* INC_WAVEFORM_H_ */
