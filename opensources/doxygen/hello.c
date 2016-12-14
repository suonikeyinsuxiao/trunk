/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       hello.c
*   @brief:      根据实际情况填写
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-01-25
*   
***********************************************************************/
#include "hello.h"
#include <stdio.h>

/**
 * @brief    print 
 *
 * @param[in]    pcStr
 */
void print(char* pcStr)
{
	printf("string:%s\n", pcStr);
}

/**
 * @brief    sub 
 *
 * @param[in]    a
 * @param[in]    b
 *
 * @return    
 */
int sub(int a, int b)
{
	return (a-b);
}

/**
 * @brief    add 
 *
 * @param[in]    a
 * @param[in]    b
 *
 * @return    
 */
int add(int a, int b)
{
	return (a+b);
}


