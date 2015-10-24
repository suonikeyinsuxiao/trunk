#include"encoder.h"

int initEncoder(ENCODER_S* pcEncoder,int nColorSpaceType,  int nWidth, int nHeight)
{
	if (NULL == pcEncoder)	
	{
		printf("[%s,%d] pcEncoder == NULL\n", __func__, __LINE__);	
		return -1;
	}

	pcEncoder->m_psParam = (x264_param_t*)malloc(sizeof(x264_param_t));
	if (NULL == pcEncoder->m_psParam)		
	{
		fprintf(stderr, "[%s,%d], malloc x264_param_t failed:%s\n", __func__, __LINE__, strerror(errno));		
		return -1;
	}

	x264_param_default(pcEncoder->m_psParam);

	pcEncoder->m_psParam->i_width = nWidth;
	pcEncoder->m_psParam->i_height = nHeight;
	pcEncoder->m_psParam->i_csp = nColorSpaceType;
	pcEncoder->m_psParam->i_fps_den = 1;
	pcEncoder->m_psParam->i_fps_num = 25;
	
	if (0 != x264_param_apply_profile(pcEncoder->m_psParam,x264_profile_names[5]))
	{
		fprintf(stderr, "[%s,%d] x264_param_apply_profile failed:%s\n", __func__, __LINE__, strerror(errno));	
		unInitEncoder(pcEncoder);
		//if (NULL != pcEncoder->m_psParam)
		//{
		//	unInitEncoder(pcEncoder->m_psParam);		
		//	pcEncoder->m_psParam = NULL;
		//}
		return -1;
	}

	pcEncoder->m_psHandle = x264_encoder_open(pcEncoder->m_psParam);
	if (NULL == pcEncoder->m_psHandle)
	{
		fprintf(stderr, "[%s,%d]x264_encoder_open failed:%s\n", __func__, __LINE__,strerror(errno));		
		unInitEncoder(pcEncoder);
		return -1;
	}
	pcEncoder->m_cIsEncodeOpen = 1;

	pcEncoder->m_psPic = (x264_picture_t*)malloc(sizeof(x264_picture_t));
	if (NULL == pcEncoder->m_psPic)
	{
		fprintf(stderr, "[%s,%d] malloc x264_picture_t failed:%s", __func__, __LINE__, strerror(errno));		
		unInitEncoder(pcEncoder);
		return -1;
	}
	pcEncoder->m_cIsPicAlloc = 1;

	x264_picture_init(pcEncoder->m_psPic);
	if (0 != x264_picture_alloc(pcEncoder->m_psPic, nColorSpaceType, nWidth, nHeight))
	{
		fprintf(stderr, "[%s,%d] x264_picture_alloc failed:%s", __func__, __LINE__, strerror(errno));		
		unInitEncoder(pcEncoder);
		return -1;
	}

	pcEncoder->m_psPic->img.i_plane = 3;
	return 0;
}

void unInitEncoder(ENCODER_S* pcEncoder)
{
	if (NULL == pcEncoder)	
	{
		printf("[%s,%d] pcEncoder == NULL\n", __func__, __LINE__);	
		return ;
	}

	if (NULL != pcEncoder->m_psPic)
	{
		if (pcEncoder->m_cIsPicAlloc)
		{
			x264_picture_clean(pcEncoder->m_psPic);	
			pcEncoder->m_cIsPicAlloc = 0;
		}
		
		free(pcEncoder->m_psPic);		
		pcEncoder->m_psPic = NULL;
	}

	if (NULL != pcEncoder->m_psParam)
	{
		free(pcEncoder->m_psParam);		
		pcEncoder->m_psParam = NULL;
	}

//printf("789, pcEncoder=%p, pcEncoder->m_psHandle=%p\n",pcEncoder,pcEncoder->m_psHandle);

	if (NULL != pcEncoder->m_psHandle && pcEncoder->m_cIsEncodeOpen == 1)
	{
		x264_encoder_close(pcEncoder->m_psHandle);
		pcEncoder->m_psHandle = NULL;
		pcEncoder->m_cIsEncodeOpen = 0;
	}

}

