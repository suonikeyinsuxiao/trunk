/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       hello.h
*   @brief:      根据实际情况填写
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-01-25
*   
***********************************************************************/
#ifndef _HELLO_H
#define _HELLO_H
#ifdef __cplusplus
extern "C"
{
#endif

/** max number */
#define MAX 100  
/** min  number */
#define MIN 1	

/** print function pointer */
typedef void (*PRINT_F)(char *pcName);

/**
 * @brief   student description 
 */
typedef struct 
{
    char* m_pcName;         /**<student name*/
    int   m_nId;		    /**<student ID*/
}STUDENT_S;

/**
 * @brief   week description 
 */
typedef enum
{
	e_monday, 
	e_tuesday, 
	e_wednesday, 
	e_thursday, 
	e_friday, 
	e_saturday, 
	e_sunday	
}WEEK_E;

/**
 * @brief    print 
 *
 * @param[in]    pcStr
 */
void print(char* pcStr);

/**
 * @brief    sub 
 *
 * @param[in]    a
 * @param[in]    b
 *
 * @return    
 */
int sub(int a, int b);

/**
 * @brief    add 
 *
 * @param[in]    a
 * @param[in]    b
 *
 * @return    
 */
int add(int a, int b);

#ifdef __cplusplus
}
#endif
#endif // _HELLO_H
