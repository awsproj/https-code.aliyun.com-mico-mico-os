/**
 ******************************************************************************
 * @file    platform_rng.c
 * @author  William Xu
 * @version V1.0.0
 * @date    05-May-2014
 * @brief   This file provide RNG driver functions.
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
 *                   Macros
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
 *                     Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

OSStatus platform_random_number_read( void *inBuffer, int inByteCount )
{
  platform_log("unimplemented");
  return kUnsupportedErr;
}