//pcSrcFrame 为420格式的视频帧
int encodeAFrame(ENCODER_S* pcEncoder, char* pcSrcFrame)
{
	if (NULL == pcEncoder)	
	{
		printf("[%s,%d] pcEncoder == NULL\n", __func__, __LINE__);	
		return -1;
	}

	if (NULL == pcSrcFrame)
	{
		printf("[%s,%d] pcSrcFrame == NULL\n", __func__, __LINE__);		
		return -1;
	}

//	printf("x22222222222222222222222\n");	
	int i = 0;
	x264_picture_t picOut;
	int nWidth = pcEncoder->m_psParam->i_width;
	int nHeight = pcEncoder->m_psParam->i_height;
	int nSize = nWidth*nHeight;

	//yuyv2yuv420(pcSrcFrame, );
	switch(pcEncoder->m_psParam->i_csp)
	{
		case X264_CSP_I422:
		{
//			printf("X264_CSP_I422\n");
			memcpy(pcEncoder->m_psPic->img.plane[0], pcSrcFrame, nWidth*nHeight);		
			memcpy(pcEncoder->m_psPic->img.plane[1], pcSrcFrame+nSize, nWidth*nHeight/2);		
			memcpy(pcEncoder->m_psPic->img.plane[2], pcSrcFrame+nSize+nSize/2, nWidth*nHeight/2);		
		}
		break;
		case X264_CSP_I420:
		{
//			printf("X264_CSP_I420\n");
			memcpy(pcEncoder->m_psPic->img.plane[0], pcSrcFrame, nWidth*nHeight);		
			memcpy(pcEncoder->m_psPic->img.plane[1], pcSrcFrame+nSize, nSize/4);		
			memcpy(pcEncoder->m_psPic->img.plane[2], pcSrcFrame+nSize+nSize/4, nSize/4);		
		}
		break;
		default:
		{
			printf("[%s,%d] colorspace not support",__func__, __LINE__);	
			unInitEncoder(pcEncoder);
			return -1;
		}
	}
	pcEncoder->m_psPic->i_pts = pcEncoder->m_nPTS;
	//pcEncoder->m_psPic->i_type = X264_TYPE_AUTO;

	if (0 > x264_encoder_encode(pcEncoder->m_psHandle, &(pcEncoder->m_psNal), &(pcEncoder->m_nNal), pcEncoder->m_psPic, &picOut))	
	{
		fprintf(stderr, "[%s, %d] x264_encoder_encode failed:%s", __func__, __LINE__, strerror(errno));		
		unInitEncoder(pcEncoder);
		return -1;
	}
	printf("nNal=%d\n",pcEncoder->m_nNal);


	if (-1 == pcEncoder->m_nFd)
	{
		printf("pcEncoder->m_nFd == -1\n");	
		return -1;
	}

	for (i = 0; i < pcEncoder->m_nNal; ++i)
	{
//		printf("pcEncoder->m_psNal[%d].i_payload=%d\n",i,pcEncoder->m_psNal[i].i_payload);

		if (-1 == write(pcEncoder->m_nFd, pcEncoder->m_psNal[i].p_payload, pcEncoder->m_psNal[i].i_payload))
		{
			fprintf(stderr, "write error:%s\n", strerror(errno));		
			return -1;
		}
	}

	return 0;
}

void flushEncoder(ENCODER_S* pcEncoder)
{
	x264_picture_t picOut;
	int i = 0;
	int nRet = 0;

	while(1)
	{
		nRet = x264_encoder_encode(pcEncoder->m_psHandle, &pcEncoder->m_psNal, &(pcEncoder->m_nNal), NULL, &picOut);
		if (0 > nRet)
		{
			fprintf(stderr, "[%s, %d] flush encoder x264_encoder_encode failed:%s", __func__, __LINE__, strerror(errno));		
			unInitEncoder(pcEncoder);
			return ;
		}
		if (0 == nRet)
			break;

		
        for (i = 0; i < pcEncoder->m_nNal; ++i)
		{
			if (-1 == pcEncoder->m_nFd)
			{
				printf("pcEncoder->m_nFd == -1\n");	
				return;
			}
			write(pcEncoder->m_nFd, pcEncoder->m_psNal[i].p_payload, pcEncoder->m_psNal[i].i_payload);
        }
	}

}
