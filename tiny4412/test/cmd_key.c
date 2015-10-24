#include "cmd_key.h"

#define GPX3CON_ADDR 0x11000C60

void initKey()
{
	struct tiny4412_gpio_bank* psBank = (struct tiny4412_gpio_bank*)GPX3CON_ADDR; 
	gpio_direction_input(psBank, 2, 0);
	gpio_direction_input(psBank, 3, 0);
	gpio_direction_input(psBank, 4, 0);
	gpio_direction_input(psBank, 5, 0);
}


CMDTYPE_E processKey(int gpio)
{
	CMDTYPE_E key=CMDUNKNOWN;

	if (2 == gpio)
		key = processKey1();
	if (3 == gpio)
		key = processKey2();
	if (4 == gpio)
		key = processKey3();
	if (5 == gpio)
		key = processKey4();

	return key;
}

CMDTYPE_E processKey1()
{
	struct tiny4412_gpio_bank* psBank = (struct tiny4412_gpio_bank*)GPX3CON_ADDR; 
	CMDTYPE_E key;

	if (gpio_get_value(psBank, 2))
		key = KEY1OFF;
	else
		key = KEY1ON;
			
	return key;
}

CMDTYPE_E processKey2()
{
	struct tiny4412_gpio_bank* psBank = (struct tiny4412_gpio_bank*)GPX3CON_ADDR; 
	CMDTYPE_E key;

	if (gpio_get_value(psBank, 3))
		key = KEY2OFF;
	else
		key = KEY2ON;
			
	return key;
}

CMDTYPE_E processKey3()
{
	struct tiny4412_gpio_bank* psBank = (struct tiny4412_gpio_bank*)GPX3CON_ADDR; 
	CMDTYPE_E key;

	if (gpio_get_value(psBank, 4))
		key = KEY3OFF;
	else
		key = KEY3ON;
			
	return key;
}

CMDTYPE_E processKey4()
{
	struct tiny4412_gpio_bank* psBank = (struct tiny4412_gpio_bank*)GPX3CON_ADDR; 
	CMDTYPE_E key;

	if (gpio_get_value(psBank, 5))
		key = KEY4OFF;
	else
		key = KEY4ON;
			
	return key;
}
