/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       test.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-12-09
*   
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>

#define LH_PRIME 0x9e370001UL
#define ULONG_MAX   (~0UL)

unsigned long lh_ptr_hash(const void *k)
{
	/* CAW: refactored to be 64bit nice */
	return (unsigned long)((((unsigned int)k * LH_PRIME) >> 4) & ULONG_MAX);
}

unsigned long lh_char_hash(const void *k)
{
	unsigned int h = 0;
	const char* data = (const char*)k;

	while( *data!=0 ) h = h*129 + (unsigned int)(*data++) + LH_PRIME;

	return h;
}                                                                 

int main(int argc, char** argv)
{
	char* p = "hello";
	printf("%lu\n", lh_char_hash(argv[1]));
	printf("%lu\n", lh_ptr_hash(argv[1]));

	return 0;
}
