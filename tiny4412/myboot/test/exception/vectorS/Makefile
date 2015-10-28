
all:
	arm-linux-gcc  -c vector.S -o vector.o -fno-builtin
	arm-linux-ld -Ttext=0x5fff0000 *.o -o vector 
	arm-linux-objcopy -O binary vector vector.bin


clean:
	rm -rf *.o  vector.bin  
