TARGET:=flyfish
BINDIR:=$(CURDIR)/../bin
SOURCEDIR:=$(CURDIR)/../src
CC:=gcc
CFLAGS:=-Wall 
SOURCE:=main.c init.c fish.c image.c music.c barrier.c gameover.c
INCLUDE:=-I$(CURDIR)/../include
OBJS:=$(patsubst %.c,%.o,$(SOURCE))
SHARELIB:=-lSDL -lSDL_image



all:$(OBJS) $(TARGET) 
	@mv $(TARGET) $(BINDIR)
#	@mv $(SOURCE) $(SOURCEDIR) 

$(TARGET):$(OBJS)
	@$(CC) -o $(TARGET)  $(OBJS) $(SHARELIB)

%.o:%.c 
	@echo "$(CC) -c $(CFLAGS) $<  -o $(patsubst %.c,%.o,$<)"
	@$(CC) -c $(CFLAGS) $< $(INCLUDE) -o $@

.PHONY:clean

clean:
	-rm $(OBJS)
	@echo "rm bin/$(TARGET)"
	@-rm $(BINDIR)/$(TARGET)
