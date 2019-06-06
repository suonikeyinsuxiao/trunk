/**
 * Copyright (c) 2019 Semptian.
 *
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * Semptian, except with written permission of Semptian.
 *
 * @file getopt_long_test.c
 * @brief
 * @author fujun@semptian.com
 * @version 1.0.0
 * @date 2019-06-06 18:15:17
 */

#include <stdio.h>
#include <stdlib.h>


#include <getopt.h>
#include <string.h>

/*
int getopt_long(int argc, char * const argv[],
           const char *optstring,
           const struct option *longopts, int *longindex);
*/

#if 0
int main(int argc, char** argv)
{
    int c;
    int longindex = 0;
    int time = -1;

    struct option long_option[] =
	{
		{"help", 0, NULL, 'h'},
		{"time", 1, &time, 't'},
		{"device", 2, NULL, 'd'},
		{NULL, 0, NULL, 0},
	};

    c = getopt_long(argc, argv, "ht:d:", long_option, &longindex);

    printf("c:%d\n", c);
    printf("optarg:%s\n", optarg);
    printf("optind:%d\n", optind);
    printf("longindex:%d\n", longindex);
    printf("time=%d\n", time);

    return 0;
}
#endif

#if 0
int main(int argc, char** argv)
{
    int ret = -1;
    int longindex = 0;
    int flag = -1;

    struct option long_option[] =
	{
		{"help", 0, NULL, 'h'},
		{"time", 1, &flag, 't'},
		{"device", 2, &flag, 'd'},
		{NULL, 0, NULL, 0},
	};

    ret = getopt_long(argc, argv, "ht:d:", long_option, &longindex);

    printf("flag:%d\n", flag);
    printf("ret:%d\n", ret);
    printf("optarg:%s\n", optarg);
    printf("optind:%d\n", optind);
    printf("longindex:%d\n", longindex);

    return 0;
}
#endif

static void help(void)
{
	printf(
        "Usage: ./usage [OPTION]... [FILE]...\n"
        "-h,--help	help\n"
        "-d,--device	playback device\n"
        "-d,--debug	    debug mode\n"
        "\n");
}

int main(int argc, char** argv)
{
    int ret = -1;
    int longindex = 0;

    struct option long_option[] =
	{
		{"help", 0, NULL, 'h'},
		{"time", 1, NULL, 't'},
		{"device", 2, NULL, 'd'},
		{"debug", 1, NULL, 'd'},
		{NULL, 0, NULL, 0},
	};

    while((ret = getopt_long(argc, argv, "ht:d:", long_option, &longindex)) >= 0)
    {
        switch(ret)
        {
        case 'h':
            help();
            break;
        case 't':
            printf("optind:%d\n", optind);
            printf("longindex:%d\n", longindex);
            printf("time:%s\n", optarg);
            break;
        case 'd':
            printf("optind:%d\n", optind);
            printf("longindex:%d\n", longindex);
            if (0 == strcmp("device", long_option[longindex].name))
            {
                printf("device:%s\n", optarg); 
            }
            else if (0 == strcmp("debug", long_option[longindex].name))
            {
                printf("debug:%s\n", optarg); 
            }
            break;
        default:
            printf("?? getopt returned character code 0x%x ??\n", ret);
            break;
        }
    }

    return 0;
}
