#include "cmd_buzzer.h"

void initBuzzer(void)
{
	struct tiny4412_gpio_bank* psBank = (struct tiny4412_gpio_bank*)GPD0CON_ADDR; 
	gpio_direction_output(psBank, 0, 0);//GPM4:0 output
}
void buzzerOn(void)
{
	struct tiny4412_gpio_bank* psBank = (struct tiny4412_gpio_bank*)GPD0CON_ADDR; 
	gpio_set_value(psBank,0, 1);	
	delay();
}

void buzzerOff(void)
{
	struct tiny4412_gpio_bank* psBank = (struct tiny4412_gpio_bank*)GPD0CON_ADDR; 
	gpio_set_value(psBank,0, 0);	
	delay();
}
