/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       morphing.h
*   @brief:      voice morphing 
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-06
*   
***********************************************************************/
#ifndef _MORPHING_H
#define _MORPHING_H
#ifdef __cplusplus
extern "C"
{
#endif
#include	"messagequeue.h"

#define WINSIZE 256
#define PI 3.1415926
#define SAMPLENUM 256
#define NP 6
#define MP 4
#define STEP  170 //(SAMPLENUM / (NP / MP))
#define UPSAMPLE 1024 //SAMPLENUM*MP

#define FLOOR(x) ((int)floor(x)) //These functions return the largest integral value that is not greater than x


/**
 * @brief    morph256: voice morphing
 *
 * @param[in]    pdAudRecord: voice source
 * @param[in]    nDataLen:    voice data len
 * @param[out]   pdOut:		  morphing voice data
 * @param[in]    nOutLen:     data len
 */
int morph256(MSG_S* psInputMsg, MSG_S* psOutputMsg);
//void morph256(double* pdAudRecord, int nDataLen, double* pdOut, int nOutLen);

#ifdef __cplusplus
}
#endif
#endif // _MORPHING_H
