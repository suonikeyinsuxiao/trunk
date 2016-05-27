/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       morphing.c
*   @brief:      voice morphing
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-05-06
*   
***********************************************************************/
#include "morphing.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef PL_TAG
#define PL_TAG "WAVE"
#endif

/*data from matlab*/
static double adHanningWindow[WINSIZE] = 
{
	0.0001494,
	0.0005975,
	0.0013442,
	0.0023889,
	0.0037310,
	0.0053697,
	0.0073041,
	0.0095329,
	0.0120549,
	0.0148685,
	0.0179721,
	0.0213638,
	0.0250415,
	0.0290031,
	0.0332463,
	0.0377684,
	0.0425667,
	0.0476385,
	0.0529806,
	0.0585899,
	0.0644631,
	0.0705965,
	0.0769866,
	0.0836295,
	0.0905213,
	0.0976579,
	0.1050349,
	0.1126479,
	0.1204925,
	0.1285639,
	0.1368573,
	0.1453677,
	0.1540902,
	0.1630193,
	0.1721499,
	0.1814764,
	0.1909933,
	0.2006948,
	0.2105753,
	0.2206287,
	0.2308492,
	0.2412305,
	0.2517664,
	0.2624507,
	0.2732770,
	0.2842388,
	0.2953296,
	0.3065427,
	0.3178714,
	0.3293090,
	0.3408486,
	0.3524833,
	0.3642062,
	0.3760102,
	0.3878884,
	0.3998336,
	0.4118386,
	0.4238963,
	0.4359995,
	0.4481410,
	0.4603134,
	0.4725096,
	0.4847222,
	0.4969439,
	0.5091675,
	0.5213856,
	0.5335909,
	0.5457761,
	0.5579340,
	0.5700572,
	0.5821386,
	0.5941709,
	0.6061469,
	0.6180594,
	0.6299014,
	0.6416657,
	0.6533454,
	0.6649334,
	0.6764229,
	0.6878069,
	0.6990786,
	0.7102314,
	0.7212585,
	0.7321534,
	0.7429095,
	0.7535204,
	0.7639798,
	0.7742814,
	0.7844191,
	0.7943868,
	0.8041786,
	0.8137885,
	0.8232109,
	0.8324402,
	0.8414707,
	0.8502971,
	0.8589142,
	0.8673167,
	0.8754998,
	0.8834583,
	0.8911877,
	0.8986833,
	0.9059406,
	0.9129553,
	0.9197232,
	0.9262402,
	0.9325024,
	0.9385062,
	0.9442478,
	0.9497239,
	0.9549313,
	0.9598667,
	0.9645273,
	0.9689102,
	0.9730129,
	0.9768329,
	0.9803679,
	0.9836157,
	0.9865745,
	0.9892425,
	0.9916181,
	0.9936999,
	0.9954865,
	0.9969771,
	0.9981706,
	0.9990663,
	0.9996638,
	0.9999626,
	0.9999626,
	0.9996638,
	0.9990663,
	0.9981706,
	0.9969771,
	0.9954865,
	0.9936999,
	0.9916181,
	0.9892425,
	0.9865745,
	0.9836157,
	0.9803679,
	0.9768329,
	0.9730129,
	0.9689102,
	0.9645273,
	0.9598667,
	0.9549313,
	0.9497239,
	0.9442478,
	0.9385062,
	0.9325024,
	0.9262402,
	0.9197232,
	0.9129553,
	0.9059406,
	0.8986833,
	0.8911877,
	0.8834583,
	0.8754998,
	0.8673167,
	0.8589142,
	0.8502971,
	0.8414707,
	0.8324402,
	0.8232109,
	0.8137885,
	0.8041786,
	0.7943868,
	0.7844191,
	0.7742814,
	0.7639798,
	0.7535204,
	0.7429095,
	0.7321534,
	0.7212585,
	0.7102314,
	0.6990786,
	0.6878069,
	0.6764229,
	0.6649334,
	0.6533454,
	0.6416657,
	0.6299014,
	0.6180594,
	0.6061469,
	0.5941709,
	0.5821386,
	0.5700572,
	0.5579340,
	0.5457761,
	0.5335909,
	0.5213856,
	0.5091675,
	0.4969439,
	0.4847222,
	0.4725096,
	0.4603134,
	0.4481410,
	0.4359995,
	0.4238963,
	0.4118386,
	0.3998336,
	0.3878884,
	0.3760102,
	0.3642062,
	0.3524833,
	0.3408486,
	0.3293090,
	0.3178714,
	0.3065427,
	0.2953296,
	0.2842388,
	0.2732770,
	0.2624507,
	0.2517664,
	0.2412305,
	0.2308492,
	0.2206287,
	0.2105753,
	0.2006948,
	0.1909933,
	0.1814764,
	0.1721499,
	0.1630193,
	0.1540902,
	0.1453677,
	0.1368573,
	0.1285639,
	0.1204925,
	0.1126479,
	0.1050349,
	0.0976579,
	0.0905213,
	0.0836295,
	0.0769866,
	0.0705965,
	0.0644631,
	0.0585899,
	0.0529806,
	0.0476385,
	0.0425667,
	0.0377684,
	0.0332463,
	0.0290031,
	0.0250415,
	0.0213638,
	0.0179721,
	0.0148685,
	0.0120549,
	0.0095329,
	0.0073041,
	0.0053697,
	0.0037310,
	0.0023889,
	0.0013442,
	0.0005975,
	0.0001494
};

