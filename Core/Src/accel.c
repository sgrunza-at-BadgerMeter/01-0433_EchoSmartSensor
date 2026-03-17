/**
 **********************************************************************
 * @file accel.c
 *
 * @brief This file contains code related to the acceleromter.  The
 * purpose of the accelerometer is to make sure the sensor is pointed
 * down into the tank and is not bouncing around from a stirring type
 * of event
 *
 * @author grunzas
 *
 * @date Dec 29, 2025
 *
 **********************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "main.h"
#include "i2c.h"
#include "accel.h"


static bool		lis3dh_running = false;

/**
 **********************************************************************
 * Acceleromter configuration:
 *
 * Normal Mode
 * 	LPen bit is clear
 * 	HR bit is clear
 * 	10-bit data
 *
 * Full Scale set to +/- 2.0 g for 4 mg/digit
 *
 * Output Data Rate (ODR) set to 10 Hz
 *
 * FIFO configured for Stream mode
 *
 * INT1 configured as: FIFO Full output ( I1_OVERRUN )
 *
 * INT2 not used
 *
 * I2C Read Address:  0x33
 *
 * I2C Write Address: 0x32
 *
 * I2C Speed: 400 kHz
 *
 *
 **********************************************************************
 */


/*
 **********************************************************************
 * @brief accel_check_id() Check the WHO_AM_I register
 *
 * @param pointer to I2C handle
 *
 * @return HAL_OK or HAL_ERROR if a problem occurred
 **********************************************************************
 */
HAL_StatusTypeDef
   accel_check_id(
      I2C_HandleTypeDef 		*hi2c )
{
   HAL_StatusTypeDef		result;
   uint8_t			id;

   result = HAL_I2C_Mem_Read(
	       hi2c,			// handle to I2C device
	       LIS3DH_WRITE_ADDR,	// Device ID on I2C bus
	       LIS3DH_WHO_AM_I,		// Register to read inside LIS3DH
               1, 			// Number of bytes in address
	       &id, 			// pointer to address to put data
	       1,			// number of bytes in WHO_AM_I
	       1000 );			// Timeout Delay
   if( result != HAL_OK )
   {
      printf("HAL_I2C_Mem_Read() failed in %s at %d\r\n", __FILE__, __LINE__ );
      return( result );
   }

   if( id == LIS3DH_ID )
   {
      result = HAL_OK;
      printf( "ID = 0x%x\r\n", id );
   }
   else
   {
      printf( "ID returned as 0x%x instead of expected 0x%x\r\n", id, LIS3DH_ID );
      result = HAL_ERROR;
   }

   return( result );
} // end of accel_check_id()

/*
 **********************************************************************
 * @brief accel_selftest() Check the WHO_AM_I register
 *
 * @param pointer to I2C handle
 *
 * @return HAL_OK or HAL_ERROR if a problem occurred
 **********************************************************************
 */
