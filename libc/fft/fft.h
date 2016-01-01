#ifndef __FFT_H__
#define __FFT_H__

typedef struct complex_S //复数类型
{
  double m_dReal;		//实部
  double m_dVirt;		//虚部
}complex_S;

#ifndef PI
#define PI 3.1415926
#endif

int  createComplex(double* pSrc, complex_S** pDst, int size);
void destroyComplex(complex_S** pDst);
void printComplex(complex_S* pDst, int n);
void conjugateComplex(int n,complex_S in[],complex_S out[]);//共轭复数
void plusComplex(complex_S a,complex_S b,complex_S *c);//复数加
void mulComplex(complex_S a,complex_S b,complex_S *c) ;//复数乘
void subComplex(complex_S a,complex_S b,complex_S *c);	//复数减法
void divComplex(complex_S a,complex_S b,complex_S *c);	//复数除法
void absComplex(complex_S f[],double out[],int n);//复数数组取模
void fft(int N,complex_S f[]);//傅立叶变换 输出也存在数组f中
void ifft(int N,complex_S f[]); // 傅里叶逆变换
//pSrc: input datas
//size: data length
//pDst: output datas after fft and abs
void getFftAbs(double* pSrc, int size, double* pDst);

#endif
