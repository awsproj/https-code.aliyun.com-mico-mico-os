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
#include "mico_rtos.h"
#include "platform_init.h"

#include "portmacro.h"

#ifdef __GNUC__
#include "../../GCC/stdio_newlib.h"
#endif /* ifdef __GNUC__ */

/******************************************************
*                      Macros
******************************************************/

/******************************************************
*                    Constants
******************************************************/

#ifndef STDIO_BUFFER_SIZE
#define STDIO_BUFFER_SIZE 1024
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
extern platform_uart_t platform_uart_peripherals[];
extern platform_uart_driver_t platform_uart_drivers[];

#ifndef MICO_DISABLE_STDIO
static const platform_uart_config_t stdio_uart_config =
    {
        .baud_rate = STDIO_UART_BAUDRATE,
        .data_width = DATA_WIDTH_8BIT,
        .parity = NO_PARITY,
        .stop_bits = STOP_BITS_1,
        .flow_control = FLOW_CONTROL_DISABLED,
        .flags = 0,
};

static volatile ring_buffer_t stdio_rx_buffer;
static volatile uint8_t stdio_rx_data[STDIO_BUFFER_SIZE];
mico_mutex_t stdio_rx_mutex;
mico_mutex_t stdio_tx_mutex;
#endif /* #ifndef MICO_DISABLE_STDIO */

extern void         *_heap_start;
extern void         *_heap_end;
extern void         *_heap_len;

typedef struct HeapRegion
{
    uint8_t *pucStartAddress;
    size_t xSizeInBytes;
} HeapRegion_t;

HeapRegion_t xHeapRegions[] =
{
    {(uint8_t *)&_heap_start, (size_t)&_heap_len},
    {(uint8_t *)0x00900000, 0x40000},
    {NULL, 0}
};
/******************************************************
*               Function Definitions
******************************************************/

void platform_mcu_reset(void)
{
    mico_rtos_enter_critical();
    if (!platform_is_in_interrupt_context())
        MicoWdgInitialize(1);
    for (;;)
        ;
}

/* mico_main is executed after rtos is start up and before real main*/
void mico_main(void)
{
    /* Customized board configuration. */
    init_platform();

#ifndef MICO_DISABLE_STDIO
    if (stdio_tx_mutex == NULL)
        mico_rtos_init_mutex(&stdio_tx_mutex);

    ring_buffer_init((ring_buffer_t *)&stdio_rx_buffer, (uint8_t *)stdio_rx_data, STDIO_BUFFER_SIZE);
    platform_uart_init(&platform_uart_drivers[STDIO_UART], &platform_uart_peripherals[STDIO_UART], &stdio_uart_config, (ring_buffer_t *)&stdio_rx_buffer);
#endif
}

void _main(void)
{
    vPortDefineHeapRegions(xHeapRegions);
    
    driver_init();
    intc_init();

    main();
}

int heap_total_size(void)
{
	return (int)&_heap_len + 0x40000;
}