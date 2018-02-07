/**
 ******************************************************************************
 * @file    platform_mcu_powersave.c
 * @author  William Xu
 * @version V1.0.0
 * @date    05-May-2014
 * @brief   This file provide functions called by MICO to drive stm32f2xx
 *          platform: - e.g. power save, reboot, platform initialize
 ******************************************************************************
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 ******************************************************************************
 */


#include "platform_peripheral.h"
#include "platform.h"
#include "platform_logging.h"
#include <string.h> // For memcmp
#include "crt0.h"
#include "platform_init.h"

/******************************************************
*                      Macros
******************************************************/

/******************************************************
*                    Constants
******************************************************/

/******************************************************
*                   Enumerations
******************************************************/

/******************************************************
*                 Type Definitions
******************************************************/

/******************************************************
*                    Structures
******************************************************/

/******************************************************
*               Function Declarations
******************************************************/

/******************************************************
*               Variables Definitions
******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/

OSStatus platform_mcu_powersave_init(void)
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_mcu_powersave_disable( void )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_mcu_powersave_enable( void )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

void platform_idle_hook( void )
{
    __asm("wfi");
}

uint32_t platform_power_down_hook( uint32_t sleep_ms )
{
  return 0;
}