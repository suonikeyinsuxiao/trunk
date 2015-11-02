
all:
	arm-linux-gcc  -c test.c   -o test.o    -fno-builtin
	arm-linux-gcc  -c string.c -o string.o  -fno-builtin
	arm-linux-gcc  -c mmu.c    -o mmu.o     -fno-builtin
	arm-linux-gcc  -c vector.S -o vector.o  -fno-builtin
	arm-linux-gcc  -c spi.c -o spi.o  -fno-builtin
	arm-linux-gcc  -c adc.c -o adc.o  -fno-builtin
	arm-linux-ld -T pwm.lds *.o -o test 
	arm-linux-objcopy -O binary test test.bin


clean:
	rm -rf *.o  test.bin test 
