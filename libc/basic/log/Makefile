CC := gcc
VER:=DEBUG

SUBDIRS=$(shell ls -l | grep ^d | awk '{if(($$9!="debug") && ($$9!="release") && ($$9!="include")) print $$9}')

DEBUG=$(shell ls -l | grep ^d | awk '{if($$9 == "debug") print $$9}')
RELEASE=$(shell ls -l | grep ^d | awk '{if($$9 == "release") print $$9}')

ROOT_DIR=$(shell pwd)

BIN=myapp

ifeq ($(VER), DEBUG)
OBJS_DIR=debug/obj
BIN_DIR=debug/bin
else
OBJS_DIR=release/obj
BIN_DIR=release/bin
endif

HEADER_DIR=include
HEADERS=$(HEADER_DIR)/ \
		$(foreach n,$(SUBDIRS),$(n)/)

CUR_SOURCE=${wildcard *.c}

CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

export CC BIN OBJS_DIR BIN_DIR ROOT_DIR HEADERS VER

all:$(SUBDIRS) $(CUR_OBJS) $(VER)

$(SUBDIRS):ECHO
	make -C $@

DEBUG:ECHO
	make -C debug 

RELEASE:ECHO
	make -C release

ECHO:
	@echo $(SUBDIRS)

$(CUR_OBJS):%.o:%.c
ifeq ($(VER), DEBUG)
	$(CC) -c $^ -o $(ROOT_DIR)/$(OBJS_DIR)/$@ $(foreach n,$(HEADERS),-I$(ROOT_DIR)/$(n)) -DPL_DEBUG
else
	$(CC) -c $^ -o $(ROOT_DIR)/$(OBJS_DIR)/$@ $(foreach n,$(HEADERS),-I$(ROOT_DIR)/$(n))
endif
   

.PHONY:clean
clean:
ifeq ($(VER), DEBUG)
	@rm $(OBJS_DIR)/*.o
	@rm -rf $(BIN_DIR)/*
else
	@rm $(OBJS_DIR)/*.o
	@rm -rf $(BIN_DIR)/*
endif
