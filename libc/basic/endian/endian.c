/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       endian.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-07
*   
***********************************************************************/
#include "endian.h"
#include <stdio.h> 

#define LITTLE_ENDIAN 1 

int is_little_endian()
{
	long num = 1; 

	if ( 1 == *(unsigned char *) &num ) 
		return 1; 
	else  
		return 0;
}

void bint_to_uchar(unsigned char *j, int x)
{
	unsigned char *ox = (unsigned char *)&x;

#if LITTLE_ENDIAN
	j[0] = ox[3]; j[1] = ox[2]; j[2] = ox[1]; j[3] = ox[0];
#else
	memcpy((void *)j, (void *)ox, sizeof(int));
#endif
}

int uchar_to_bint(const unsigned char *inp)
{
	int o;
	unsigned char *outp = (unsigned char *)&o;
#if LITTLE_ENDIAN
  	outp[0] = inp[3]; outp[1] = inp[2]; outp[2] = inp[1]; outp[3] = inp[0];
#else
  	memcpy((void *)outp, (void *)inp, 4);
#endif
  	return(o);
}
int main(int argc, char *argv[]) 
{ 
	unsigned char acBuf[4] = {0x12,0x34,0x56,0x78};
	printf("sizeof(unsigned char*)=%d\n",sizeof(unsigned char*));

	//bint_to_char(acBuf, 0x1234);
	//printf("acBuf[0]=%x\n", (int)acBuf[0]);
	//printf("acBuf[1]=%x\n", (int)acBuf[1]);
	//printf("acBuf[2]=%x\n", (int)acBuf[2]);
	//printf("acBuf[3]=%x\n", (int)acBuf[3]);

	printf("%x\n",uchar_to_bint(acBuf));
	printf("%x\n",*(int*)acBuf);
	
	return 0; 
}
