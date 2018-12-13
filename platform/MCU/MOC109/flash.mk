#
#  UNPUBLISHED PROPRIETARY SOURCE CODE
#  Copyright (c) 2016 MXCHIP Inc.
#
#  The contents of this file may not be disclosed to third parties, copied or
#  duplicated in any form, in whole or in part, without the prior written
#  permission of MXCHIP Corporation.
#

.PHONY: download kill_openocd

download_app: $(BIN_OUTPUT_FILE) display_map_summary kill_openocd
	$(eval IMAGE_SIZE := $(shell $(PYTHON) $(IMAGE_SIZE_SCRIPT) $(BIN_OUTPUT_FILE)))
	$(QUIET)$(ECHO) Downloading application to partition: $(APPLICATION_FIRMWARE_PARTITION_TCL) size: $(IMAGE_SIZE) bytes... 
	$(PYTHON) mico-os/sub_build/spi_flash_write_progressbar/sflash_write_moc109.py -o $(OPENOCD_FULL_NAME) -f $(BIN_OUTPUT_FILE) -a 0x13200

download: download_app $(if $(findstring total,$(MAKECMDGOALS)), EXT_IMAGE_DOWNLOAD,)

kill_openocd:
	$(KILL_OPENOCD)