#if 0
int hammingWin(unsigned int N, double **w)
{
	unsigned int n;
	double *ret = NULL;
	ret = (double *)malloc(N * sizeof(double));
	if(ret == NULL)
		return -1;
	
	for(n = 0; n < N; n++)
	{
		*(ret + n) = 0.54 - 0.46 * cos (2 * PI *  ( double )n / ( N - 1 ) );
	}
	
	*w = ret;
	
	return 0;
}

int hannWin(unsigned int N, double **w)
{
	unsigned int n;
	double *ret;
	ret = (double *)malloc(N * sizeof(double));
	if(ret == NULL)
		return -1;
	
	for(n = 0; n < N; n++)
	{
		*(ret + n) = 0.5 * ( 1 - cos( 2 * PI * (double)n/(N - 1)));
	}
	
	*w = ret;
	
	return 0;
}
#endif

static double* resample(double* pdIn, int nInLen, int mP, int nP) 
{
	if (NULL == pdIn)
	{
		PL_LOGE(PL_TAG, "NULL == pdIn\n");	
		return NULL;
	}

	int i = 0;
	int j = 0; 
	int nStep = 0;
	double dSum = 0.0;
	double adUpSample[UPSAMPLE] = {0.0};
	double* pdOut = (double*)malloc(FLOOR(SAMPLENUM*MP/NP)*sizeof(double));
	if (NULL == pdOut)
	{
		PL_LOGE(PL_TAG, "malloc failed!\n");	
		return NULL;
	}

	for (i = 0; i < nInLen; i++)
	{
		for (j = 0; j < mP; j++)
		{
			*(adUpSample+i*mP+j) = pdIn[i];
		}
	}

	for (i = 0; i < FLOOR(SAMPLENUM*MP/NP); i++)
	{
		dSum = 0.0;
		nStep = FLOOR(i*nP);
		for (j = 0; j < nP; j++)
		{
			dSum = dSum + *(adUpSample+i*nP+j);
		}
		pdOut[i] = dSum/nP;
	}

	return pdOut;
}

