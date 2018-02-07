NAME := stm32l431rc

$(NAME)_SOURCES =  \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c \
Src/stm32l4xx_it.c \
Src/system_stm32l4xx.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c \
Src/main.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c \
Src/stm32l4xx_hal_msp.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c \
Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c \
startup_stm32l431xx.s

GLOBAL_INCLUDES += \
Inc \
Drivers/STM32L4xx_HAL_Driver/Inc \
Drivers/STM32L4xx_HAL_Driver/Inc/Legacy \
Drivers/CMSIS/Device/ST/STM32L4xx/Include \
Drivers/CMSIS/Include

GLOBAL_DEFINES += \
USE_HAL_DRIVER \
STM32L431xx