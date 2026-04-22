/**
 **********************************************************************
 * @file config.c
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

#include <stdio.h>

#include "main.h"

#define SRC_CONFIG_C_

#include "turbidity.h"
#include "config.h"

#include <string.h>

/*
 **********************************************************************
 *
 * Design intent:
 *   Do not modify the last 8kB of the Flash in the HEX file created
 * by this code.  When the code runs it will check the checksum and
 * reload the NV_CONFIG section with default configuration data.  Once
 * the NV_CONFIG section has valid data, only the code should modify
 * it.
 *
 *   This should allow a re-flashing of the code to not disturb the
 * configuration area.
 *
 **********************************************************************
 */


static uint32_t GetSector(uint32_t Address);
static uint32_t GetBank(uint32_t Address);

/*
 **********************************************************************
 * @brief cfg_load_default() loads a default set of values into the
 * given memory structure
 *
 * @param cfg - pointer to memory large enough to hold the configuration
 * structure
 *
 * @return none
 *
 **********************************************************************
 */
void
   cfg_load_default(
      SSP_CONFIG_T	*cfg,
      CFG_LOAD_TYPE_E	type )
{

   uint8_t		address;

   address = SSP_configuration.address;

   if( type == LOAD_ALL_SETTINGS )
   {
      cfg->address = 1;
      cfg->eq_type = EQ_TYPE_PROBE;
      cfg->fw_ver = 0;
      cfg->hasTurbidity = 0;
      cfg->hasReedSwitch = 0;
      cfg->hasWiper = 0;
      cfg->hw_id = 0;
      cfg->serialNumber = 0;	// 16-bit value
      cfg->range = RANGE_5_FT;
      cfg->gainAdjust = 3;
      cfg->uuid.first = HAL_GetUIDw0();
      cfg->uuid.second = HAL_GetUIDw1();
      cfg->uuid.third = HAL_GetUIDw2();
      cfg->turb_0_ntu_cal = DFT_0_NTU_CAL_VALUE;
      cfg->turb_50_ntu_cal = DFT_50_NTU_CAL_VALUE;

      cfg->tankDepth = DEFAULT_TANK_DEPTH;
      cfg->dwellTime = 0;
      cfg->minLevel = DEFAULT_MIN_LEVEL;
      cfg->maxLevel = (DEFAULT_TANK_DEPTH * 110) / 100;
      cfg->pingDelay = DEFAULT_PING_DELAY;
      cfg->updateRate = DEFAULT_UPDATE_RATE;
      cfg->smoothing = DEFAULT_SMOOTHING;
      cfg->deltaSmoothing = 20;
      cfg->sensitivity = DEFAULT_SENSITIVITY;
      cfg->algorithm = DEFAULT_ALGORITHM;
      cfg->candidates = DEFAULT_CANDIDATES;
      cfg->wallZone = DEFAULT_WALL_ZONE;
      cfg->settlingZone = DEFAULT_SETTLING_ZONE;
      cfg->cellLimit = DEFAULT_CELL_LIMIT;
      cfg->gateMin = DEFAULT_GATE_MIN;
      cfg->gateMax = DEFAULT_GATE_MAX;
      cfg->history = DEFAULT_HISTORY;
      cfg->speedSound = DEFAULT_SOUND_SPEED;
      cfg->gainIncrement = DEFAULT_GAIN_INCREMENT;
      cfg->wallZoneAG = DEFAULT_WALLZONE_AG;
      cfg->AGsetPoint = DEFAULT_AG_SETPOINT;
      cfg->gainBand = DEFAULT_GAIN_BAND;
      cfg->units = UNITS_FEET;
      cfg->wiperDelay = DEFAULT_WIPER_DELAY;
      cfg->setPoint4ma = 0;
      cfg->setPoint20mA = DEFAULT_TANK_DEPTH;
      cfg->echoLossAction = ECHO_LOSS_ACTION_CYCLE;
      cfg->echoDelay = 60;		///< 60 minutes
      cfg->levelLoopMin = I_LOOP_MIN;
      cfg->levelLoopMax = I_LOOP_MAX;
      cfg->minGain = -4095;
      cfg->maxGain = 4095;

      //cfg->name[]
      memcpy( cfg->name, DEFAULT_NAME, DEFAULT_NAME_LEN );

      cfg->turbLoopMin = I_LOOP_MIN;
      cfg->turbLoopMax = I_LOOP_MAX;
      cfg->fixedGainBandMidPoint = 135;	///< Magic number

   }
   else if( type == LOAD_USER_SETTINGS )
   {
      cfg->address = address;	// address not changed
      cfg->tankDepth = DEFAULT_TANK_DEPTH;
      cfg->zeroAdjust = (TANK_DEPTH_INCR_FT / 2);
      cfg->dwellTime = 0;
      cfg->minLevel = DEFAULT_MIN_LEVEL;
      cfg->pingDelay = 25;	// magic number
      cfg->maxLevel = (DEFAULT_TANK_DEPTH * 110)/100;
      cfg->speedSound = DEFAULT_SOUND_SPEED;
      cfg->updateRate = DEFAULT_UPDATE_RATE;
      cfg->smoothing = DEFAULT_SMOOTHING;
      cfg->deltaSmoothing = 20;
      cfg->sensitivity = DEFAULT_SENSITIVITY;
      cfg->algorithm = DEFAULT_ALGORITHM;
      cfg->candidates = DEFAULT_CANDIDATES;
      cfg->wallZone = DEFAULT_WALL_ZONE;
      cfg->settlingZone = DEFAULT_SETTLING_ZONE;
      cfg->cellLimit = DEFAULT_CELL_LIMIT;
      cfg->gateMin = DEFAULT_GATE_MIN;
      cfg->gateMax = DEFAULT_GATE_MAX;
      cfg->history = DEFAULT_HISTORY;
      cfg->gainIncrement = DEFAULT_GAIN_INCREMENT;
      cfg->wallZoneAG = DEFAULT_WALLZONE_AG;
      cfg->AGsetPoint = DEFAULT_AG_SETPOINT;
      cfg->gainBand = DEFAULT_GAIN_BAND;
      cfg->units = UNITS_FEET;
      cfg->wiperDelay = DEFAULT_WIPER_DELAY;
      cfg->setPoint4ma = 0;
      cfg->setPoint20mA = DEFAULT_TANK_DEPTH;
      cfg->echoLossAction = ECHO_LOSS_ACTION_CYCLE;
      cfg->echoDelay = 60;		///< 60 minutes
      memset( cfg->name, 0, MAX_NAME_LEN );
      memcpy( cfg->name, DEFAULT_NAME, DEFAULT_NAME_LEN );
      cfg->fixedGainBandMidPoint = 135;	///< Magic number
   }
   else
   {
      printf( "Internal logic error in file %s at line %d\r\n", __FILE__, __LINE__ );
   }

   return;
} // end of cfg_load_default()

