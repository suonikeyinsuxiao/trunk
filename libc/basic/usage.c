/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       usage.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-02-03
*   
***********************************************************************/
//#include "usage.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("usage: %s xxx\n",argv[0]);	
		return -1;
	}

	printf("w:%d\n", atoi(argv[1]));
	printf("h:%d\n", atoi(argv[2]));

	return 0;
}
