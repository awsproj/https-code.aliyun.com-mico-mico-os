/**
 ******************************************************************************
 * @file    platform_flash.c
 * @author  William Xu
 * @version V1.0.0
 * @date    05-May-2014
 * @brief   This file provides flash operation functions.
 ******************************************************************************
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "platform_logging.h"
#include "platform.h"
#include "Platform_config.h"
#include "platform_peripheral.h"
#include "stdio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

OSStatus platform_flash_init( const platform_flash_t *peripheral )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_flash_erase( const platform_flash_t *peripheral, uint32_t StartAddress, uint32_t EndAddress  )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_flash_write( const platform_flash_t *peripheral, volatile uint32_t* FlashAddress, uint8_t* Data ,uint32_t DataLength  )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_flash_read( const platform_flash_t *peripheral, volatile uint32_t* FlashAddress, uint8_t* Data ,uint32_t DataLength  )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_flash_enable_protect( const platform_flash_t *peripheral, uint32_t start_address, uint32_t end_address )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_flash_disable_protect( const platform_flash_t *peripheral, uint32_t start_address, uint32_t end_address )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