/*note: the caller free the pointer return*/
static double* morphInternel(double* pdData, int nDataLen)
{
	if (NULL == pdData)
	{
		PL_LOGE(PL_TAG, "NULL == pdData\n");	
		return NULL;
	}

	int i = 0;
	double adSeg[SAMPLENUM] = {0.0};
	double* pdResampleSeg = NULL;
	int nResampleSegLen = FLOOR(SAMPLENUM*MP/NP);

	double* pdOut = (double*)malloc(nDataLen * sizeof(double));
	if (NULL == pdOut)
	{
		PL_LOGE(PL_TAG, "malloc failed!\n");	
		return NULL;
	}
	memset(pdOut, 0, nDataLen*sizeof(double));

	for (i = 0; i < SAMPLENUM; i++)
		adSeg[i] = pdData[i] * adHanningWindow[i];

	pdResampleSeg = resample(adSeg, SAMPLENUM, MP, NP);

	//for (i = 0; i < nResampleSegLen; i++)
	//{
	//	pdOut[i] = pdResampleSeg[i];
	//}
	memcpy(pdOut, pdResampleSeg, nResampleSegLen*sizeof(double));
	free(pdResampleSeg);
	pdResampleSeg = NULL;

	return pdOut;
}

static double adInputData[SAMPLENUM] = {0};
static double adOutputData[SAMPLENUM] = {0};

int morph256(MSG_S* psInputMsg, MSG_S* psOutputMsg)
{
	if (NULL == psInputMsg || NULL == psOutputMsg)
	{
		PL_LOGE(PL_TAG, "NULL == psInputMsg || NULL == psOutputMsg\n");	
		return -1;
	}

	unsigned int nIndex = 0;
	double adSeg[SAMPLENUM] = {0.0};
	double adTmp[SAMPLENUM] = {0.0};
	double* pdMorphInternel = NULL;
	int i = 0;
		
	memcpy(adSeg, psInputMsg->m_pMsgData, psInputMsg->m_nMsgLen*sizeof(double));	
	//for (i = 0; i < SAMPLENUM; i++)
	//{
	//	PL_LOGD(PL_TAG, "adSeg[%d]=%f\n",i, adSeg[i]);	
	//}

	pdMorphInternel = morphInternel(adSeg, SAMPLENUM);
	if (NULL == pdMorphInternel)
	{
		PL_LOGE(PL_TAG, "morphInternel failed!\n");
		return -1;
	}

	//for (i = 0; i < SAMPLENUM; i++)
	//{
	//	printf("pdMorphInternel[%d]=%lf\n",i,pdMorphInternel[i]);	
	//}

	for (i = 0; i < SAMPLENUM; i++)
	{
		adTmp[i] += adOutputData[i];	
	}
	memset(adOutputData, 0, SAMPLENUM*sizeof(double));

	for (i = 0; i < (SAMPLENUM-(STEP>>1)); i++)
		adOutputData[i] = adTmp[(STEP>>1)+i] + pdMorphInternel[i];

	for (i = (SAMPLENUM-(STEP>>1)); i < SAMPLENUM; i++)
		adOutputData[i] = pdMorphInternel[i]; 
	

	memcpy(psOutputMsg->m_pMsgData, adOutputData, psInputMsg->m_nMsgLen*sizeof(double));	
	free(pdMorphInternel);
	pdMorphInternel = NULL;

	return 0;
}

#if 0
void morph256(double* pdAudRecord, int nDataLen, double* pdOut, int nOutLen)
{
	if (NULL == pdAudRecord || NULL == pdOut)
	{
		PL_LOGE(PL_TAG, "NULL == pdAudRecord || NULL == pdOut\n");	
		return ;
	}

	unsigned int nIndex = 0;
	double adSeg[SAMPLENUM] = {0.0};
	double* pdMorphInternel = NULL;
	int i = 0;
	

	while(nIndex < (nDataLen-(SAMPLENUM << 1)))
	{
		memcpy(adSeg, pdAudRecord+nIndex, SAMPLENUM*sizeof(double));	
		pdMorphInternel = morphInternel(adSeg, SAMPLENUM);
		if (NULL == pdMorphInternel)
		{
			PL_LOGE(PL_TAG, "morphInternel failed!\n");
			break;
		}

		for (i = 0; i < SAMPLENUM; i++)
		{
			*(pdOut + nIndex + i) += pdMorphInternel[i];	
		}
	
		nIndex += STEP >> 1;
		free(pdMorphInternel);
		pdMorphInternel = NULL;
	}
}
#endif


