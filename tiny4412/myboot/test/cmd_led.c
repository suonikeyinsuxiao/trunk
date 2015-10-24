//#include "hardware.h"
#include "cmd_led.h"

#define GPM4CON_ADDR 0x110002E0

void initLed(void)
{
	struct tiny4412_gpio_bank* psBank = (struct tiny4412_gpio_bank*)GPM4CON_ADDR; 
	gpio_direction_output(psBank, 0, 1);//GPM4:0 output
	gpio_direction_output(psBank, 1, 1);//GPM4:1 output
	gpio_direction_output(psBank, 2, 1);//GPM4:2 output
	gpio_direction_output(psBank, 3, 1);//GPM4:3 output

}

void ledAllOn(void)
{
	struct tiny4412_gpio_bank* psBank = (struct tiny4412_gpio_bank*)GPM4CON_ADDR; 
	gpio_set_value(psBank, 0 ,0);
	gpio_set_value(psBank, 1 ,0);
	gpio_set_value(psBank, 2 ,0);
	gpio_set_value(psBank, 3 ,0);
}


void ledAllOff(void)
{
	struct tiny4412_gpio_bank* psBank = (struct tiny4412_gpio_bank*)GPM4CON_ADDR; 
	gpio_set_value(psBank, 0 ,1);
	gpio_set_value(psBank, 1 ,1);
	gpio_set_value(psBank, 2 ,1);
	gpio_set_value(psBank, 3 ,1);
}
