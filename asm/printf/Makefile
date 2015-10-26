
all:
	arm-linux-gcc -c printf.S -o printf.o
	arm-linux-ld -Ttext=0x40008000 printf.o -o printf
	arm-linux-objcopy -O binary printf printf.bin

clean:
	rm -rf *.o printf.bin printf
