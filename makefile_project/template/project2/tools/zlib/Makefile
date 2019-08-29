TOOL_TOP_DIR ?= $(shell pwd)
TOOL_TAR := zlib-1.2.11.tar.gz 
TOOL_NAME := zlib-1.2.11
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
		prefix=$(TOOL_HOST) CC=$(TOOL_CC) ./configure --prefix=$(TOOL_TOP_DIR)/$(TOOL_INSTALL) ; \
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