HAL_StatusTypeDef
   accel_selftest(
      I2C_HandleTypeDef 		*hi2c )
{
   HAL_StatusTypeDef		result;
   uint8_t			ctrlReg4;
   ACCEL_DATA_T			normalMode;
   ACCEL_DATA_T			testMode;
   int16_t			xDiff;
   int16_t			yDiff;
   int16_t			zDiff;


   if( !lis3dh_running )
   {
      accel_init( hi2c );

      HAL_Delay( 500 );		// let LIS3DH stabilize
   }

   // Read ST bit
   result = HAL_I2C_Mem_Read(
	       hi2c,			// handle to I2C device
	       LIS3DH_READ_ADDR,	// Device ID on I2C bus
	       LIS3DH_CTRL_REG4,	// Register to read inside LIS3DH
               1, 			// Number of bytes in address
	       &ctrlReg4,		// pointer to address to put data
	       1,			// number of bytes in WHO_AM_I
	       1000 );			// Timeout Delay
   if( result != HAL_OK )
   {
      printf("HAL_I2C_Mem_Read() failed in %s at %d\r\n", __FILE__, __LINE__ );
      return( result );
   }

   // Modify ST bit to clear if necessary
   if( ctrlReg4 & LIS3DH_CTRL4_ST_MASK )
   {
      // Not currently in Normal mode
      ctrlReg4 &= LIS3DH_CTRL4_ST_MASK;	// clear to normal mode
      ctrlReg4 |= LIS3DH_CTRL4_ST_NORMAL;// set to normal mode
      result = HAL_I2C_Mem_Write(
   	       hi2c,			// handle to I2C device
   	       LIS3DH_READ_ADDR,	// Device ID on I2C bus
   	       LIS3DH_CTRL_REG4,	// Register to read inside LIS3DH
               1, 			// Number of bytes in address
   	       &ctrlReg4,		// pointer to data to write
   	       1,			// number of bytes to write
   	       1000 );			// Timeout delay
      if( result != HAL_OK )
      {
         printf("HAL_I2C_Mem_Write() failed in %s at %d\r\n", __FILE__, __LINE__ );
         return( result );
      }

      HAL_Delay( 1000 );		// Let new mode initialize

   }

   // Read X-Y-Z data
   result = accel_get_xyz( hi2c, &normalMode );
   if( result != HAL_OK )
   {
      printf( "accel_get_xyz() failed in %s at %d\r\n", __FILE__, __LINE__ );
      return( result );
   }

   // Set ST bit
   ctrlReg4 &= LIS3DH_CTRL4_ST_MASK;	// clear to normal mode
   ctrlReg4 |= LIS3DH_CTRL4_ST_MODE0;	// set to mode 0
   result = HAL_I2C_Mem_Write(
	    hi2c,			// handle to I2C device
	    LIS3DH_READ_ADDR,		// Device ID on I2C bus
	    LIS3DH_CTRL_REG4,		// Register to read inside LIS3DH
	    1, 				// Number of bytes in address
	    &ctrlReg4,			// pointer to data to write
	    1,				// number of bytes to write
	    1000 );			// Timeout delay
   if( result != HAL_OK )
   {
      printf("HAL_I2C_Mem_Write() failed in %s at %d\r\n", __FILE__, __LINE__ );
      return( result );
   }

   HAL_Delay( 1000 );			// Let new mode initialize

   // Read X-Y-Z data
   result = accel_get_xyz( hi2c, &testMode );
   if( result != HAL_OK )
   {
      printf( "accel_get_xyz() failed in %s at %d\r\n", __FILE__, __LINE__ );
      return( result );
   }

   // Compare normal and test readings against datasheet
   xDiff = normalMode.x - testMode.x;
   yDiff = normalMode.y - testMode.y;
   zDiff = normalMode.z - testMode.z;

   xDiff = abs( xDiff );
   yDiff = abs( yDiff );
   zDiff = abs( zDiff );

   result = HAL_OK;

   printf( "Self Test limits are between %d and %d\r\n",
	   LIS3DH_MIN_DELTA,
	   LIS3DH_MAX_DELTA );

   if( (xDiff < LIS3DH_MIN_DELTA) || (xDiff > LIS3DH_MAX_DELTA) )
   {
      result = HAL_ERROR;
      printf( "xDiff is %d\r\n", xDiff );
   }

   if( (yDiff < LIS3DH_MIN_DELTA) || (yDiff > LIS3DH_MAX_DELTA) )
   {
      result = HAL_ERROR;
      printf( "yDiff is %d\r\n", yDiff );
   }

   if( (zDiff < LIS3DH_MIN_DELTA) || (zDiff > LIS3DH_MAX_DELTA) )
   {
      result = HAL_ERROR;
      printf( "zDiff is %d\r\n", zDiff );
   }

   // Clear ST bit
   ctrlReg4 &= LIS3DH_CTRL4_ST_MASK;	// clear to normal mode
   ctrlReg4 |= LIS3DH_CTRL4_ST_NORMAL;	// set to normal mode
   result = HAL_I2C_Mem_Write(
	       hi2c,			// handle to I2C device
	       LIS3DH_READ_ADDR,	// Device ID on I2C bus
	       LIS3DH_CTRL_REG4,	// Register to read inside LIS3DH
	       1, 			// Number of bytes in address
	       &ctrlReg4,		// pointer to data to write
	       1,			// number of bytes to write
	       1000 );			// Timeout delay
   if( result != HAL_OK )
   {
      printf("HAL_I2C_Mem_Write() failed in %s at %d\r\n", __FILE__, __LINE__ );
      return( result );
   }

   HAL_Delay( 1000 );		// Let new mode initialize

   if( result == HAL_OK )
   {
      printf( "Self Test passed.\r\n");
   }
   else
   {
      printf( "Self Test failed.\r\n" );
   }

   return( result );
} // end of accel_selftest()

/*
 **********************************************************************
 * @brief accel_init() Initialize the accelerometer
 *
 * @param pointer to I2C handle
 *
 * @return HAL_OK or HAL_ERROR if a problem occurred
 **********************************************************************
 */
