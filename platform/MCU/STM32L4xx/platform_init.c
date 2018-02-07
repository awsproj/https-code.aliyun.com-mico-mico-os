/**
 ******************************************************************************
 * @file    platform_init.c
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
#include "platform_config.h"
#include "platform_logging.h"
#include <string.h> // For memcmp
#include "crt0.h"
#include "mico_platform.h"
#include "platform_init.h"


/******************************************************
*                      Macros
******************************************************/

/******************************************************
*                    Constants
******************************************************/

#ifndef STDIO_BUFFER_SIZE
#define STDIO_BUFFER_SIZE   64
#endif

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
extern platform_uart_t          platform_uart_peripherals[];
extern platform_uart_driver_t   platform_uart_drivers[];
extern const platform_flash_t   platform_flash_peripherals[];
extern platform_flash_driver_t  platform_flash_drivers[];
extern const mico_logic_partition_t     mico_partitions[];


#ifndef MICO_DISABLE_STDIO
static const mico_uart_config_t stdio_uart_config =
{
  .baud_rate    = STDIO_UART_BAUDRATE,
  .data_width   = DATA_WIDTH_8BIT,
  .parity       = NO_PARITY,
  .stop_bits    = STOP_BITS_1,
  .flow_control = FLOW_CONTROL_DISABLED,
  .flags        = 0,
};

static volatile ring_buffer_t stdio_rx_buffer;
static volatile uint8_t             stdio_rx_data[STDIO_BUFFER_SIZE];
mico_mutex_t        stdio_rx_mutex;
mico_mutex_t        stdio_tx_mutex;
#endif /* #ifndef MICO_DISABLE_STDIO */

/******************************************************
*               Function Definitions
******************************************************/
#if defined ( __ICCARM__ )
static inline void __jump_to( uint32_t addr )
{
  __asm( "MOV R1, #0x00000001" );
  __asm( "ORR R0, R0, R1" );  /* Last bit of jump address indicates whether destination is Thumb or ARM code */
  __asm( "BLX R0" );
}


#elif defined ( __GNUC__ )
__attribute__( ( always_inline ) ) static __INLINE void __jump_to( uint32_t addr )
{
  addr |= 0x00000001;  /* Last bit of jump address indicates whether destination is Thumb or ARM code */
  __ASM volatile ("BX %0" : : "r" (addr) );
}


#elif defined ( __CC_ARM )
static void __asm __jump_to( uint32_t addr )
{
  MOV R1, #0x00000001
  ORR R0, R0, R1  /* Last bit of jump address indicates whether destination is Thumb or ARM code */
  BLX R0
}
#endif

void startApplication( uint32_t app_addr )
{
}


void platform_mcu_reset( void )
{
    NVIC_SystemReset();
}

/* STM32F2 common clock initialisation function
* This brings up enough clocks to allow the processor to run quickly while initialising memory.
* Other platform specific clock init can be done in init_platform() or init_architecture()
*/
void init_clocks( void )
{
}

WEAK void init_memory( void )
{
  
}

void init_architecture( void )
{
#ifndef MICO_DISABLE_STDIO
  mico_rtos_init_mutex( &stdio_tx_mutex );
  mico_rtos_unlock_mutex ( &stdio_tx_mutex );
  mico_rtos_init_mutex( &stdio_rx_mutex );
  mico_rtos_unlock_mutex ( &stdio_rx_mutex );

  ring_buffer_init  ( (ring_buffer_t*)&stdio_rx_buffer, (uint8_t*)stdio_rx_data, STDIO_BUFFER_SIZE );
  platform_uart_init( &platform_uart_drivers[STDIO_UART], &platform_uart_peripherals[STDIO_UART], &stdio_uart_config, (ring_buffer_t*)&stdio_rx_buffer );
#endif
}

void mico_main(void)
{
  init_architecture();
}

OSStatus stdio_hardfault( char* data, uint32_t size )
{
}

static char global_cid[25] = { 0 };
const char *mico_generate_cid(void)
{
  uint8_t *chipid = (uint8_t*)(0x7C0000 + 0x7FDA);
  
  sprintf(global_cid, "%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",
          0, 0, 0, 0, 0, 0, 
          chipid[0], chipid[1], chipid[2], chipid[3], chipid[4], chipid[5]);
  return global_cid;
}

/******************************************************
*            NO-OS Functions
******************************************************/


#ifdef NO_MICO_RTOS
static volatile uint32_t no_os_tick = 0;

void SysTick_Handler(void)
{
  no_os_tick ++;
}

uint32_t mico_get_time_no_os(void)
{
  return no_os_tick;
}

void mico_thread_msleep_no_os(uint32_t milliseconds)
{
  int tick_delay_start = mico_get_time_no_os();
  while(mico_get_time_no_os() < tick_delay_start+milliseconds);  
}
#endif
