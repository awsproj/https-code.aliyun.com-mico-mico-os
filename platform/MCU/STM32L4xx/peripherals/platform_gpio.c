/**
 ******************************************************************************
 * @file    platform_gpio.c
 * @author  William Xu
 * @version V1.0.0
 * @date    05-May-2014
 * @brief   This file provide GPIO driver functions.
 ******************************************************************************
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 ******************************************************************************
 */


#include "platform.h"
#include "platform_peripheral.h"

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
*               Variables Definitions
******************************************************/

/******************************************************
*               Function Declarations
******************************************************/

/******************************************************
*               Function Definitions
******************************************************/

OSStatus platform_gpio_init( const platform_gpio_t* gpio, platform_pin_config_t config )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_gpio_deinit( const platform_gpio_t* gpio )
{ 
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_gpio_output_high( const platform_gpio_t* gpio )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_gpio_output_low( const platform_gpio_t* gpio )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_gpio_output_trigger( const platform_gpio_t* gpio )
{
  platform_log("unimplemented");
  return kUnsupportedErr; 
}

bool platform_gpio_input_get( const platform_gpio_t* gpio )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_gpio_irq_enable( const platform_gpio_t* gpio, platform_gpio_irq_trigger_t trigger, platform_gpio_irq_callback_t handler, void* arg )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_gpio_irq_disable( const platform_gpio_t* gpio )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}
