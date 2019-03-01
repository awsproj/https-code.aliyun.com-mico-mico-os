EXTRA_POST_BUILD_TARGETS += $(MICO_ALL_BIN_OUTPUT_FILE)

ifeq ($(HOST_OS),Win32)
CRC := "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/crc/win/crc.exe"
XZ 		:= "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/xz/win/xz.exe"
else  # Win32
ifeq ($(HOST_OS),Linux32)
CRC := "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/crc/linux/crc"
XZ 		:= "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/xz/linux/xz"
else # Linux32
ifeq ($(HOST_OS),Linux64)
CRC := "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/crc/linux/crc"
XZ 		:= "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/xz/linux/xz"
else # Linux64
ifeq ($(HOST_OS),OSX)
CRC := "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/crc/osx/crc"
XZ 		:= "$(SOURCE_ROOT)/mico-os/platform/MCU/MOC108/tools/xz/osx/xz"
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
ifeq ($(target), jd10)
BOOT_BIN_FILE := $(MICO_OS_PATH)/resources/moc_kernel/$(MODULE)/boot_jd.bin
else
BOOT_BIN_FILE := $(MICO_OS_PATH)/resources/moc_kernel/$(MODULE)/boot_zs.bin
endif
$(info ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ BOOT_BIN_FILE=$(BOOT_BIN_FILE))
endif
BOOT_OFFSET   := 0x0

#application 
GOLDENMAP_BIN_FILE := $(GOLDENMAP_FILE)
GOLDENMAP_OFFSET := 0xE000

#audio
VBS_FLASH_SONG_SRC_PATH := $(VBS_FLASH_SONG_SRC_DIR)
VBS_FLASH_SOUND_REMIND_SRC_PATH := $(VBS_FLASH_SOUND_REMIND_SRC_DIR)

#application 
APP_BIN_FILE := $(BIN_OUTPUT_FILE)
APP_OFFSET := 0x13200

#ate firmware
ATE_BIN_FILE := $(MICO_OS_PATH)/resources/ate_firmware/$(MODULE)/ate.bin
ATE_OFFSET := 0x1C501C

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

BK3266_RAW_FILE_NAME := $(notdir $(BK3266_RAW_FILE))
BK3266_RAW_XZ_FILE :=  build/$(CLEANED_BUILD_STRING)/resources/$(BK3266_RAW_FILE_NAME:.bin=.bin.xz)
BK3266_OTA_FILE :=  build/$(CLEANED_BUILD_STRING)/resources/$(BK3266_RAW_FILE_NAME:.bin=.ota.bin)
BK3266_OTA_OFFSET := 0x145000

BK3266_OTA_HDR_FILE := build/$(CLEANED_BUILD_STRING)/resources/$(BK3266_RAW_FILE_NAME:.bin=.otahdr.bin)
BK3266_OTA_HDR_OFFSET := 0x12000
BK3266CRC := $(BK3266CRC_FILE)
BK3266_OTA_HDR_SCRIPT := $(BK3266_OTA_HDR_SCRIPT_FILE)

$(BK3266_RAW_FILE): build_done

$(BK3266_RAW_XZ_FILE): $(BK3266_RAW_FILE)
	$(QUIET)$(ECHO) Compressing $< ...
	$(XZ) --lzma2=dict=32KiB --check=crc32 -k $<
	$(MV) $(<:.bin=.bin.xz) $(BK3266_RAW_XZ_FILE)

$(BK3266_OTA_HDR_FILE): $(BK3266_RAW_XZ_FILE)
	$(QUIET)$(ECHO) Making $@ ...
	$(eval SIZEVAL := $(shell $(PYTHON) $(IMAGE_SIZE_SCRIPT) $(BK3266_RAW_FILE)))
	$(eval CRCVAL := $(shell $(BK3266CRC) -f $(BK3266_RAW_FILE) -l $(SIZEVAL)))
	$(eval SIZEVAL := $(shell $(PYTHON) $(IMAGE_SIZE_SCRIPT) $(BK3266_RAW_XZ_FILE)))
	$(PYTHON) $(BK3266_OTA_HDR_SCRIPT) -s $(SIZEVAL) -c $(CRCVAL) -o $@

