
all:
	arm-linux-gcc  -c adc.c   -o adc.o    -fno-builtin
	arm-linux-ld -T adc.lds *.o -o test 
	arm-linux-objcopy -O binary test test.bin


clean:
	rm -rf *.o  test.bin  test
