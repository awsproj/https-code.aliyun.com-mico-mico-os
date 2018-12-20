/**
 ******************************************************************************
 * @file    platform.c
 * @author  William Xu
 * @version V1.0.0
 * @date    05-May-2014
 * @brief   This file provides all MICO Peripherals mapping table and platform
 *          specific functions.
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2014 MXCHIP Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 */

#include "mico.h"

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

const platform_gpio_t platform_gpio_pins[] =
    {
        [MICO_GPIO_0] = {0},
        [MICO_GPIO_1] = {1},
        [MICO_GPIO_2] = {2},
        [MICO_GPIO_3] = {3},
        [MICO_GPIO_4] = {4},
        [MICO_GPIO_5] = {5},
        [MICO_GPIO_6] = {6},
        [MICO_GPIO_7] = {7},
        [MICO_GPIO_8] = {8},
        [MICO_GPIO_9] = {9},
        [MICO_GPIO_10] = {10},
        [MICO_GPIO_11] = {11},
        [MICO_GPIO_12] = {12},
        [MICO_GPIO_13] = {13},
        [MICO_GPIO_14] = {14},
        [MICO_GPIO_15] = {15},
        [MICO_GPIO_16] = {16},
        [MICO_GPIO_17] = {17},
        [MICO_GPIO_18] = {18},
        [MICO_GPIO_19] = {19},
        [MICO_GPIO_20] = {20},
        [MICO_GPIO_21] = {21},
        [MICO_GPIO_22] = {22},
        [MICO_GPIO_23] = {23},
        [MICO_GPIO_24] = {24},
        [MICO_GPIO_25] = {25},
        [MICO_GPIO_26] = {26},
        [MICO_GPIO_27] = {27},
        [MICO_GPIO_28] = {28},
        [MICO_GPIO_29] = {29},
        [MICO_GPIO_30] = {30},
        [MICO_GPIO_31] = {31},
};

const platform_pwm_t platform_pwm_peripherals[] = {};

const platform_i2c_t platform_i2c_peripherals[] = {};
platform_i2c_driver_t platform_i2c_drivers[MICO_I2C_MAX];

const platform_uart_t platform_uart_peripherals[] =
    {
        [MICO_UART_1] = {0},
        [MICO_UART_2] = {1},
};

platform_uart_driver_t platform_uart_drivers[MICO_UART_MAX];

const platform_spi_t platform_spi_peripherals[] = {};
platform_spi_driver_t platform_spi_drivers[MICO_SPI_MAX];

/* Flash memory devices */
const platform_flash_t platform_flash_peripherals[] = {};
platform_flash_driver_t platform_flash_drivers[MICO_FLASH_MAX];

/* Logic partition on flash devices */
const mico_logic_partition_t mico_partitions[] =
    {
        [MICO_PARTITION_BOOTLOADER] =
            {
                .partition_owner = MICO_FLASH_EMBEDDED,
                .partition_description = "Bootloader",
                .partition_start_addr = 0x0,
                .partition_length = 0x10000, //64k bytes
                .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
            },
        [MICO_PARTITION_PARAMETER_1] =
            {
                .partition_owner = MICO_FLASH_EMBEDDED,
                .partition_description = "PARAMETER1",
                .partition_start_addr = 0x10000,
                .partition_length = 0x1000, // 4k bytes
                .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
            },
        [MICO_PARTITION_PARAMETER_2] =
            {
                .partition_owner = MICO_FLASH_EMBEDDED,
                .partition_description = "PARAMETER2",
                .partition_start_addr = 0x11000,
                .partition_length = 0x1000, //4k bytes
                .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
            },
        [MICO_PARTITION_APPLICATION] =
            {
                .partition_owner = MICO_FLASH_EMBEDDED,
                .partition_description = "Application",
                .partition_start_addr = 0x13000,
                .partition_length = 0xED000, //948k bytes
                .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
            },
        [MICO_PARTITION_OTA_TEMP] =
            {
                .partition_owner = MICO_FLASH_EMBEDDED,
                .partition_description = "OTA Storage",
                .partition_start_addr = 0x100000,
                .partition_length = 0xA5E66, //664k bytes
                .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
            },
        [MICO_PARTITION_PARAMETER_3] =
            {
                .partition_owner = MICO_FLASH_EMBEDDED,
                .partition_description = "PARAMETER3",
                .partition_start_addr = 0x12000,
                .partition_length = 0x1000, //4k bytes
                .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
            },
        [MICO_PARTITION_PARAMETER_4] =
            {
                .partition_owner = MICO_FLASH_EMBEDDED,
                .partition_description = "PARAMETER4",
                .partition_start_addr = 0xD000,
                .partition_length = 0x1000, //4k bytes
                .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
            },
        [MICO_PARTITION_SYSTEM_DATA] =
            {
                .partition_owner = MICO_FLASH_EMBEDDED,
                .partition_description = "System data",
                .partition_start_addr = 0xE000,
                .partition_length = 0x2000, //8k bytes
                .partition_options = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
            },
};

const platform_adc_t platform_adc_peripherals[] = {};

/******************************************************
*               Function Definitions
******************************************************/

void init_platform(void)
{
}

void MicoSysLed(bool onoff)
{
}

void MicoRfLed(bool onoff)
{
}

bool MicoShouldEnterMFGMode(void)
{
    return false;
}