#ifndef __CMD_KEY_H
#define __CMD_KEY_H
#include "cmd.h"

void initKey();
CMDTYPE_E processKey(int gpio);
CMDTYPE_E processKey1();
CMDTYPE_E processKey2();
CMDTYPE_E processKey3();
CMDTYPE_E processKey4();

#endif //__CMD_KEY_H
