#include "fft.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
//精度0.000001弧度

void conjugateComplex(int n,complex_S in[],complex_S out[])
{
  int i = 0;
  for(i=0;i<n;i++)
  {
    out[i].m_dVirt = -in[i].m_dVirt;
    out[i].m_dReal = in[i].m_dReal;
  }	
}

void absComplex(complex_S f[],double out[],int n)
{
  int i = 0;
  double t;
  for(i=0;i<n;i++)
  {
    t = f[i].m_dReal * f[i].m_dReal + f[i].m_dVirt * f[i].m_dVirt;
    out[i] = sqrt(t);
  }	
}

void plusComplex(complex_S a,complex_S b,complex_S *c)
{
  c->m_dReal = a.m_dReal + b.m_dReal;
  c->m_dVirt = a.m_dVirt + b.m_dVirt;
}

void subComplex(complex_S a,complex_S b,complex_S *c)
{
  c->m_dReal = a.m_dReal - b.m_dReal;
  c->m_dVirt = a.m_dVirt - b.m_dVirt;	
}

void mulComplex(complex_S a,complex_S b,complex_S *c)
{
  c->m_dReal = a.m_dReal * b.m_dReal - a.m_dVirt * b.m_dVirt;
  c->m_dVirt = a.m_dReal * b.m_dVirt + a.m_dVirt * b.m_dReal;	
}

void divComplex(complex_S a,complex_S b,complex_S *c)
{
  c->m_dReal = (a.m_dReal * b.m_dReal + a.m_dVirt * b.m_dVirt)/(b.m_dReal * b.m_dReal +b.m_dVirt * b.m_dVirt);
  c->m_dVirt = (a.m_dVirt * b.m_dReal - a.m_dReal * b.m_dVirt)/(b.m_dReal * b.m_dReal +b.m_dVirt * b.m_dVirt);
}

void printComplex(complex_S* pDst, int n)
{
	int i = 0;
	for (i = 0; i < n; i++)
	{
		printf("pDst[%d].m_dReal=%f,pDst[%d].m_dVirt=%f\n", 
				i, pDst[i].m_dReal, i, pDst[i].m_dVirt);	
	}
}

static void Wn_i(int n,int i,complex_S *Wn,char flag)
{
  Wn->m_dReal = cos(2*PI*i/n);
  if(flag == 1)
  Wn->m_dVirt = -sin(2*PI*i/n);
  else if(flag == 0)
  Wn->m_dVirt = -sin(2*PI*i/n);
}

//傅里叶变化
void fft(int N,complex_S f[])
{
  complex_S t,wn;//中间变量
  int i,j,k,m,n,l,r,M;
  int la,lb,lc;
  /*----计算分解的级数M=log2(N)----*/
  for(i=N,M=1;(i=i/2)!=1;M++); 
  /*----按照倒位序重新排列原信号----*/
  for(i=1,j=N/2;i<=N-2;i++)
  {
    if(i<j)
    {
      t=f[j];
      f[j]=f[i];
      f[i]=t;
    }
    k=N/2;
    while(k<=j)
    {
      j=j-k;
      k=k/2;
    }
    j=j+k;
  }

  /*----FFT算法----*/
  for(m=1;m<=M;m++)
  {
    la=pow(2,m); //la=2^m代表第m级每个分组所含节点数		
    lb=la/2;    //lb代表第m级每个分组所含碟形单元数
                 //同时它也表示每个碟形单元上下节点之间的距离
    /*----碟形运算----*/
    for(l=1;l<=lb;l++)
    {
      r=(l-1)*pow(2,M-m);	
      for(n=l-1;n<N-1;n=n+la) //遍历每个分组，分组总数为N/la
      {
        lc=n+lb;  //n,lc分别代表一个碟形单元的上、下节点编号     
        Wn_i(N,r,&wn,1);//wn=Wnr
        mulComplex(f[lc],wn,&t);//t = f[lc] * wn复数运算
        subComplex(f[n],t,&(f[lc]));//f[lc] = f[n] - f[lc] * Wnr
        plusComplex(f[n],t,&(f[n]));//f[n] = f[n] + f[lc] * Wnr
      }
    }
  }
}

//傅里叶逆变换
void ifft(int N,complex_S f[])
{
  int i=0;
  conjugateComplex(N,f,f);
  fft(N,f);
  conjugateComplex(N,f,f);
  for(i=0;i<N;i++)
  {
    f[i].m_dVirt = (f[i].m_dVirt)/N;
    f[i].m_dReal = (f[i].m_dReal)/N;
  }
}

int createComplex(double* pSrc, complex_S** pDst, int size)
{
	int i = 0;

	*pDst = malloc(sizeof(complex_S)*size);
	if (NULL == *pDst)
	{
		printf("malloc complex failed\n");
		return -1;
	}

	//printf("xxxxxxxxxxxxx, *pDst=%p\n", *pDst);

	for (i = 0; i < size; i++)
	{
		(*(*pDst+i)).m_dReal = *(pSrc+i);	
		(*(*pDst+i)).m_dVirt = 0.0;
	}
}

void destroyComplex(complex_S** pDst)
{
	free(*pDst);	
	*pDst = NULL;
}

void getFftAbs(double* pSrc, int size, double* pDst)
{
	int i = 0;

	double* pTmp = malloc(sizeof(double)*size);
	complex_S* pCm = NULL;

	createComplex(pSrc, &pCm, size);
	fft(size, pCm);

	absComplex(pCm, pDst, size);

	free(pTmp);
	destroyComplex(&pCm);
}
