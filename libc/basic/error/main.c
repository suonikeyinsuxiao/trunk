/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       main.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-09-21
*   
***********************************************************************/
#include "pl_error.h"
#include <stdio.h>


int main(int argc, char** argv)
{
	printf("1=%s\n",get_error_desc(-1));
	printf("2=%s\n",get_error_desc(10));
	printf("3=%s\n",get_error_desc(50));
	return 0;
}
