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
static void UART_RxISR_8BIT(UART_HandleTypeDef *huart);
static void UART_RxISR_16BIT(UART_HandleTypeDef *huart);
/******************************************************
*               Function Definitions
******************************************************/

OSStatus platform_uart_init( platform_uart_driver_t* driver, const platform_uart_t* peripheral, const platform_uart_config_t* config, ring_buffer_t* optional_ring_buffer )
{
  OSStatus err = kNoErr;
  GPIO_InitTypeDef GPIO_InitStruct;
  HAL_StatusTypeDef status;

  platform_mcu_powersave_disable();

  require_action_quiet(driver != NULL && peripheral != NULL && config != NULL, exit, err = kParamErr);
  require_action_quiet(optional_ring_buffer->buffer != NULL && optional_ring_buffer->size != 0, exit, err = kParamErr);

  driver->rx_buffer = optional_ring_buffer;
  driver->rx_size = 0;

  if( driver->initialized == false )  
  {
    mico_rtos_init_semaphore( &driver->tx_complete, 1 );
    mico_rtos_init_semaphore( &driver->rx_complete, 1 );
    mico_rtos_init_mutex    ( &driver->tx_mutex );
  }

  driver->huart.Instance = peripheral->Instance;
  driver->huart.Init.BaudRate = config->baud_rate;
  driver->huart.Init.WordLength = config->data_width == DATA_WIDTH_9BIT || (config->data_width == DATA_WIDTH_8BIT && config->parity != NO_PARITY) ? UART_WORDLENGTH_9B : UART_WORDLENGTH_8B;
  driver->huart.Init.StopBits = config->stop_bits == STOP_BITS_1 ? UART_STOPBITS_1 : UART_STOPBITS_2;
  driver->huart.Init.Parity = config->parity == NO_PARITY ? UART_PARITY_NONE : config->parity == EVEN_PARITY ? UART_PARITY_EVEN : UART_PARITY_ODD;
  driver->huart.Init.Mode = UART_MODE_TX_RX;
  driver->huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  driver->huart.Init.OverSampling = UART_OVERSAMPLING_16;
  driver->huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  driver->huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  driver->huart.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  driver->huart.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  status = HAL_UART_Init(&driver->huart);
  require_action_quiet( status == HAL_OK, exit, err = kGeneralErr);

  UART_MASK_COMPUTATION((&driver->huart));
  driver->huart.RxISR = config->data_width == DATA_WIDTH_9BIT && config->parity == NO_PARITY ? UART_RxISR_16BIT : UART_RxISR_8BIT;
  /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
  SET_BIT(driver->huart.Instance->CR3, USART_CR3_EIE);
  /* Enable the UART Parity Error interrupt and Data Register Not Empty interrupt */
  SET_BIT(driver->huart.Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);

  driver->initialized = true;
exit:
  platform_mcu_powersave_enable();
  return err;
}

OSStatus platform_uart_deinit( platform_uart_driver_t* driver )
{
  OSStatus err = kNoErr;
  HAL_StatusTypeDef status;

  platform_mcu_powersave_disable();

  require_action_quiet( driver != NULL, exit, err = kParamErr);
  
  status = HAL_UART_DeInit(&driver->huart);
  require_action_quiet( status == HAL_OK, exit, err = kGeneralErr);

exit:
  platform_mcu_powersave_enable();
  return err;
}

OSStatus platform_uart_transmit_bytes( platform_uart_driver_t* driver, const uint8_t* data_out, uint32_t size )
{
  OSStatus err = kNoErr;
  HAL_StatusTypeDef status;

  platform_mcu_powersave_disable();
  
  mico_rtos_lock_mutex(&driver->tx_mutex);

  require_action_quiet(driver != NULL && data_out != NULL && size != 0, exit, err = kParamErr);

  status = HAL_UART_Transmit_IT(&driver->huart, data_out, size);
  require_action_quiet( status == HAL_OK, exit, err = kGeneralErr);

  /* Wait for transmission complete */
  mico_rtos_get_semaphore( &driver->tx_complete, MICO_NEVER_TIMEOUT );

exit:  
  mico_rtos_unlock_mutex( &driver->tx_mutex );
  platform_mcu_powersave_enable();
  return err;
}

OSStatus platform_uart_receive_bytes( platform_uart_driver_t* driver, uint8_t* data_in, uint32_t expected_data_size, uint32_t timeout_ms )
{
  OSStatus err = kNoErr;
  uint32_t actual_size;

  require_action_quiet( ( driver != NULL ) && ( data_in != NULL ) && ( expected_data_size != 0 ) && expected_data_size <= driver->rx_buffer->size, exit, err = kParamErr);

  mico_rtos_get_semaphore( &driver->rx_complete, 0 );

  /* Set rx_size and wait in rx_complete semaphore until data reaches rx_size or timeout occurs */
  driver->rx_size = expected_data_size;
  
  /* Check if ring buffer already contains the required amount of data. */
  if ( expected_data_size > ring_buffer_used_space( driver->rx_buffer ) )
  {
    err = mico_rtos_get_semaphore( &driver->rx_complete, timeout_ms );
    require_quiet(err == kNoErr, exit);
  }
  
  ring_buffer_read(driver->rx_buffer, data_in, expected_data_size, &actual_size);

exit:
  driver->rx_size = 0;
  return err;
}

uint32_t platform_uart_get_length_in_buffer( platform_uart_driver_t* driver )
{
  return ring_buffer_used_space( driver->rx_buffer );
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  platform_uart_driver_t *driver = (platform_uart_driver_t *)huart;
  mico_rtos_set_semaphore( &driver->tx_complete );
}

static void UART_RxISR_8BIT(UART_HandleTypeDef *huart)
{
  platform_uart_driver_t *driver = (platform_uart_driver_t *)huart;

  uint8_t uhdata = (uint8_t)(READ_REG(huart->Instance->RDR) & huart->Mask);

  if ( ring_buffer_is_full( driver->rx_buffer ) == 0 )
  {
      ring_buffer_write( driver->rx_buffer, &uhdata, 1 );
      if (driver->rx_size > 0 && ring_buffer_used_space( driver->rx_buffer ) >= driver->rx_size)
      {
            mico_rtos_set_semaphore( &driver->rx_complete );
            driver->rx_size = 0;
      }
  }
}

static void UART_RxISR_16BIT(UART_HandleTypeDef *huart)
{
  platform_uart_driver_t *driver = (platform_uart_driver_t *)huart;

  uint16_t uhdata = (uint8_t)(READ_REG(huart->Instance->RDR) & huart->Mask);

  if ( ring_buffer_is_full( driver->rx_buffer ) == 0 )
  {
      ring_buffer_write( driver->rx_buffer, &uhdata, 2 );
      if (driver->rx_size > 0 && ring_buffer_used_space( driver->rx_buffer ) >= driver->rx_size)
      {
            mico_rtos_set_semaphore( &driver->rx_complete );
            driver->rx_size = 0;
      }
  }
}
