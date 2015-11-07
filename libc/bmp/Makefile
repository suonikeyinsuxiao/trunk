#CROSS_COMPILE:=
CC:=gcc
CC_FLAGS:= -c
LD:=ld
#LD_FLAGS:=
OBJS:=test.o bmp.o
SRC:=test.c bmp.c
RM:=rm
RM_FLAGS:=-rf
TARGET:=bmp

all:
	$(CC) $(SRC) -o $(TARGET)

#all:$(OBJS)
#	$(LD) $(OBJS) -o $(TARGET)
#
#%.o:%.c
#	$(CC) $(CC_FLAGS) $^  

clean:
	$(RM) $(RM_FLAGS) $(OBJS) $(TARGET)

