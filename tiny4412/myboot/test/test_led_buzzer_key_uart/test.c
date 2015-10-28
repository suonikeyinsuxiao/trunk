#include "gpio.h"
#include "common.h"
#include "cmd.h"
#include "cmd_led.h"
#include "cmd_buzzer.h"
#include "string.h"

#define GPM4CON_ADDR (GPIOBASE_PART2+0x02E0)//led
#define GPM4DAT_ADDR (GPIOBASE_PART2+0x02E4)
#define GPX3CON_ADDR (GPIOBASE_PART2+0x0C60)//key 
#define GPX3DAT_ADDR (GPIOBASE_PART2+0x0C64) 
#define GPD0CON_ADDR (GPIOBASE_PART1+0x00A0)//buzzer
#define GPD0DAT_ADDR (GPIOBASE_PART1+0x00A4) 

PRINT_F printf = (void*)0x43E11434;

#define LEDTEST
//#define BUZZERTEST
//#define UARTTEST
//#define KEYTEST

int _start(void)
{
	printf("product ID=%#x\n", getPROID);
	printf("package ID=%#x\n", getPACKAGEID);

	//led
#ifdef LEDTEST 
	//struct tiny4412_gpio_bank* psBank = GPM4CON_ADDR; 
	//gpio_direction_output(psBank, 0, 1);//GPM4:0 output
	//gpio_direction_output(psBank, 1, 1);//GPM4:1 output
	//gpio_direction_output(psBank, 2, 1);//GPM4:2 output
	//gpio_direction_output(psBank, 3, 1);//GPM4:3 output

	while(1)
	{
		//gpio_set_value(psBank, 0 ,0);
		//gpio_set_value(psBank, 1 ,0);
		//gpio_set_value(psBank, 2 ,0);
		//gpio_set_value(psBank, 3 ,0);
		ledOn(0);

		delay();

		ledOff(0);

		//gpio_set_value(psBank, 0 , 1);
		//gpio_set_value(psBank, 1 , 1);
		//gpio_set_value(psBank, 2 , 1);
		//gpio_set_value(psBank, 3 , 1);
			
		delay();
	}

#endif

#ifdef BUZZERTEST
	struct tiny4412_gpio_bank* psBank = GPD0CON_ADDR; 
	gpio_direction_output(psBank, 0, 0);//GPM4:0 output

	while(1)
	{
		gpio_set_value(psBank,0, 1);	
		delay();
		gpio_set_value(psBank,0, 0);	
		delay();
	}

#endif

#ifdef KEYTEST
	initKey();	
	initLed();
	initBuzzer();


	CMDTYPE_E type=CMDUNKNOWN;
	int i = 2;

	while(1)
	{

		type = processKey(2);
		switch(type)
		{
			case KEY1ON:
			{
				ledAllOn();	
				buzzerOn();
			}
			break;
			case KEY1OFF:
			{
				ledAllOff();	
				buzzerOff();
			}
			break;
		}

//		for (i=2; i < 6; i++)
//		{
//			type = processKey(i);
//	//		key_delay();
//			switch(type)
//			{
//				case KEY1ON:
//				{
//					ledAllOn();	
//					buzzerOn();
//				}
//				break;
//				case KEY1OFF:
//				{
//					ledAllOff();	
//					buzzerOff();
//				}
//				break;
//				case KEY2ON:
//				{
//					ledAllOn();	
//					buzzerOn();
//				}
//				break;
//				case KEY2OFF:
//				{
//					ledAllOff();	
//					buzzerOff();
//				}
//				break;
//				case KEY3ON:
//				{
//					ledAllOn();	
//					buzzerOn();
//				}
//				break;
//				case KEY3OFF:
//				{
//					ledAllOff();	
//					buzzerOff();
//				}
//				break;
//				case KEY4ON:
//				{
//					ledAllOn();	
//					buzzerOn();
//				}
//				break;
//				case KEY4OFF:
//				{
//					ledAllOff();	
//					buzzerOff();
//				}
//				break;
//			}
//		}

//		delay();
	}
#endif

#ifdef UARTTEST
	//GPA0 0~1
	GPA0CON &= ~0xff;
	GPA0CON |= 0x22;

	//8n1
	ULCON0 &= 0x3;;

	//polling mode
	UCON0 &= 0x5;

	//set baud-rate
	UBRDIV0 = 53;
	UFRACVAL0= 4;

	printu("*************************************\r");	
	printu("***   welcome to frank's A9      ****\r");	
	printu("***                   2015-10-22 ****\r");	
	printu("*************************************\r");	

	unsigned char ch;
	unsigned char buffer[128];

	uartCommand_S com[] = {
	{LEDALLON, 		"ledon"},
	{LEDALLOFF,		"ledoff"},
	{BUZZERON, 		"buzzeron"},
	{BUZZEROFF, 	"buzzeroff"},
	{KEY1ON, 		"key1on"},
	{KEY2ON, 		"key2on"},
	{KEY3ON, 		"key3on"},
	{KEY4ON, 		"key4on"},
	{KEY1OFF, 		"key1off"},
	{KEY2OFF, 		"key2off"},
	{KEY3OFF, 		"key3off"},
	{KEY4OFF, 		"key4off"}
};

	CMDTYPE_E type;
	initLed();
	initBuzzer();

	while(1)
	{
		puts("[frank@] # ");
		//ch = getchar();		
		//putchar(ch);
		gets(buffer);
		if (0 == strlen(buffer))
			continue;
		type = findCommand(buffer, com, ARRAYSIZE(com));
		doCmd(type);
	}             	
                	
#endif            	

	return 0;
}
