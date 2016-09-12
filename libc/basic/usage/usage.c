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
#include <unistd.h>
#include <getopt.h>

static void help(void)
{
	int k;
	printf(
"Usage: pcm [OPTION]... [FILE]...\n"
"-h,--help	help\n"
"-D,--device	playback device\n"
"-r,--rate	stream rate in Hz\n"
"-c,--channels	count of channels in stream\n"
"-f,--frequency	sine wave frequency in Hz\n"
"-b,--buffer	ring buffer size in us\n"
"-p,--period	period size in us\n"
"-m,--method	transfer method\n"
"-o,--format	sample format\n"
"-v,--verbose   show the PCM setup parameters\n"
"-n,--noresample  do not resample\n"
"-e,--pevent    enable poll event after each period\n"
"\n");
}

int main(int argc, char** argv)
{
	struct option long_option[] =
	{
		{"help", 0, NULL, 'h'},
		{"device", 1, NULL, 'D'},
		{"rate", 1, NULL, 'r'},
		{"channels", 1, NULL, 'c'},
		{"frequency", 1, NULL, 'f'},
		{"buffer", 1, NULL, 'b'},
		{"period", 1, NULL, 'p'},
		{"method", 1, NULL, 'm'},
		{"format", 1, NULL, 'o'},
		{"verbose", 1, NULL, 'v'},
		{"noresample", 1, NULL, 'n'},
		{"pevent", 1, NULL, 'e'},
		{NULL, 0, NULL, 0},
	};

	while (1) 
	{
		int c;
		if ((c = getopt_long(argc, argv, "hD:r:c:f:b:p:m:o:vne", long_option, NULL)) < 0)
			break;
		switch (c) 
		{
			case 'h':
				printf("h\n");	
				break;
			case 'D':
				printf("D\n");	
				break;
			case 'r':
				printf("r\n");	
				break;
			case 'c':
				printf("c\n");	
				break;
			case 'f':
				printf("f\n");	
				break;
			case 'b':
				printf("b\n");	
				break;
			case 'p':
				printf("p\n");	
				break;
			case 'm':
				printf("m\n");	
				break;
			case 'o':
				printf("o\n");	
				break;
			case 'v':
				printf("v\n");	
				break;
			case 'n':
				printf("n\n");	
				break;
			case 'e':
				printf("e\n");	
				break;
		}
	}

	return 0;
}

#if 0
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
#endif
