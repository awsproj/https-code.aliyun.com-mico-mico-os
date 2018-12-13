#
#  UNPUBLISHED PROPRIETARY SOURCE CODE
#  Copyright (c) 2016 MXCHIP Inc.
#
#  The contents of this file may not be disclosed to third parties, copied or
#  duplicated in any form, in whole or in part, without the prior written
#  permission of MXCHIP Corporation.
#


NAME = MOC109

# Host architecture is ARM968E-S
HOST_ARCH := ARM968E-S

# Host MCU alias for OpenOCD
HOST_OPENOCD := moc109

GLOBAL_INCLUDES += \
. \
../.. \
../../include \
../../$(TOOLCHAIN_NAME) \
../../$(HOST_ARCH)

# Global flags
GLOBAL_CFLAGS   += $$(CPU_CFLAGS) -fno-builtin-printf -Wno-implicit-function-declaration -Wno-int-conversion -Wno-unused-variable -Wno-unused-function
GLOBAL_CXXFLAGS += $$(CPU_CXXFLAGS)
GLOBAL_ASMFLAGS += $$(CPU_ASMFLAGS)
GLOBAL_LDFLAGS  += $$(CPU_LDFLAGS)

GLOBAL_DEFINES += CONFIG_MX108
GLOBAL_DEFINES += configUSE_WATCHDOG_TICK=32000
GLOBAL_DEFINES += configTOTAL_HEAP_SIZE=102400
GLOBAL_DEFINES += configTIMER_TASK_STACK_DEPTH=750
GLOBAL_DEFINES += CONFIG_USE_LINKER_HEAP=1

ifeq ($(filter $(RTOS),NoRTOS),)
GLOBAL_LDFLAGS += -Wl,-wrap,_malloc_r -Wl,-wrap,free -Wl,-wrap,realloc -Wl,-wrap,malloc -Wl,-wrap,calloc -Wl,-wrap,_free_r -Wl,-wrap,_realloc_r -Wl,-wrap,_calloc_r#-Wl,-wrap,printf -Wl,-wrap,sprintf
endif

GLOBAL_LDFLAGS += -Wl,-wrap,printf

GLOBAL_LDFLAGS  += -L .
GLOBAL_LDFLAGS  += --specs=nosys.specs

ifdef TOOLCHAIN_NAME
$(NAME)_PREBUILT_LIBRARY := peripherals.$(HOST_ARCH).$(TOOLCHAIN_NAME).release.a
$(NAME)_PREBUILT_LIBRARY += MOC109.$(HOST_ARCH).$(TOOLCHAIN_NAME).release.a
endif

# Source files
$(NAME)_SOURCES := \
platform_stub.c \
../../$(HOST_ARCH)/platform_core.c \
../mico_platform_common.c \
platform_init.c

EXTRA_TARGET_MAKEFILES += \
mico-os/platform/MCU/MOC109/flash.mk  \
mico-os/platform/MCU/MOC109/image.mk

DEFAULT_LINK_SCRIPT := moc109.ld
