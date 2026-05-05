/**
 **********************************************************************
 * @file waveform.c
 *
 * @brief This file contains routines to handle the waveform data
 *
 * @author grunzas
 *
 * @date May 4, 2026
 *
 **********************************************************************
 */


#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "tx_api.h"
#include "app_threadx.h"
#include "modbus.h"
#include "system_io.h"
#include "usart.h"
#include "fifo.h"
#include "crc.h"
#include "ssp_com.h"
#include "config.h"

#define __SRC_WAVEFORM_C__
#include "waveform.h"


//*********************************************************************
void buildRangeTable( float *table, uint16_t speedOfSound )
{
   float	rangeBySound;

   rangeBySound = fCvtToFeet( speedOfSound ) * 6.0;	// Speed in IPS (divided by 2 for range)

   table[0] = (BASE_SAMPLE_TIME/2) * rangeBySound;
   table[1] = (BASE_SAMPLE_TIME) * rangeBySound;
   table[2] = (BASE_SAMPLE_TIME * 2) * rangeBySound;
   table[3] = (BASE_SAMPLE_TIME * 3) * rangeBySound;

   return;
} // buildRangeTable()


/*********************************************************************
*                                                                    *
*    Function Name: fCvtToFeet()                                     *
*    Change Info:   06/30/08                                         *
*    Description:   Converts value pased based on selected units.    *
*    Parameters:    wVal                                             *
*    Returns:       Float of conversion                              *
*                                                                    *
**********************************************************************/
float fCvtToFeet(uint16_t wVal)
{
   float fVal;

   if( (SSP_configuration.units == UNITS_FEET) || (SSP_configuration.units == UNITS_INCH) )
   {
      fVal = wVal * 1.0;
   }
   else
   {
      // must be metric - wVal is in meters
      fVal = wVal;
      fVal = (fVal * 100.0)/(2.54 * 12.0);
   }
   return (fVal);	// Return conversion
} // end of fCvtToFeet()

// end of file waveform.c
