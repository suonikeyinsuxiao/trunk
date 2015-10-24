TOP_DIR := $(shell pwd)
#APP = $(TOP_DIR)/bin/camera_h264encode
APP = h264encode 

CC = gcc
CFLAGS = -g 
LIBS = -lpthread -lx264 -lm 
#DEP_LIBS = -L$(TOP_DIR)/lib
HEADER =
OBJS = test.o convert.o videoCapture.o encoder.o netsocket.o message.o

all:  $(OBJS)
	$(CC) -g -o $(APP) $(OBJS) $(LIBS)  
#	$(CC) -g -o $(APP) $(OBJS) $(LIBS) $(DEP_LIBS) 

clean:
	rm -f *.o a.out $(APP) core *~
