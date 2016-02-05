SUBDIRS=$(shell ls -l | grep ^d | awk '{print $$9}')

CUR_SOURCE=${wildcard *.c}
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

all:$(SUBDIRS) $(CUR_OBJS)

$(SUBDIRS):ECHO
	make -C $@

$(CUR_OBJS):%.o:%.c
ifeq ($(VER), DEBUG)
	$(CC) -c $^ -o $(ROOT_DIR)/$(OBJS_DIR)/$@ -I$(ROOT_DIR)/include -DPL_DEBUG
else
	$(CC) -c $^ -o $(ROOT_DIR)/$(OBJS_DIR)/$@ -I$(ROOT_DIR)/include 
endif

ECHO:
	@echo $(SUBDIRS)