/*
 **********************************************************************
 * @brief cfg_equip_name() returns a pointer to a null terminated
 * string containing the name of the equipment type
 *
 * @param t - type of equipment
 *
 * @return ptr to a string
 *
 **********************************************************************
 */
char *
   cfg_equip_name(
      SSP_CFG_EQIP_E 	t )
{

   char		*c;

   switch( t )
   {
      case EQ_TYPE_PROBE:
	 c = (char *)PROBE_STRING;
	 break;

      case EQ_TYPE_PWR_SUPPY:
	 c = (char *)PWR_SUPPLY_STRING;
	 break;

      case EQ_TYPE_LCD_CTRL:
	 c = (char *)LCD_CTRL_STRING;
	 break;

      default:
	 c = (char *)"NULL\0";
	 break;
   }

   return( c );
} // end of cfg_equip_name()

/*
 **********************************************************************
 * @brief cfg_verify_nvconfig() returns true if the non-volatile
 * structure pointed to by cfg is valid based on the CRC32
 *
 * @cfg	- ptr to configuration data structure to check
 *
 * @length - number of bytes to check
 *
 * @return true if the structure is valid
 *
 * @note The CRC32 is only computed on the data in the structure,
 * not the stored CRC32 at the end.
 *
 **********************************************************************
 */
bool
   cfg_verify_nvconfig(
      SSP_CONFIG_T 		*cfg,
      uint32_t 			length )
{

   bool		cfg_Ok;
   uint32_t	checkLen = 0;
   uint32_t	calc_crc32;

   // Don't include the CRC32 at the end of the structure
   cfg_Ok = false;
   if( length > 4 )
   {
      checkLen = length - sizeof( uint32_t );
      cfg_Ok = true;
   }

   if( cfg == NULL ) // Validate inputs
   {
      cfg_Ok = false;
   }

   if( cfg_Ok )
   {
      // inputs are good, now check the actual structure

      calc_crc32 = HAL_CRC_Calculate(
	       &hcrc,
	       (uint32_t *)cfg,
	       checkLen );

      if( calc_crc32 != cfg->crc32 )
      {
	 // Something is wrong
	 cfg_Ok = false;
      }
   }

   return cfg_Ok;
} // end of cfg_verify_nvconfig()

/*
 **********************************************************************
 * @brief cfg_write_nvconfig() stores the configuration to Flash
 *
 * @cfg	- ptr to configuration data structure to check
 *
 * @addr - ptr to start of Flash that holds the configuration data
 *
 * @length - number of bytes to check
 *
 * @return true if the structure is valid
 *
 * @note The CRC32 is only computed on the data in the structure,
 * not the stored CRC32 at the end.
 *
 **********************************************************************
 */
