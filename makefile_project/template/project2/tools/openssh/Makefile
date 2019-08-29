TOOL_TOP_DIR ?= $(shell pwd)
TOOL_TAR := openssh-6.8p1.tar.gz
TOOL_NAME := openssh-6.8p1
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
		./configure --disable-strip --prefix=$(TOOL_TOP_DIR)/$(TOOL_INSTALL) --sysconfdir=$(TOOL_TOP_DIR)/$(TOOL_INSTALL)/ssl --with-privsep-path=$(TOOL_TOP_DIR)/$(TOOL_INSTALL)/var/empty \
		--host=$(TOOL_HOST) CC=$(TOOL_CC) AR=$(TOOL_AR) \
		--with-privsep-user=sshd CFLAGS="-I$(TOOL_TOP_DIR)/../../pub/include" LDFLAGS="-L$(TOOL_TOP_DIR)/../../pub/lib"; \
		make -j$(TOOL_JOBS); \
		make install-nokeys; \
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
