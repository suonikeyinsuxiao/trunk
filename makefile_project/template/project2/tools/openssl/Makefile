TOOL_TOP_DIR ?= $(shell pwd)
TOOL_TAR := openssl-1.0.2s.tar.gz
TOOL_NAME := openssl-1.0.2s
TOOL_INSTALL := inst
TOOL_CROSS ?= 
TOOL_CROSS_CFLAGS ?=
TOOL_CROSS_LDFLAGS ?=
TOOL_HOST ?= 
TOOL_CC = $(TOOL_HOST)-gcc
TOOL_AR = $(TOOL_HOST)-ar
TOOL_JOBS ?= 4

all:
	-rm $(TOOL_TOP_DIR)/$(TOOL_NAME) -rf;
	tar -xzf $(TOOL_TAR);
	mkdir -p $(TOOL_TOP_DIR)/$(TOOL_INSTALL)/;
	pushd $(TOOL_TOP_DIR)/$(TOOL_NAME)/; \
		./Configure linux-generic32 no-asm shared --prefix=$(TOOL_TOP_DIR)/$(TOOL_INSTALL) --cross-compile-prefix=$(TOOL_HOST)- ; \
		make -j$(TOOL_JOBS); \
		make install ; \
	popd

.PHONY: clean
clean:
	-pushd $(TOOL_TOP_DIR)/$(TOOL_NAME)/ &&\
		make -C $(TOOL_TOP_DIR)/$(TOOL_NAME)/ distclean && \
	popd

.PHONY: distclean
distclean:
	-rm $(TOOL_TOP_DIR)/$(TOOL_NAME) -rf;
	-rm $(TOOL_TOP_DIR)/$(TOOL_INSTALL) -rf;