HAL_StatusTypeDef
   accel_init(
      I2C_HandleTypeDef 	*hi2c )
{
   HAL_StatusTypeDef		result;
   uint8_t			reg;

   reg = 0x27;	// ODR 10Hz, Normal mode, XYZ enabled

   result = HAL_I2C_Mem_Write(
	       hi2c,			// handle to I2C device
	       LIS3DH_READ_ADDR,	// Device ID on I2C bus
	       LIS3DH_CTRL_REG1,	// Register to read inside LIS3DH
	       1, 			// Number of bytes in address
	       &reg,			// pointer to data to write
	       1,			// number of bytes to write
	       1000 );			// Timeout delay
   if( result != HAL_OK )
   {
      printf("HAL_I2C_Mem_Write() failed in %s at %d\r\n", __FILE__, __LINE__ );
      return( result );
   }

   reg = 0x02;	// L1_overrun set for FIFO overrun interrupt on INT1

   result = HAL_I2C_Mem_Write(
	       hi2c,			// handle to I2C device
	       LIS3DH_READ_ADDR,	// Device ID on I2C bus
	       LIS3DH_CTRL_REG3,	// Register to read inside LIS3DH
	       1, 			// Number of bytes in address
	       &reg,			// pointer to data to write
	       1,			// number of bytes to write
	       1000 );			// Timeout delay
   if( result != HAL_OK )
   {
      printf("HAL_I2C_Mem_Write() failed in %s at %d\r\n", __FILE__, __LINE__ );
      return( result );
   }

   reg = 0x00;	// Not running self test

   result = HAL_I2C_Mem_Write(
	       hi2c,			// handle to I2C device
	       LIS3DH_READ_ADDR,	// Device ID on I2C bus
	       LIS3DH_CTRL_REG4,	// Register to read inside LIS3DH
	       1, 			// Number of bytes in address
	       &reg,			// pointer to data to write
	       1,			// number of bytes to write
	       1000 );			// Timeout delay
   if( result != HAL_OK )
   {
      printf("HAL_I2C_Mem_Write() failed in %s at %d\r\n", __FILE__, __LINE__ );
      return( result );
   }

   reg = 0x40;	// Enable FIFO

   result = HAL_I2C_Mem_Write(
	       hi2c,			// handle to I2C device
	       LIS3DH_READ_ADDR,	// Device ID on I2C bus
	       LIS3DH_CTRL_REG5,	// Register to read inside LIS3DH
	       1, 			// Number of bytes in address
	       &reg,			// pointer to data to write
	       1,			// number of bytes to write
	       1000 );			// Timeout delay
   if( result != HAL_OK )
   {
      printf("HAL_I2C_Mem_Write() failed in %s at %d\r\n", __FILE__, __LINE__ );
      return( result );
   }

   reg = 0xA0;	// Not running self test

   result = HAL_I2C_Mem_Write(
	       hi2c,			// handle to I2C device
	       LIS3DH_READ_ADDR,	// Device ID on I2C bus
	       LIS3DH_FIFO_CTRL_REG,	// Register to read inside LIS3DH
	       1, 			// Number of bytes in address
	       &reg,			// pointer to data to write
	       1,			// number of bytes to write
	       1000 );			// Timeout delay
   if( result != HAL_OK )
   {
      printf("HAL_I2C_Mem_Write() failed in %s at %d\r\n", __FILE__, __LINE__ );
      return( result );
   }

   if( result == HAL_OK )
   {
      lis3dh_running = true;
   }

   return( result );
} // end of accel_init()

/*
 **********************************************************************
 * @brief accel_get_xyz() Initialize the accelerometer
 *
 * With an ODR of 10 Hz the 32 entry FIFO should be filled within
 * 3 seconds.
 *
 * Wait for the FIFO over run bit to be set, then read and average
 * all 32 X, Y, Z entries.
 *
 * @param pointer to I2C handle
 *
 * @return HAL_OK or HAL_ERROR if a problem occurred
 **********************************************************************
 */
