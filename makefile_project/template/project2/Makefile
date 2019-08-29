TOP_DIR := $(shell pwd)

PROJECT2_CROSS := aarch64-himix100-linux
PROJECT2_CROSS_COMPILE := $(PROJECT2_CROSS)-

MAKE := make

CHIP := hi3559av100
ARCH := arm64
AMP_TYPE ?= linux

BOOT_MEDIA ?= spi
ifeq ($(BOOT_MEDIA), nand)
	NAND_FLAG = nand_
endif
ifeq ($(BOOT_MEDIA), emmc)
	NAND_FLAG = emmc_
endif
ifeq ($(BOOT_MEDIA), ufs)
	NAND_FLAG = ufs_
endif


ifeq ($(AMP_TYPE),linux)
	CPU_TYPE ?= multi-core
KERNEL_CFG:=$(CHIP)_arm64_big_little_$(NAND_FLAG)defconfig
endif
ifeq ($(AMP_TYPE),linux_liteos)
	CPU_TYPE ?= big-little
KERNEL_CFG:=$(CHIP)_arm64_big_little_$(NAND_FLAG)amp_defconfig
endif

BLACK 	:= "\e[30;1m"
RED  	:= "\e[31;1m"
GREEN 	:= "\e[32;1m"
YELLOW 	:= "\e[33;1m"
BLUE  	:= "\e[34;1m"
PURPLE 	:= "\e[35;1m"
CYAN  	:= "\e[36;1m"
WHITE 	:= "\e[37;1m"
DONE    := "\033[0m"

CP := cp -adf
RM := rm -rf
MKDIR := mkdir -p



.PHONY: tools

all: tools 

clean: tools_clean

distclean: tools_distclean

print_env:
	@echo "chip:$(CHIP)"
	@echo "arch:$(ARCH)"
	@echo "amp type:$(AMP_TYPE)"
	@echo "boot media:$(BOOT_MEDIA)"
	@echo "kernel config:$(KERNEL_CFG)"

tools:
	@echo -e $(GREEN)"---build tools---"$(DONE)
	$(MKDIR) $(TOP_DIR)/pub/include
	$(MKDIR) $(TOP_DIR)/pub/lib
	$(MKDIR) $(TOP_DIR)/pub/openssh
	$(MAKE) -C $(TOP_DIR)/tools/zlib TOOL_HOST=$(PROJECT2_CROSS)
	$(CP) $(TOP_DIR)/tools/zlib/inst/include/*.h $(TOP_DIR)/pub/include
	$(CP) $(TOP_DIR)/tools/zlib/inst/lib/libz.so* $(TOP_DIR)/pub/lib
	$(MAKE) -C $(TOP_DIR)/tools/openssl TOOL_HOST=$(PROJECT2_CROSS)
	$(CP) $(TOP_DIR)/tools/openssl/inst/lib/libcrypto.so* $(TOP_DIR)/pub/lib 
	$(CP) $(TOP_DIR)/tools/openssl/inst/lib/libssl.so* $(TOP_DIR)/pub/lib 
	$(CP) $(TOP_DIR)/tools/openssl/inst/include/openssl $(TOP_DIR)/pub/include 
	$(MAKE) -C $(TOP_DIR)/tools/openssh	TOOL_HOST=$(PROJECT2_CROSS) TOOL_JOBS=8
	$(CP) $(TOP_DIR)/tools/openssh/inst/bin $(TOP_DIR)/pub/openssh 
	$(CP) $(TOP_DIR)/tools/openssh/inst/libexec $(TOP_DIR)/pub/openssh 
	$(CP) $(TOP_DIR)/tools/openssh/inst/sbin $(TOP_DIR)/pub/openssh 
	$(CP) $(TOP_DIR)/tools/openssh/inst/ssl $(TOP_DIR)/pub/openssh 
	$(CP) $(TOP_DIR)/tools/openssh/inst/var $(TOP_DIR)/pub/openssh 

tools_clean:
	$(MAKE) -C $(TOP_DIR)/tools/zlib clean
	$(MAKE) -C $(TOP_DIR)/tools/openssl clean
	$(MAKE) -C $(TOP_DIR)/tools/openssh clean

tools_distclean:
	-$(RM) $(TOP_DIR)/pub
	$(MAKE) -C $(TOP_DIR)/tools/zlib distclean
	$(MAKE) -C $(TOP_DIR)/tools/openssl distclean
	$(MAKE) -C $(TOP_DIR)/tools/openssh distclean

