
all:
	arm-linux-gcc  -c test.c -o test.o  -fno-builtin
	arm-linux-gcc  -c common.c -o common.o  -fno-builtin
	arm-linux-gcc  -c string.c -o string.o  -fno-builtin
	arm-linux-gcc  -c gpio.c -o gpio.o  -fno-builtin
	arm-linux-gcc  -c _divsi3.S -o _divsi3.o  -fno-builtin
	arm-linux-gcc  -c _udivsi3.S -o _udivsi3.o  -fno-builtin
	arm-linux-gcc  -c div0.c -o div0.o  -fno-builtin
	arm-linux-gcc  -c cmd.c -o cmd.o  -fno-builtin
	arm-linux-gcc  -c cmd_led.c -o cmd_led.o  -fno-builtin
	arm-linux-gcc  -c cmd_buzzer.c -o cmd_buzzer.o  -fno-builtin
	arm-linux-gcc  -c cmd_key.c -o cmd_key.o  -fno-builtin
	arm-linux-ld -T test.lds *.o -o test
	arm-linux-objcopy -O binary test test.bin


clean:
	rm -rf *.o  test.bin  
