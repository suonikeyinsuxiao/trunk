
CC := arm-openwrt-linux-gcc
CXX := arm-openwrt-linux-g++
LD := arm-openwrt-linux-gcc
INCLUDE := -I ./include/alsa ./include/sys
SHARELIB:= -L ./
CFLAGS1 := -g -W -DPL_DEBUG -D_GNU_SOURCE  
LDFLAGSMY := -lasound -lm 

SRCS = $(wildcard *.c)
OBJS = $(patsubst %c,%o,$(SRCS))
TARGET = alsa-play

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(SHARELIB) $(LDFLAGSMY) -o $@ $^

%o: %c
	$(CC) $(CFLAGS1)  -c $< $(INCLUDE) -o $@
clean:
	@rm -f *.o $(TARGET)


