/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       log.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-02-05
*   
***********************************************************************/
#include "log.h"

int main(int argc, char** argv)
{
	char* p = "hello world";
	TLOGE("%s\n", p);
	return 0;
}
