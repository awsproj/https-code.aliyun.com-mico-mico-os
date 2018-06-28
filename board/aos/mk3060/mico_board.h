/**
 ******************************************************************************
 * @file    platform.h
 * @author  William Xu
 * @version V1.0.0
 * @date    05-May-2014
 * @brief   This file provides all MICO Peripherals defined for current platform.
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


#ifndef __MICO_BOARD_H__
#define __MICO_BOARD_H__

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
  

   
/******************************************************
 *                   Enumerations
 ******************************************************/

enum
{
    MICO_GPIO_4,
    MICO_GPIO_22,
    MICO_GPIO_23,
    MICO_GPIO_20,
    MICO_GPIO_21,
    MICO_GPIO_31,
    MICO_GPIO_0,
    MICO_GPIO_1,
    MICO_GPIO_15,
    MICO_GPIO_17,
    MICO_GPIO_16,
    MICO_GPIO_14,
    MICO_GPIO_30,
    MICO_GPIO_29,
    MICO_GPIO_MAX, /* Denotes the total number of GPIO port aliases. Not a valid GPIO alias */
    MICO_GPIO_NONE,
    MICO_SYS_LED = MICO_GPIO_4,
    MICO_RF_LED = MICO_GPIO_NONE, 
    BOOT_SEL = MICO_GPIO_NONE, 
    MFG_SEL = MICO_GPIO_NONE, 
    EasyLink_BUTTON = MICO_GPIO_29,
    STDIO_UART_RX = MICO_GPIO_NONE,  
    STDIO_UART_TX = MICO_GPIO_NONE,  
};

enum
{
  MICO_SPI_1,
  MICO_SPI_MAX, /* Denotes the total number of SPI port aliases. Not a valid SPI alias */
  MICO_SPI_NONE,
};

enum
{
    MICO_IIS_MAX, /* Denotes the total number of IIS port aliases. Not a valid IIS alias */
    MICO_IIS_NONE,
};

enum
{
    MICO_I2C_1,
    MICO_I2C_2,
    MICO_I2C_MAX, /* Denotes the total number of I2C port aliases. Not a valid I2C alias */
    MICO_I2C_NONE,
};

enum
{
    MICO_PWM_MAX, /* Denotes the total number of PWM port aliases. Not a valid PWM alias */
    MICO_PWM_NONE,
};

enum
{
    MICO_ADC_1,
    MICO_ADC_2,
    MICO_ADC_MAX, /* Denotes the total number of ADC port aliases. Not a valid ADC alias */
    MICO_ADC_NONE,
};

enum
{
	MICO_UART_1, /* UART1 */
    MICO_UART_2, /* UART2 */
    MICO_UART_MAX, /* Denotes the total number of UART port aliases. Not a valid UART alias */
    MICO_UART_NONE,
};

enum
{
  MICO_FLASH_EMBEDDED,
  MICO_FLASH_MAX,
  MICO_FLASH_NONE,
};

/* Make sure same to hal_partition_t defined in AliOS flash.h*/
enum {
    MICO_PARTITION_ERROR = -1,
    MICO_PARTITION_BOOTLOADER,
    MICO_PARTITION_APPLICATION,
    MICO_PARTITION_ATE,
    MICO_PARTITION_OTA_TEMP,
    MICO_PARTITION_RF_FIRMWARE,
    MICO_PARTITION_PARAMETER_1,
    MICO_PARTITION_PARAMETER_2,
    MICO_PARTITION_PARAMETER_3,
    MICO_PARTITION_PARAMETER_4,
    MICO_PARTITION_LINK_KEY,
    MICO_PARTITION_BT_FIRMWARE,
    MICO_PARTITION_SPIFFS,
    MICO_PARTITION_MAX,
    MICO_PARTITION_NONE,
};

#ifdef BOOTLOADER
#define MICO_STDIO_UART             (MICO_UART_1)
#define MICO_STDIO_UART_BAUDRATE    (921600)
#else
#define MICO_STDIO_UART             (MICO_UART_1)
#define MICO_STDIO_UART_BAUDRATE    (921600)
#endif

#define MICO_UART_FOR_APP           (MICO_UART_2)
#define MICO_MFG_TEST               (MICO_UART_2)
#define MICO_CLI_UART               (MICO_UART_1)

/* Components connected to external I/Os*/
#define USE_MICO_SPI_FLASH

/* Arduino extention connector */
#define Arduino_RXD         (MICO_GPIO_1)
#define Arduino_TXD         (MICO_GPIO_0)
#define Arduino_D2          (MICO_GPIO_NONE)
#define Arduino_D3          (MICO_GPIO_NONE)
#define Arduino_D4          (MICO_GPIO_NONE)
#define Arduino_D5          (MICO_GPIO_4)
#define Arduino_D6          (MICO_GPIO_NONE)
#define Arduino_D7          (MICO_GPIO_NONE)

#define Arduino_D8          (MICO_GPIO_NONE)
#define Arduino_D9          (MICO_GPIO_NONE)
#define Arduino_CS          (MICO_GPIO_15)
#define Arduino_SI          (MICO_GPIO_16)
#define Arduino_SO          (MICO_GPIO_17)
#define Arduino_SCK         (MICO_GPIO_14)
#define Arduino_SDA         (MICO_GPIO_21)
#define Arduino_SCL         (MICO_GPIO_20)

#define Arduino_A0          (MICO_ADC_NONE)
#define Arduino_A1          (MICO_ADC_NONE)
#define Arduino_A2          (MICO_ADC_NONE)
#define Arduino_A3          (MICO_ADC_NONE)
#define Arduino_A4          (MICO_ADC_NONE)
#define Arduino_A5          (MICO_ADC_NONE)

#define Arduino_I2C         (MICO_I2C_1)
#define Arduino_SPI         (MICO_SPI_1)
#define Arduino_UART        (MICO_UART_2)

#define MICO_I2C_CP         (MICO_I2C_NONE)


void mico_board_init(void);

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif //__MICO_BOARD_H__
