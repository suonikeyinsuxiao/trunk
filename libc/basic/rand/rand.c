/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       rand.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-08-31
*   
***********************************************************************/
//#include "rand.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief    myrand 
 *
 * @param[in]    range
 *
 * @return   random number 
 */
int myrand(int range)
{
	srand((unsigned int)time(NULL));
	return (rand()%range);
}

int main(int argc, char** argv)
{
	printf("rand=%d\n", myrand(100));

	return 0;
}
