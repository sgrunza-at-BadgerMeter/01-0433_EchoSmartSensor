/**
 **********************************************************************
 * @file accel.h
 *
 * @brief This file contains the data structures related to the
 * acceleromter.  The purpose of the accelerometer is to make sure the
 * sensor is pointed down into the tank and is not bouncing around
 * from a stirring type of event
 *
 * @author grunzas
 *
 * @date Dec 29, 2025
 *
 **********************************************************************
 */
#ifndef INC_ACCEL_H_
#define INC_ACCEL_H_

#include <stdbool.h>


typedef struct __attribute__((packed)) ACCEL_DATA_T
{
   int16_t	x;	///<	X axis acceleration
   int16_t	y;	///<	Y axis acceleration
   int16_t	z;	///<	Z axis acceleration
} ACCEL_DATA_T;

#endif /* INC_ACCEL_H_ */

// Size of a read to capture all 32 FIFO entries of three two-byte entries
#define ACCEL_FIFO_SIZE_ENTRIES		32
#define ACCEL_BYTES_PER_ENTRY		6
#define ACCEL_FIFO_SIZE_BYTES	( ACCEL_FIFO_SIZE_ENTRIES * ACCEL_BYTES_PER_ENTRY )


#define ACCEL_MAX_G		2
#define ACCEL_MAX_COUNT 	0x7FFF

/**
 * @defgroup LIS3DH Definitions for the LIS3DH Accelerometer
 * @brief Relevant registers, bits, and masks for the LIS3DH
 *
 * Definitions for the LIS3DH accelerometer for this specific
 * project are in this section.  Concerns for SPI or other
 * endian processors are not addressed.  This is a narrowly
 * targeted implementation for size and speed constraints.
 *
 * @{
 *
 */

#define LIS3DH_READ_ADDR	0x33
#define LIS3DH_WRITE_ADDR	0x32

/** Device Identification (Who am I) **/
#define LIS3DH_ID    		0x33U

// Registers
#define LIS3DH_STATUS_REG_AUX  	0x07U
#define LIS3DH_OUT_ADC1_L      	0x08U
#define LIS3DH_OUT_ADC1_H      	0x09U
#define LIS3DH_OUT_ADC2_L      	0x0AU
#define LIS3DH_OUT_ADC2_H      	0x0BU
#define LIS3DH_OUT_ADC3_L      	0x0CU
#define LIS3DH_OUT_ADC3_H      	0x0DU
#define LIS3DH_WHO_AM_I        	0x0FU
#define LIS3DH_CTRL_REG0       	0x1EU
#define LIS3DH_TEMP_CFG_REG    	0x1FU
#define LIS3DH_CTRL_REG1       	0x20U
#define LIS3DH_CTRL_REG2       	0x21U
#define LIS3DH_CTRL_REG3       	0x22U
#define LIS3DH_CTRL_REG4       	0x23U
#define LIS3DH_CTRL_REG5       	0x24U
#define LIS3DH_CTRL_REG6      	0x25U
#define LIS3DH_REFERENCE      	0x26U
#define LIS3DH_STATUS_REG     	0x27U
#define LIS3DH_OUT_X_L        	0x28U
#define LIS3DH_OUT_X_H        	0x29U
#define LIS3DH_OUT_Y_L        	0x2AU
#define LIS3DH_OUT_Y_H        	0x2BU
#define LIS3DH_OUT_Z_L        	0x2CU
#define LIS3DH_OUT_Z_H        	0x2DU
#define LIS3DH_FIFO_CTRL_REG  	0x2EU
#define LIS3DH_FIFO_SRC_REG   	0x2FU
#define LIS3DH_INT1_CFG       	0x30U
#define LIS3DH_INT1_SRC       	0x31U
#define LIS3DH_INT1_THS   	0x32U
#define LIS3DH_INT1_DURATION    0x33U
#define LIS3DH_INT2_CFG   	0x34U
#define LIS3DH_INT2_SRC   	0x35U
#define LIS3DH_INT2_THS   	0x36U
#define LIS3DH_INT2_DURATION    0x37U
#define LIS3DH_CLICK_CFG  	0x38U
#define LIS3DH_CLICK_SRC  	0x39U
#define LIS3DH_CLICK_THS  	0x3AU
#define LIS3DH_TIME_LIMIT 	0x3BU
#define LIS3DH_TIME_LATENCY     0x3CU
#define LIS3DH_TIME_WINDOW	0x3DU
#define LIS3DH_ACT_THS    	0x3EU
#define LIS3DH_ACT_DUR		0x3FU


// CTRL_REG4 (0x23) values
#define LIS3DH_CTRL4_ST_MASK	0x06
#define LIS3DH_CTRL4_ST_MODE0	0x02
#define LIS3DH_CTRL4_ST_MODE1	0x04
#define LIS3DH_CTRL4_ST_NORMAL	0x00

// Self Test result limits (Table 4)
#define LIS3DH_MIN_DELTA	17
#define LIS3DH_MAX_DELTA	360

typedef enum LIS3DH_ODR_E
{
  LIS3DH_POWER_DOWN                      = 0x00,
  LIS3DH_ODR_1Hz                         = 0x01,
  LIS3DH_ODR_10Hz                        = 0x02,
  LIS3DH_ODR_25Hz                        = 0x03,
  LIS3DH_ODR_50Hz                        = 0x04,
  LIS3DH_ODR_100Hz                       = 0x05,
  LIS3DH_ODR_200Hz                       = 0x06,
  LIS3DH_ODR_400Hz                       = 0x07,
  LIS3DH_ODR_1kHz620_LP                  = 0x08,
  LIS3DH_ODR_5kHz376_LP_1kHz344_NM_HP    = 0x09,
} LIS3DH_ODR_E;


/** @} */	// end of LIS3DH definitions


// Define what level means in terms of degrees from pointing
// straight down to the center of the Earth
#define ACCEL_MAX_PITCH		20.0f
#define ACCEL_MAX_ROLL		20.0f

HAL_StatusTypeDef	accel_init( I2C_HandleTypeDef *hi2c );
HAL_StatusTypeDef	accel_check_id( I2C_HandleTypeDef *hi2c );
HAL_StatusTypeDef	accel_selftest( I2C_HandleTypeDef *hi2c );
HAL_StatusTypeDef	accel_get_xyz( I2C_HandleTypeDef *hi2c, ACCEL_DATA_T *d );
bool			accel_is_level( ACCEL_DATA_T *d );
void			accel_display_data( ACCEL_DATA_T *d, bool showHeader, bool units );

// end of file accel.h
