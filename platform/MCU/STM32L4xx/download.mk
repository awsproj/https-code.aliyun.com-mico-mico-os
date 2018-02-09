#
#  UNPUBLISHED PROPRIETARY SOURCE CODE
#  Copyright (c) 2016 MXCHIP Inc.
#
#  The contents of this file may not be disclosed to third parties, copied or
#  duplicated in any form, in whole or in part, without the prior written
#  permission of MXCHIP Corporation.
#

.PHONY: run download download_app kill_openocd

OPENOCD_LOG_FILE ?= $(SOURCE_ROOT)build/openocd_log.txt
DOWNLOAD_LOG := >> $(OPENOCD_LOG_FILE)

download_app: $(STRIPPED_LINK_OUTPUT_FILE) display_map_summary kill_openocd
	$(eval IMAGE_SIZE := $(shell $(PYTHON) $(IMAGE_SIZE_SCRIPT) $(BIN_OUTPUT_FILE)))
	$(QUIET)$(ECHO) Downloading application to partition: $(APPLICATION_FIRMWARE_PARTITION_TCL) size: $(IMAGE_SIZE) bytes... 
	$(call CONV_SLASHES, $(OPENOCD_FULL_NAME)) -s $(SOURCE_ROOT) -f $(OPENOCD_CFG_PATH)interface/$(JTAG).cfg -f $(OPENOCD_CFG_PATH)$(HOST_OPENOCD)/$(HOST_OPENOCD).cfg -c init -c "reset halt" -c "flash write_image erase $(STRIPPED_LINK_OUTPUT_FILE)" -c shutdown $(DOWNLOAD_LOG) 2>&1 && $(ECHO) Download complete && $(ECHO_BLANK_LINE) || $(ECHO) Download failed. See build/openocd_log.txt for detail.

download: download_app

kill_openocd:
	$(KILL_OPENOCD)

run: download_app
	$(call CONV_SLASHES, $(OPENOCD_FULL_NAME)) -s $(SOURCE_ROOT) -f $(OPENOCD_CFG_PATH)interface/$(JTAG).cfg -f $(OPENOCD_CFG_PATH)$(HOST_OPENOCD)/$(HOST_OPENOCD).cfg -c init -c "reset" -c shutdown $(DOWNLOAD_LOG) 2>&1 && $(ECHO) Reset complete && $(ECHO_BLANK_LINE) || $(ECHO) Reset failed. See build/openocd_log.txt for detail.
