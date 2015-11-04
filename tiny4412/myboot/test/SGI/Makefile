CROSS_COMPILE:=arm-linux-
CC:=$(CROSS_COMPILE)gcc
CC_FLAGS:= -fno-builtin -c 
CC_FLAGS_EX:= -fno-builtin 
LD:=$(CROSS_COMPILE)ld
LD_FLAGS:=-T sgi.lds
OBJCOPY:=$(CROSS_COMPILE)objcopy
COPY_FLAGS:=-O binary
OBJDUMP:=$(CROSS_COMPILE)objdump
DUMP_FLAGS:=-D
OBJS:=test.o string.o mmu.o vector.o sgi.o
TAG:=test
RM_FLAGS:=-rf

all:$(OBJS)
	$(LD) $(LD_FLAGS) $(OBJS) -o $(TAG)
	$(OBJCOPY) $(COPY_FLAGS) $(TAG) $(TAG).bin
	$(OBJDUMP) $(DUMP_FLAGS) $(TAG) > $(TAG).dis
%.o:%.s
	#$(CC) $(CC_FLAGS) $^ $(CC_FLAGS_EX)
	$(CC) $(CC_FLAGS) $^ 
%.o:%.S
	$(CC) $(CC_FLAGS) $^ 
%.o:%.c
	$(CC) $(CC_FLAGS) $^ 

clean:
	rm $(RM_FLAGS) $(OBJS)  $(TAG).bin $(TAG) $(TAG).dis