HAL_StatusTypeDef
   accel_get_xyz(
      I2C_HandleTypeDef 	*hi2c,
      ACCEL_DATA_T 		*d )
{

   HAL_StatusTypeDef		result;
   uint8_t			reg;
   bool				overrun;
   bool				tooLong;
   uint32_t			timeout;
   uint32_t			currentTime;
   ACCEL_DATA_T			fifo[ ACCEL_FIFO_SIZE_ENTRIES ] = { 0 };
   uint8_t			*data;

   uint8_t			i;
   uint8_t			j;

   int32_t			x;
   int32_t			y;
   int32_t			z;

   if( d == NULL )
   {
      return( HAL_ERROR );
   }

   if( !lis3dh_running )
   {
      accel_init( hi2c );

      HAL_Delay( 500 );		// let LIS3DH stabilize
   }

   timeout = HAL_GetTick();
   timeout += 4000;
   tooLong = false;

   do
   {
      result = HAL_I2C_Mem_Read(
	       hi2c,			// handle to I2C device
	       LIS3DH_READ_ADDR,	// Device ID on I2C bus
	       LIS3DH_FIFO_SRC_REG,	// Register to read inside LIS3DH
	       1, 			// Number of bytes in address
	       &reg, 			// pointer to address to put data
	       1,			// number of bytes to read
	       1000 );			// Timeout Delay
      if( result != HAL_OK )
      {
	 printf("HAL_I2C_Mem_Read() failed in %s at %d\r\n", __FILE__, __LINE__ );
	 return( result );
      }

      currentTime = HAL_GetTick();
      if( currentTime > timeout )
      {
	 tooLong = true;
      }

      overrun = (reg & 0x40) ? true : false;	// OVRN_FIFO bit
   } while( !overrun && !tooLong );

   if( tooLong )
   {
      printf( "Timed out waiting for FIFO Overrun bit in FIFO_SRC_REG to be set\r\n");
      return( HAL_ERROR );
   }

   // Read 32 FIFO entries

   // TODO: Figure out why individual reads are necessary -- should be able to read
   // all bytes in one call to HAL_I2C_Mem_Read()

   data = (uint8_t *)fifo;
   for( j = 0; j < ACCEL_FIFO_SIZE_ENTRIES; j++ )
   {
      // Read all 6 bytes of the entry
      for( i = 0; i < ACCEL_BYTES_PER_ENTRY; i++ )
      {
	 result = HAL_I2C_Mem_Read(
		  hi2c,				// handle to I2C device
		  LIS3DH_WRITE_ADDR,		// Device ID on I2C bus
		  (LIS3DH_OUT_X_L + i),		// Register to read inside LIS3DH
		  I2C_MEMADD_SIZE_8BIT, 	// Number of bytes in address
		  data,				// pointer to address to put data
		  1, 				// number of bytes to read
		  2000 );			// Timeout Delay
	 data++;
      }
   }

   if( result != HAL_OK )
   {
      printf("HAL_I2C_Mem_Read() failed in %s at %d\r\n", __FILE__, __LINE__ );
      return( result );
   }

   x = 0;
   y = 0;
   z = 0;
   for( i = 0; i < ACCEL_FIFO_SIZE_ENTRIES; i++ )
   {
      x += fifo[i].x;
      y += fifo[i].y;
      z += fifo[i].z;
   }

   d->x = x / ACCEL_FIFO_SIZE_ENTRIES;
   d->y = y / ACCEL_FIFO_SIZE_ENTRIES;
   d->z = z / ACCEL_FIFO_SIZE_ENTRIES;

   return( result );
} // end of accel_get_xyz()

/*
 **********************************************************************
 * @brief accel_is_level() Evaluate the given accelerometer data to see
 * if the sensor is level
 *
 * @param pointer to accelerometer data
 *
 * @return true if level, otherwise false
 **********************************************************************
 */
bool
   accel_is_level(
      ACCEL_DATA_T 		*d )
{

   bool		result;
   double	roll;
   double	pitch;

   roll = atan2( d->y, d->z ) * 180.0 / M_PI;
   pitch = atan2( -d->x, sqrt( d->y * d->y + d->z * d->z) ) * 180.0 / M_PI;

   result = true;
   if( abs(roll) > ACCEL_MAX_ROLL )
   {
      printf( " roll magnitude (%d degrees) is greater than limit of %3.2f degrees\r\n",
	       abs(roll), ACCEL_MAX_ROLL );
      result = false;
   }

   if( abs(pitch) > ACCEL_MAX_PITCH )
   {
      printf( "pitch magnitude (%d degrees) is greater than limit of %3.2f degrees\r\n",
	       abs(pitch), ACCEL_MAX_PITCH );
      result = false;
   }

   return( result );
} // accel_is_level()

/*
 **********************************************************************
 * @brief accel_display_data() Display the given accelerometer data
 *
 * @param pointer to accelerometer data
 *
 * @return none
 **********************************************************************
 */
void
   accel_display_data(
      ACCEL_DATA_T 		*d,
      bool			showHeader,
      bool			showRawData )
{

   double	roll;
   double	pitch;

   double	xG;
   double	yG;
   double	zG;

   roll = atan2( d->y, d->z ) * 180.0 / M_PI;
   pitch = atan2( -d->x, sqrt( d->y * d->y + d->z * d->z) ) * 180.0 / M_PI;

   if( showHeader )
   {
      if( showRawData )
      {
	 printf( " X-axis     Y-axis     Z-axis     pitch(degrees)    roll(degrees)\r\n" );
      }
      else
      {
	 printf( " X-axis(mg)     Y-axis(mg)     Z-axis(g)     pitch(degrees)    roll(degrees)\r\n" );
      }
   }

   if( showRawData )
   {
      printf( " %6d        %6d     %6d      %3.2lf            %3.2lf\r\n",
	       d->x, d->y, d->z, pitch, roll );
   }
   else
   {
      xG = (d->x / 16.0f);
      yG = (d->y / 16.0f);
      zG = (d->z / 16.0f) / 1000;
      printf( " %3.2lf          %3.2lf         %3.2lf         %3.2lf              %3.2lf\r\n",
	       xG, yG, zG, pitch, roll );

   }

   return;
} // end of accel_display_data()


// end of file accel.c