bool
   cfg_write_nvconfig(
      SSP_CONFIG_T 		*cfg,	// in RAM
      SSP_CONFIG_T		*addr,	// in Flash
      uint32_t 			length )
{
   bool				success;
   uint32_t			calc_crc32;
   uint32_t			len_wo_crc;

   HAL_StatusTypeDef		retVal;
   FLASH_EraseInitTypeDef 	EraseInitStruct;
   uint32_t			SectorError;
   uint32_t			flash_end;
   uint32_t			*flash_end_p;

   uint32_t			firstSector;
   uint32_t			numOfSectors;
   uint32_t			bankNumber;

   uint32_t			writeAddress;
   uint32_t			readRAMaddr;


   if( cfg == NULL )
   {
      success = false;
   }
   else if( addr == NULL )
   {
      success = false;
   }
   else if( length < sizeof( uint32_t ) )
   {
      success = false;
   }
   else
   {
      success = true;
   }

   len_wo_crc = length - sizeof( uint32_t );

   calc_crc32 = HAL_CRC_Calculate(
	       &hcrc,
	       (uint32_t *)cfg,
	       len_wo_crc );

   cfg->crc32 = calc_crc32;

   // Disable instruction cache prior to changing internal cacheable memory update
   if (HAL_ICACHE_Disable() != HAL_OK)
   {
     Error_Handler();
   }

   // Unlock the Flash to enable the flash control register access
   retVal = HAL_FLASH_Unlock();
   if( retVal != HAL_OK )
   {
      printf( "HAL_FLASH_Unlock() failed in %s at %d.\r\n", __FILE__, __LINE__ );
      fflush( stdout );
      Error_Handler();
   }

   /* Get the 1st sector to erase */
   firstSector = GetSector( (uint32_t) addr );

   /* Get the number of sector to erase from 1st sector*/
   flash_end_p = (uint32_t*)((uint8_t *)addr + length);
   flash_end = (uint32_t) flash_end_p;

   numOfSectors = GetSector( (uint32_t)flash_end ) - firstSector + 1;

   /* Get the bank */
   bankNumber = GetBank( (uint32_t)addr );

   /* Fill EraseInit structure*/
   EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
   EraseInitStruct.Banks         = bankNumber;
   EraseInitStruct.Sector        = firstSector;
   EraseInitStruct.NbSectors     = numOfSectors;

   // Erase Flash

   // Make sure the Programming Sequence error bit is not set
   FLASH->NSCCR |= FLASH_CCR_CLR_PGSERR;

   retVal = HAL_FLASHEx_Erase( &EraseInitStruct, &SectorError);
   if( retVal != HAL_OK )
   {
     /*
       Error occurred while sector erase.
       User can add here some code to deal with this error.
       SectorError will contain the faulty sector and then to know the code error on this sector,
       user can call function 'HAL_FLASH_GetError()'
     */
      SectorError = HAL_FLASH_GetError();
      printf( "HAL_FLASHEx_Erase() failed with error %ld(0x%lx)\r\n", SectorError, SectorError );

      Error_Handler();
   }

   // Write to Flash

   writeAddress = (uint32_t)addr;
   readRAMaddr = (uint32_t)cfg;

   while( writeAddress < flash_end )
   {

      retVal = HAL_FLASH_Program(
		     FLASH_TYPEPROGRAM_QUADWORD,	// 128-bit write
		     writeAddress,			// address in Flash
		     readRAMaddr );			// address in RAM
      if( retVal == HAL_OK )
      {
	 writeAddress = writeAddress + 16;
	 readRAMaddr = readRAMaddr + 16;
      }
      else
      {
	 // Flash write failure
	 Error_Handler();
      }
   }

   // Lock Flash control registers
   HAL_FLASH_Lock();

   // Enable instruction cache prior to internal cacheable memory update
   if (HAL_ICACHE_Enable() != HAL_OK)
   {
     Error_Handler();
   }

   success = cfg_verify_nvconfig(
		  addr,
		  sizeof( SSP_CONFIG_T ) );

   return( success );
} // end of cfg_write_nvconfig()


/**
  * @brief  Gets the sector of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;

  if((Address >= FLASH_BASE) && (Address < FLASH_BASE + FLASH_BANK_SIZE))
  {
    sector = (Address & ~FLASH_BASE) / FLASH_SECTOR_SIZE;
  }
  else if ((Address >= FLASH_BASE + FLASH_BANK_SIZE) && (Address < FLASH_BASE + FLASH_SIZE))
  {
    sector = ((Address & ~FLASH_BASE) - FLASH_BANK_SIZE) / FLASH_SECTOR_SIZE;
  }
  else
  {
    sector = 0xFFFFFFFF; /* Address out of range */
  }

  return sector;
}


/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
  uint32_t bank = 0;

  if (READ_BIT(FLASH->OPTSR_CUR, FLASH_OPTSR_SWAP_BANK) == 0)
  {
    /* No Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_1;
    }
    else
    {
      bank = FLASH_BANK_2;
    }
  }
  else
  {
    /* Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_2;
    }
    else
    {
      bank = FLASH_BANK_1;
    }
  }

  return bank;
}

// end of file config.c
