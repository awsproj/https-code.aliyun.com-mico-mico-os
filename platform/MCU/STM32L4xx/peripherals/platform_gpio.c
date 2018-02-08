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
static platform_gpio_irq_callback_t callbacks[15];
static void *args[15];
/******************************************************
*               Function Declarations
******************************************************/

/******************************************************
*               Function Definitions
******************************************************/

OSStatus platform_gpio_init( const platform_gpio_t* gpio, platform_pin_config_t config )
{
  OSStatus err = kNoErr;
  uint32_t mode, pull;
  platform_gpio_t* gpiow;

  platform_mcu_powersave_disable();
  require_action_quiet( gpio != NULL, exit, err = kParamErr);

  gpiow = (platform_gpio_t *)gpio;

  switch(config)
  {
  case INPUT_PULL_UP:
  mode = GPIO_MODE_INPUT;
  pull = GPIO_PULLUP;
  break;  

  case INPUT_PULL_DOWN:
  mode = GPIO_MODE_INPUT;
  pull = GPIO_PULLDOWN;
  break;

  case INPUT_HIGH_IMPEDANCE:
  mode = GPIO_MODE_INPUT;
  pull = GPIO_NOPULL;
  break;

  case OUTPUT_PUSH_PULL:
  mode = GPIO_MODE_OUTPUT_PP;
  break;

  case OUTPUT_OPEN_DRAIN_NO_PULL:
  mode = GPIO_MODE_OUTPUT_OD;
  pull = GPIO_NOPULL;
  break;

  case OUTPUT_OPEN_DRAIN_PULL_UP:
  mode = GPIO_MODE_OUTPUT_OD;
  pull = GPIO_PULLUP;
  break;

  default:
  break;
  }

  /*Configure GPIO pin : PC13 */
  gpiow->GPIO_Pin.Mode = mode;
  gpiow->GPIO_Pin.Pull = pull;
  HAL_GPIO_Init(gpiow->GPIOx, &gpiow->GPIO_Pin);

exit:
  platform_mcu_powersave_enable();
  return err;
}

OSStatus platform_gpio_deinit( const platform_gpio_t* gpio )
{
  OSStatus err = kNoErr;

  platform_mcu_powersave_disable();
  require_action_quiet( gpio != NULL, exit, err = kParamErr);

  HAL_GPIO_DeInit(gpio->GPIOx, gpio->GPIO_Pin.Pin);
  platform_gpio_irq_disable(gpio);

exit:
  platform_mcu_powersave_enable();
  return err;
}

OSStatus platform_gpio_output_high( const platform_gpio_t* gpio )
{
  OSStatus err = kNoErr;

  platform_mcu_powersave_disable();

  require_action_quiet( gpio != NULL, exit, err = kParamErr);

  HAL_GPIO_WritePin(gpio->GPIOx, gpio->GPIO_Pin.Pin, GPIO_PIN_SET);
  
exit:
  platform_mcu_powersave_enable();
  return err;
}

OSStatus platform_gpio_output_low( const platform_gpio_t* gpio )
{
  OSStatus err = kNoErr;

  platform_mcu_powersave_disable();

  require_action_quiet( gpio != NULL, exit, err = kParamErr);

  HAL_GPIO_WritePin(gpio->GPIOx, gpio->GPIO_Pin.Pin, GPIO_PIN_RESET);
  
exit:
  platform_mcu_powersave_enable();
  return err;
}

OSStatus platform_gpio_output_trigger( const platform_gpio_t* gpio )
{
  OSStatus err = kNoErr;

  platform_mcu_powersave_disable();

  require_action_quiet( gpio != NULL, exit, err = kParamErr);

  HAL_GPIO_TogglePin(gpio->GPIOx, gpio->GPIO_Pin.Pin);
  
exit:
  platform_mcu_powersave_enable();
  return err;
}

bool platform_gpio_input_get( const platform_gpio_t* gpio )
{
  return HAL_GPIO_ReadPin(gpio->GPIOx, gpio->GPIO_Pin.Pin);
}

OSStatus platform_gpio_irq_enable( const platform_gpio_t* gpio, platform_gpio_irq_trigger_t trigger, platform_gpio_irq_callback_t handler, void* arg )
{
  OSStatus err = kNoErr;
  uint32_t i, pin;
  GPIO_InitTypeDef GPIO_InitStruct;

  platform_mcu_powersave_disable();
  require_action_quiet( gpio != NULL, exit, err = kParamErr);

  for(i = 0, pin = gpio->GPIO_Pin.Pin; pin > 0; pin = pin >> 1, i++);
  i--;

  callbacks[i] = handler;
  args[i] = arg;

  GPIO_InitStruct = gpio->GPIO_Pin;
  GPIO_InitStruct.Mode = trigger == IRQ_TRIGGER_RISING_EDGE ? GPIO_MODE_IT_RISING : trigger == IRQ_TRIGGER_FALLING_EDGE ? GPIO_MODE_IT_FALLING : IRQ_TRIGGER_BOTH_EDGES;
  HAL_GPIO_Init(gpio->GPIOx, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

exit:
  platform_mcu_powersave_enable();
  return err;
}

OSStatus platform_gpio_irq_disable( const platform_gpio_t* gpio )
{
  OSStatus err = kNoErr;

  platform_mcu_powersave_disable();
  require_action_quiet( gpio != NULL, exit, err = kParamErr);

  /* EXTI interrupt init*/
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
  HAL_GPIO_Init(gpio->GPIOx, &gpio->GPIO_Pin);

exit:
  platform_mcu_powersave_enable();
  return err;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint32_t i, pin;

  for(i = 0, pin = GPIO_Pin; pin > 0; pin = pin >> 1, i++);
  i--;

  callbacks[i](args[i]);
}