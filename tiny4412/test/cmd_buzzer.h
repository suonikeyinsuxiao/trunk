#ifndef __BUZZER_H
#define __BUZZER_H
#include "common.h"

#define GPD0CON_ADDR (0x114000A0)//buzzer
void initBuzzer(void);
void buzzerOn(void);
void buzzerOff(void);

#endif
