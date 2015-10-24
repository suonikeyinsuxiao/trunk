TOP_DIR := $(shell pwd)
#APP = $(TOP_DIR)/bin/camera_h264encode
APP = server 

CC = gcc
CFLAGS = -g 
LIBS = -lavcodec -lavformat -lavutil -lswscale 
DEP_LIBS = -L /usr/local/lib
HEADER =
OBJS = server.o netsocket.o decoder.o

all:  $(OBJS)
	$(CC) -g -o $(APP) $(OBJS) $(LIBS) $(DEP_LIBS) 

clean:
	rm -f *.o a.out $(APP) core *~
