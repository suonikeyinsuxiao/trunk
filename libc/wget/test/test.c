/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       test.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-11-28
*   
***********************************************************************/
//#include "test.h"
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long uintptr_t;

#define INVALID_PTR ((void *) ~(uintptr_t) 0)
#define CELL_OCCUPIED(key) ((key) != INVALID_PTR)
#define countof(x) (sizeof (x) / sizeof ((x)[0]))

struct cell {
	void *key;
	void *value;
};

unsigned long
hash_string (const void *key)
{
	const char *p = key;
	unsigned int h = *p;

	if (h)
	{
		printf("h=%u\n", h);
		for (p += 1; *p != '\0'; p++)
		{
			h = (h << 5) - h + *p;
			printf("h=%u\n\n", h);
		}
	}

	return h;
}

unsigned long
hash_pointer (const void *ptr)
{
	uintptr_t key = (uintptr_t) ptr;
	key += (key << 12);
	key ^= (key >> 22);
	key += (key << 4);
	key ^= (key >> 9);
	key += (key << 10);
	key ^= (key >> 2);
	key += (key << 7);
	key ^= (key >> 12);
#if SIZEOF_VOID_P > 4
	key += (key << 44);
	key ^= (key >> 54);
	key += (key << 36);
	key ^= (key >> 41);
	key += (key << 42);
	key ^= (key >> 34);
	key += (key << 39);
	key ^= (key >> 44);
#endif
	return (unsigned long) key;
}

static int
prime_size (int size, int *prime_offset)
{
	static const int primes[] = {
		13, 19, 29, 41, 59, 79, 107, 149, 197, 263, 347, 457, 599, 787, 1031,
		1361, 1777, 2333, 3037, 3967, 5167, 6719, 8737, 11369, 14783,
		19219, 24989, 32491, 42257, 54941, 71429, 92861, 120721, 156941,
		204047, 265271, 344857, 448321, 582821, 757693, 985003, 1280519,
		1664681, 2164111, 2813353, 3657361, 4754591, 6180989, 8035301,
		10445899, 13579681, 17653589, 22949669, 29834603, 38784989,
		50420551, 65546729, 85210757, 110774011, 144006217, 187208107,
		243370577, 316381771, 411296309, 534685237, 695090819, 903618083,
		1174703521, 1527114613, 1837299131, 2147483647
	};
	size_t i;

	for (i = *prime_offset; i < countof (primes); i++)
		if (primes[i] >= size)
		{
			/* Set the offset to the next prime.  That is safe because,
			   next time we are called, it will be with a larger SIZE,
			   which means we could never return the same prime anyway.
			   (If that is not the case, the caller can simply reset
			 *prime_offset.)  */
			*prime_offset = i + 1;
			printf("*prime_offset=%d, primes[%d]=%d\n", *prime_offset, i, primes[i]);
			return primes[i];
		}

	abort ();
}

int main(int argc, char** argv)
{
	char* pcStr = "hello";
	int size = 1;
	int prime_offset = 0;

	printf("hash_string=%lu, y=%d\n", hash_string(pcStr), hash_string(pcStr)%13);

	printf("hash_string=%lu\n", hash_pointer(pcStr));

	printf("%d\n", CELL_OCCUPIED(NULL));
	printf("%d\n", CELL_OCCUPIED(0));
	printf("%d\n", CELL_OCCUPIED(pcStr));
	printf("%x\n", INVALID_PTR);
	printf("%x\n", -1);

	printf("sizeof(struct cell)=%d\n",sizeof(struct cell));

	prime_size(size, &prime_offset);

	return 0;
}
