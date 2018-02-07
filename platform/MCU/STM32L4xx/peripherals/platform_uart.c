/**
 ******************************************************************************
 * @file    platform_uart.c
 * @author  William Xu
 * @version V1.0.0
 * @date    05-May-2014
 * @brief   This file provide UART driver functions.
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

OSStatus platform_uart_init( platform_uart_driver_t* driver, const platform_uart_t* peripheral, const platform_uart_config_t* config, ring_buffer_t* optional_ring_buffer )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_uart_deinit( platform_uart_driver_t* driver )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_uart_transmit_bytes( platform_uart_driver_t* driver, const uint8_t* data_out, uint32_t size )
{
   platform_log("unimplemented");
  return kUnsupportedErr;
}

OSStatus platform_uart_receive_bytes( platform_uart_driver_t* driver, uint8_t* data_in, uint32_t expected_data_size, uint32_t timeout_ms )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}

uint32_t platform_uart_get_length_in_buffer( platform_uart_driver_t* driver )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}
