#ifndef __ENCODER_H
#define __ENCODER_H

#include"common.h"
#include<x264.h>

#pragma pack(1)
typedef struct _ENCODER_S
{
	x264_param_t* 	m_psParam;
	x264_t* 		m_psHandle;
	x264_picture_t* m_psPic;
	x264_nal_t*		m_psNal;
	U8				m_cIsEncodeOpen;
	U8				m_cIsPicAlloc;
	U32				m_nNal;
	int 			m_nFd;
	int				m_nPTS;
}ENCODER_S;
#pragma pack()

//void    x264_param_default( x264_param_t * );
//int     x264_param_apply_profile( x264_param_t *, const char *profile );
//x264_t *x264_encoder_open( x264_param_t * );
//int     x264_encoder_encode( x264_t *, x264_nal_t **pp_nal, int *pi_nal, x264_picture_t *pic_in, x264_picture_t *pic_out );
//void    x264_encoder_close  ( x264_t * );

//void 	  x264_picture_init( x264_picture_t *pic );
//int     x264_picture_alloc( x264_picture_t *pic, int i_csp, int i_width, int i_height );
//void    x264_picture_clean( x264_picture_t *pic );

//success return 0, otherwise return -1
int initEncoder(ENCODER_S* pcEncoder,int nCspType, int nWidth, int nHeight);
void unInitEncoder(ENCODER_S* pcEncoder);
int	encodeAFrame(ENCODER_S* pcEncoder, char* pcSrcFrame);
void flushEncoder(ENCODER_S* pcEncoder);


#endif		//__ENCODER_H


