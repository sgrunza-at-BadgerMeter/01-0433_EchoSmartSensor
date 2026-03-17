/**
 **********************************************************************
 * @file wiper.c
 *
 * @brief This file contains wiper related routines
 *
 * @author grunzas
 *
 * @date Oct 20, 2025
 *
 **********************************************************************
 */

#include <stdio.h>
#include "main.h"
#include "config.h"
#include "gpio.h"

#define SRC_WIPER_C_

#include "wiper.h"


/*
 **********************************************************************
 * @brief wiperInitMotor() initialize motor and status information
 *
 * @param none
 *
 * @return none
 *
 **********************************************************************
 */
void
   wiperInitMotor( void )
{

   // Ensure motor is currently off
   GPIO_wiper_off();

   // TODO: Finish wiper logic

   return;
} // end of wiperInitMotor()

// end of wiper.c
