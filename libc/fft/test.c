#include <stdio.h>
#include <stdlib.h>
#include "fft.h"

#define TEST_COMPLEX 0
#define TEST_FFT 	 0
#define TEST_FFTABS  1

void changeComplex(complex_S* pDst, int size, double data)
{
	int i = 0;
	for (i = 0; i < size; i++)
	{
		pDst[i].m_dReal -= data;
		pDst[i].m_dVirt -= data;
	}
}

int main()
{
#if TEST_COMPLEX
	int i = 0;
	double aSrc[10] = {1,2,3,4,5,6,7,8,9,10};	
	complex_S* pDst = NULL;
	createComplex(aSrc, &pDst, 10);

	changeComplex(pDst, 10, 2);
	printComplex(pDst, 10);

	printf("test, pDst=%p\n", pDst);
	destroyComplex(&pDst);
	if (NULL == pDst)
		printf("NULL == pDst\n");
#endif
	
#if TEST_FFT
	double aSrc[8] = {1,2,3,4,5,6,7,8};	
	complex_S* pDst = NULL;
	createComplex(aSrc, &pDst, 8);
	int i = 0;

	fft(8,pDst);

	printComplex(pDst, 8);

	destroyComplex(&pDst);
#endif

#if TEST_FFTABS
	double aSrc[8] = {1,2,3,4,5,6,7,8};	
	double aOut[8] = {0};	
	complex_S* pDst = NULL;
	createComplex(aSrc, &pDst, 8);
	int i = 0;

	fft(8,pDst);

	printComplex(pDst, 8);

	absComplex(pDst, aOut, 8);

	for (i = 0; i < 8; i++)
	{
		printf("out[%d]=%f\n", i, aOut[i]);	
	}

	destroyComplex(&pDst);
#endif
	return 0;
}
