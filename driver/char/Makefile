obj-m += test.o

FS := /disk/A9/filesystem
KERNEL_SRC := /disk/A9/linux-3.5

all:
	make -C $(KERNEL_SRC) M=`pwd` modules

clean:
	make -C $(KERNEL_SRC) M=`pwd` clean

install:
	make -C $(KERNEL_SRC) M=`pwd` modules_install INSTALL_MOD_PATH=$(FS) 

app:
	arm-linux-gcc syscall.c -o syscall
