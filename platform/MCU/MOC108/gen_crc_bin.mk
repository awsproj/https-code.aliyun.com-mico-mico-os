EXTRA_POST_BUILD_TARGETS += $(MICO_ALL_BIN_OUTPUT_FILE)

ifeq ($(HOST_OS),Win32)
CRC := "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/crc/win/crc.exe"
XZ 		:= "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/xz/win/xz.exe"
BK3266CRC := "$(SOURCE_ROOT)/bk3266ota/bk3266br/tools/crc32_win"
else  # Win32
ifeq ($(HOST_OS),Linux32)
CRC := "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/crc/linux/crc"
XZ 		:= "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/xz/linux/xz"
BK3266CRC := "$(SOURCE_ROOT)/bk3266ota/bk3266br/tools/crc32_linux"
else # Linux32
ifeq ($(HOST_OS),Linux64)
CRC := "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/crc/linux/crc"
XZ 		:= "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/xz/linux/xz"
BK3266CRC := "$(SOURCE_ROOT)/bk3266ota/bk3266br/tools/crc32_linux"
else # Linux64
ifeq ($(HOST_OS),OSX)
CRC := "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/crc/osx/crc"
XZ 		:= "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/xz/osx/xz"
BK3266CRC := "$(SOURCE_ROOT)/bk3266ota/bk3266br/tools/crc32_osx"
else # OSX
$(error not surport for $(HOST_OS))
endif # OSX
endif # Linux64
endif # Linux32
endif # Win32

ADD_MD5_SCRIPT := $(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/add_md5.py

# _crc.bin
CRC_BIN_OUTPUT_FILE :=$(LINK_OUTPUT_FILE:$(LINK_OUTPUT_SUFFIX)=_crc$(BIN_OUTPUT_SUFFIX))
# _crc.bin.xz
CRC_XZ_BIN_OUTPUT_FILE :=$(LINK_OUTPUT_FILE:$(LINK_OUTPUT_SUFFIX)=_crc$(BIN_OUTPUT_SUFFIX).xz)
# .ota.bin
OTA_BIN_OUTPUT_FILE := $(LINK_OUTPUT_FILE:$(LINK_OUTPUT_SUFFIX)=.ota$(BIN_OUTPUT_SUFFIX))
# .raw.bin
RAW_BIN_OUTPUT_FILE := $(LINK_OUTPUT_FILE:$(LINK_OUTPUT_SUFFIX)=.raw$(BIN_OUTPUT_SUFFIX))

#bootloader
ifneq (,$(filter MK5060,$(PLATFORM)))
BOOT_BIN_FILE := $(MICO_OS_PATH)/resources/moc_kernel/$(MODULE)/boot_qc.bin
else
BOOT_BIN_FILE := $(MICO_OS_PATH)/resources/moc_kernel/$(MODULE)/boot.bin
endif
BOOT_OFFSET   := 0x0

#application 
APP_BIN_FILE := $(BIN_OUTPUT_FILE)
APP_OFFSET := 0x13200

#ate firmware
ATE_BIN_FILE := $(MICO_OS_PATH)/resources/ate_firmware/$(MODULE)/ate.bin
ATE_OFFSET := 0x1C5000

# Required to build Full binary file
GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT:= $(SCRIPTS_PATH)/gen_common_bin_output_file.py

MICO_ALL_BIN_OUTPUT_FILE :=$(LINK_OUTPUT_FILE:$(LINK_OUTPUT_SUFFIX)=.all$(BIN_OUTPUT_SUFFIX))

$(CRC_BIN_OUTPUT_FILE): $(STRIPPED_LINK_OUTPUT_FILE)
	$(QUIET)$(OBJCOPY) -O binary -R .eh_frame -R .init -R .fini -R .comment -R .ARM.attributes $(STRIPPED_LINK_OUTPUT_FILE) $(BIN_OUTPUT_FILE)
	$(CRC) $(BIN_OUTPUT_FILE) 0 0 0 0 > $(DEV_NULL)

$(OTA_BIN_OUTPUT_FILE): $(CRC_BIN_OUTPUT_FILE)
	$(XZ) --lzma2=dict=32KiB --check=crc32 -k $(CRC_BIN_OUTPUT_FILE)
	$(CP) $(CRC_XZ_BIN_OUTPUT_FILE) $(OTA_BIN_OUTPUT_FILE)
	$(RM) $(CRC_XZ_BIN_OUTPUT_FILE)
	$(PYTHON) $(ADD_MD5_SCRIPT) $(OTA_BIN_OUTPUT_FILE)

BK3266_OTA_FILE := lib_audio_3266/firmware/5.1/BK326x_s_flash_image_crc.bin
BK3266_OTA_OFFSET := 0x145000
BK3266_OTA_XZ_FILE := $(BK3266_OTA_FILE:.bin=.bin.xz)

BK3266_OTA_HDR_FILE := $(BK3266_OTA_FILE:.bin=.otahdr.bin)
BK3266_OTA_HDR_OFFSET := 0x12000
BK3266_OTA_HDR_SCRIPT := bk3266ota/tools/bk3266otahdr.py

AUDIO_FILE := build/$(CLEANED_BUILD_STRING)/resources/filesystem.bin
AUDIO_OFFSET := 0x200000

$(BK3266_OTA_XZ_FILE): $(BK3266_OTA_FILE) build_done
	$(QUIET)$(ECHO) Compressing $< ...
	$(XZ) --lzma2=dict=32KiB --check=crc32 -k $<

$(BK3266_OTA_HDR_FILE): $(BK3266_OTA_XZ_FILE)
	$(QUIET)$(ECHO) Making $@ ...
	$(eval SIZEVAL := $(shell $(PYTHON) $(IMAGE_SIZE_SCRIPT) $(BK3266_OTA_XZ_FILE)))
	$(eval CRCVAL := $(shell $(BK3266CRC) -f $(BK3266_OTA_FILE) -l 0xFD000))
	$(PYTHON) $(BK3266_OTA_HDR_SCRIPT) -s $(SIZEVAL) -c $(CRCVAL) -o $@
	
$(MICO_ALL_BIN_OUTPUT_FILE): $(OTA_BIN_OUTPUT_FILE) $(BK3266_OTA_HDR_FILE)
	$(QUIET)$(ECHO) Making $@ ...
	$(CP) $(BIN_OUTPUT_FILE) $(RAW_BIN_OUTPUT_FILE)
	$(CP) $(CRC_BIN_OUTPUT_FILE) $(BIN_OUTPUT_FILE)
	$(RM) $(CRC_BIN_OUTPUT_FILE)
	$(QUIET)$(RM) $(MICO_ALL_BIN_OUTPUT_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(BOOT_OFFSET) $(BOOT_BIN_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(BK3266_OTA_HDR_OFFSET) $(BK3266_OTA_HDR_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(APP_OFFSET)  $(APP_BIN_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(BK3266_OTA_OFFSET)  $(BK3266_OTA_XZ_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(ATE_OFFSET)  $(ATE_BIN_FILE)
	$(RM) $(BK3266_OTA_XZ_FILE)
	$(RM) $(BK3266_OTA_HDR_FILE)