
all:
	arm-linux-gcc  -c test.c   -o test.o    -fno-builtin
	arm-linux-gcc  -c wdt.c -o wdt.o  -fno-builtin
	arm-linux-gcc  -c string.c -o string.o  -fno-builtin
	arm-linux-ld -T wdt.lds  *.o -o test 
	arm-linux-objcopy -O binary test test.bin


clean:
	rm -rf *.o  test.bin test 