$(BK3266_OTA_FILE): $(BK3266_OTA_HDR_FILE) $(BK3266_RAW_XZ_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(BK3266_OTA_FILE) -f 0x00  $(BK3266_OTA_HDR_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(BK3266_OTA_FILE) -f 0x10  $(BK3266_RAW_XZ_FILE)

SONGS_BIN_NAME:= songs
SONGS_FTFS_BIN:= build/$(CLEANED_BUILD_STRING)/resources/$(SONGS_BIN_NAME).bin
sflash_gen_songs: build_done
	$(QUIET)$(ECHO) Making $(SONGS_FTFS_BIN) ...
	$(QUIET)$(shell $(PYTHON) $(MAKEFILES_PATH)/scripts/flash_pack.py 001 $(SONGS_BIN_NAME).bin $(VBS_FLASH_SONG_SRC_PATH))
	$(QUIET)$(MV) $(SOURCE_ROOT)$(SONGS_BIN_NAME).bin $(SOURCE_ROOT)build/$(CLEANED_BUILD_STRING)/resources

SONGS_BIN_FILE := $(SONGS_FTFS_BIN)
# 0x400000-0x16011C 4k对齐
SONGS_BIN_OFFSET := 0x29F000

SOUND_REMIND_BIN_NAME:= sound_remind
SOUND_REMIND_FTFS_BIN:= build/$(CLEANED_BUILD_STRING)/resources/$(SOUND_REMIND_BIN_NAME).bin
sflash_gen_soundremind: build_done
	$(QUIET)$(ECHO) Making $(SOUND_REMIND_FTFS_BIN) ...
	$(QUIET)$(shell $(PYTHON) $(MAKEFILES_PATH)/scripts/flash_pack.py 001 $(SOUND_REMIND_BIN_NAME).bin $(VBS_FLASH_SOUND_REMIND_SRC_PATH))
	$(QUIET)$(MV) $(SOURCE_ROOT)$(SOUND_REMIND_BIN_NAME).bin $(SOURCE_ROOT)build/$(CLEANED_BUILD_STRING)/resources

SOUND_REMIND_BIN_FILE := $(SOUND_REMIND_FTFS_BIN)
SOUND_REMIND_BIN_OFFSET := 0x200000

$(MICO_ALL_BIN_OUTPUT_FILE): $(OTA_BIN_OUTPUT_FILE) $(BK3266_OTA_FILE) sflash_gen_songs sflash_gen_soundremind
	$(QUIET)$(ECHO) Making $@ ...
	$(CP) $(BIN_OUTPUT_FILE) $(RAW_BIN_OUTPUT_FILE)
	$(CP) $(CRC_BIN_OUTPUT_FILE) $(BIN_OUTPUT_FILE)
	$(RM) $(CRC_BIN_OUTPUT_FILE)
	$(QUIET)$(RM) $(MICO_ALL_BIN_OUTPUT_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(BOOT_OFFSET) $(BOOT_BIN_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(GOLDENMAP_OFFSET) $(GOLDENMAP_BIN_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(BK3266_OTA_HDR_OFFSET) $(BK3266_OTA_HDR_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(APP_OFFSET)  $(APP_BIN_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(BK3266_OTA_OFFSET)  $(BK3266_OTA_FILE)
#	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(ATE_OFFSET)  $(ATE_BIN_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(SOUND_REMIND_BIN_OFFSET)  $(SOUND_REMIND_BIN_FILE)
	$(PYTHON) $(GEN_COMMON_BIN_OUTPUT_FILE_SCRIPT) -o $(MICO_ALL_BIN_OUTPUT_FILE) -f $(SONGS_BIN_OFFSET)  $(SONGS_BIN_FILE)

dt ?= app

download: $(dt)

APP_OTA_FILE := $(LINK_OUTPUT_FILE:$(LINK_OUTPUT_SUFFIX)=.ota$(BIN_OUTPUT_SUFFIX))

app: $(MICO_ALL_BIN_OUTPUT_FILE)
	$(eval IMAGE_SIZE := $(shell $(PYTHON) $(IMAGE_SIZE_SCRIPT) $(APP_OTA_FILE)))
	$(QUIET)$(ECHO) Downloading $(APP_OTA_FILE), size: $(IMAGE_SIZE) bytes... 
	$(PYTHON) mico-os/sub_build/spi_flash_write_progressbar/sflash_write_moc108.py -o $(OPENOCD_FULL_NAME) -f $(APP_OTA_FILE) -a 0x13200

codec: $(MICO_ALL_BIN_OUTPUT_FILE)
	$(eval IMAGE_SIZE := $(shell $(PYTHON) $(IMAGE_SIZE_SCRIPT) $(BK3266_OTA_HDR_FILE)))
	$(QUIET)$(ECHO) Downloading $(BK3266_OTA_HDR_FILE), size: $(IMAGE_SIZE) bytes... 
	$(PYTHON) mico-os/sub_build/spi_flash_write_progressbar/sflash_write_moc108.py -o $(OPENOCD_FULL_NAME) -f $(BK3266_OTA_HDR_FILE) -a $(BK3266_OTA_HDR_OFFSET)
	$(eval IMAGE_SIZE := $(shell $(PYTHON) $(IMAGE_SIZE_SCRIPT) $(BK3266_OTA_FILE)))
	$(QUIET)$(ECHO) Downloading $(BK3266_OTA_FILE), size: $(IMAGE_SIZE) bytes... 
	$(PYTHON) mico-os/sub_build/spi_flash_write_progressbar/sflash_write_moc108.py -o $(OPENOCD_FULL_NAME) -f $(BK3266_OTA_FILE) -a $(BK3266_OTA_OFFSET) -x 0

songs: $(MICO_ALL_BIN_OUTPUT_FILE)
	echo $(OPENOCD_FULL_NAME)
	$(eval IMAGE_SIZE := $(shell $(PYTHON) $(IMAGE_SIZE_SCRIPT) $(SONGS_BIN_FILE)))
	$(QUIET)$(ECHO) Downloading $(SONGS_BIN_FILE), size: $(IMAGE_SIZE) bytes... 
	$(PYTHON) mico-os/sub_build/spi_flash_write_progressbar/sflash_write_moc108.py -o $(OPENOCD_FULL_NAME) -f $(SONGS_BIN_FILE) -a $(SONGS_BIN_OFFSET)

sr: $(MICO_ALL_BIN_OUTPUT_FILE)
	echo $(OPENOCD_FULL_NAME)
	$(eval IMAGE_SIZE := $(shell $(PYTHON) $(IMAGE_SIZE_SCRIPT) $(SOUND_REMIND_BIN_FILE)))
	$(QUIET)$(ECHO) Downloading $(SOUND_REMIND_BIN_FILE), size: $(IMAGE_SIZE) bytes... 
	$(PYTHON) mico-os/sub_build/spi_flash_write_progressbar/sflash_write_moc108.py -o $(OPENOCD_FULL_NAME) -f $(SOUND_REMIND_BIN_FILE) -a $(SOUND_REMIND_BIN_OFFSET)


kill_openocd:
	$(KILL_OPENOCD)