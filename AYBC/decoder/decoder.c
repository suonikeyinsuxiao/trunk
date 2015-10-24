#include "decoder.h"

static readBufCB_F pfCB = NULL;

int registerCallBack(readBufCB_F readBuf)
{
	if (NULL == readBuf)
	{
		printf("registerCallBack error NULL == readBuf\n");
		return -1;
	}

	pfCB = readBuf;	

	return 0;
}

int initDecoder(decoderHandle_S* psDecoderHandle)
{
	if (NULL == psDecoderHandle)
	{
		printf("initDecoder NULL == psDecoderHandle\n");		
		return -1;
	}

	printf("init decoder start...\n");

	int nRet = -1;

	av_register_all();
	psDecoderHandle->m_psFmtCxt = avformat_alloc_context();

	unsigned char *aviobuffer = (unsigned char *)av_malloc(BUFFERLENGTH);
	printf("callback=%p\n", pfCB);
	AVIOContext *avio = avio_alloc_context(aviobuffer, BUFFERLENGTH, 0, NULL, pfCB, NULL, NULL);
	psDecoderHandle->m_psFmtCxt->pb = avio;

	nRet = avformat_open_input(&psDecoderHandle->m_psFmtCxt,NULL, NULL, NULL);
	if (nRet < 0)
	{
		fprintf(stderr, "avformat_open_input failed:%s\n", strerror(errno));		
		avformat_close_input(&psDecoderHandle->m_psFmtCxt);
		psDecoderHandle->m_psFmtCxt = NULL;
		return -1;
	}

	nRet = avformat_find_stream_info(psDecoderHandle->m_psFmtCxt, NULL);
	if (nRet < 0)
	{
		fprintf(stderr, "avformat_open_input failed:%s\n", strerror(errno));		
		avformat_close_input(&psDecoderHandle->m_psFmtCxt);
		psDecoderHandle->m_psFmtCxt = NULL;
		return -1;
	}

	int i = 0;
	for (i = 0; i < psDecoderHandle->m_psFmtCxt->nb_streams; ++i)
	{
		if (AVMEDIA_TYPE_VIDEO == psDecoderHandle->m_psFmtCxt->streams[i]->codec->codec_type)	
		{
			printf("AV_CODEC_ID_H264=%d,codec_id=%d\n",AV_CODEC_ID_H264,psDecoderHandle->m_psFmtCxt->streams[i]->codec->codec_id);
			psDecoderHandle->m_nVideoIndex = i;
			break;	
		}
	}

	psDecoderHandle->m_psCodecCxt = psDecoderHandle->m_psFmtCxt->streams[psDecoderHandle->m_nVideoIndex]->codec;

	psDecoderHandle->m_psCodec = avcodec_find_decoder(psDecoderHandle->m_psCodecCxt->codec_id);
	if (NULL == psDecoderHandle->m_psCodec)
	{
		fprintf(stderr, "avcodec_find_decoder failed:%s\n", strerror(errno));		
		avformat_close_input(&psDecoderHandle->m_psFmtCxt);
		psDecoderHandle->m_psFmtCxt = NULL;
		return -1;
	}

	nRet = avcodec_open2(psDecoderHandle->m_psCodecCxt, psDecoderHandle->m_psCodec, NULL);
	if (nRet < 0)
	{
		fprintf(stderr, "avcodec_open2 failed:%s\n", strerror(errno));
		avformat_close_input(&psDecoderHandle->m_psFmtCxt);
		psDecoderHandle->m_psFmtCxt = NULL;
		return -1;
	}

	psDecoderHandle->m_psFrame = av_frame_alloc();
	psDecoderHandle->m_psFrameYUV = av_frame_alloc();

	uint8_t* pnOutBuf = av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, psDecoderHandle->m_psCodecCxt->width, psDecoderHandle->m_psCodecCxt->height));

	avpicture_fill((AVPicture*)psDecoderHandle->m_psFrameYUV, pnOutBuf, AV_PIX_FMT_YUV420P, psDecoderHandle->m_psCodecCxt->width, psDecoderHandle->m_psCodecCxt->height);

	psDecoderHandle->m_psPacket = av_malloc(sizeof(AVPacket));

	psDecoderHandle->m_psSwsCxt = sws_getContext(psDecoderHandle->m_psCodecCxt->width,psDecoderHandle->m_psCodecCxt->height, AV_PIX_FMT_YUV420P,psDecoderHandle->m_psCodecCxt->width,psDecoderHandle->m_psCodecCxt->height, AV_PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL);
	if (NULL == psDecoderHandle->m_psSwsCxt)
	{
		fprintf(stderr, "sws_getContext failed:%s\n", strerror(errno));
		av_frame_free(&psDecoderHandle->m_psFrame);
		psDecoderHandle->m_psFrame = NULL;
		
		av_frame_free(&psDecoderHandle->m_psFrameYUV);
		psDecoderHandle->m_psFrameYUV = NULL;
		//pnOutBuf是否需要free?

		avcodec_close(psDecoderHandle->m_psCodecCxt);
		psDecoderHandle->m_psCodecCxt = NULL;

		avformat_close_input(&psDecoderHandle->m_psFmtCxt);
		psDecoderHandle->m_psFmtCxt = NULL;
		return -1;
	}

	printf("init decoder end...\n");

	return 0;
}

int unInitDecoder(decoderHandle_S* psDecodeHandle)
{
	if (NULL == psDecodeHandle)
	{
		printf("unInitDecoder NULL == psDecoderHandle\n");		
		return -1;
	}
	
	if (NULL != psDecodeHandle->m_psSwsCxt)
	{
		sws_freeContext(psDecodeHandle->m_psSwsCxt);
		psDecodeHandle->m_psSwsCxt = NULL;
	}

	if (NULL != psDecodeHandle->m_psFrame)
	{
		av_frame_free(&psDecodeHandle->m_psFrame);
		psDecodeHandle->m_psFrame = NULL;
	}

	if (NULL != psDecodeHandle->m_psFrameYUV)
	{
		av_frame_free(&psDecodeHandle->m_psFrameYUV);
		psDecodeHandle->m_psFrameYUV = NULL;
	}

	if (NULL != psDecodeHandle->m_psCodecCxt)
	{
		avcodec_close(psDecodeHandle->m_psCodecCxt);
		psDecodeHandle->m_psCodecCxt = NULL;
		//
	}

	if (NULL != psDecodeHandle->m_psFmtCxt)
	{
		avformat_close_input(&psDecodeHandle->m_psFmtCxt);
		psDecodeHandle->m_psFmtCxt = NULL;
	}

	return 0;
}

//int decodeFrame(decoderHandle_S* psDecodeHandle, char** ppcBuf, int nBufSize)//nBufSize w*h
int decodeFrame(decoderHandle_S* psDecodeHandle, char* pcFrameBuf, int nFrameResolution)//nFrameResolution w*h
{
	if (NULL == psDecodeHandle || NULL == pcFrameBuf)
	{
		printf("decodeFrame NULL == psDecoderHandle || NULL == pcFrameBuf\n");		
		return -1;
	}

	printf("start decode a frame...\n");

	int nRet = 0;
	int nGotPicture = 0;
	if (0 == av_read_frame(psDecodeHandle->m_psFmtCxt, psDecodeHandle->m_psPacket))
	{
		if (psDecodeHandle->m_psPacket->stream_index == psDecodeHandle->m_nVideoIndex)		
		{
//			printf("pts=%lld, dts=%lld,size=%d\n",psDecodeHandle->m_psPacket->pts, psDecodeHandle->m_psPacket->dts, psDecodeHandle->m_psPacket->size);			
			//解码一帧压缩的视频数据
			nRet = avcodec_decode_video2(psDecodeHandle->m_psCodecCxt, psDecodeHandle->m_psFrame, &nGotPicture, psDecodeHandle->m_psPacket);
			if (nRet < 0)
			{
				fprintf(stderr, "avcodec_decode_video2 failed:%s\n", strerror(errno));
				return -1;
			}
			
			//
			if (nGotPicture)
			{
				psDecodeHandle->m_nDataFlag = 1;
				sws_scale(psDecodeHandle->m_psSwsCxt, (const uint8_t *const*)psDecodeHandle->m_psFrame->data, psDecodeHandle->m_psFrame->linesize,0, psDecodeHandle->m_psCodecCxt->height,psDecodeHandle->m_psFrameYUV->data, psDecodeHandle->m_psFrameYUV->linesize);
				printf("psDecodeHandle->m_nFrameCount=%d,width=%d,height=%d\n",psDecodeHandle->m_nFrameCount,psDecodeHandle->m_psCodecCxt->width,psDecodeHandle->m_psCodecCxt->height);
				memcpy(pcFrameBuf, psDecodeHandle->m_psFrameYUV->data[0], nFrameResolution);
				memcpy(pcFrameBuf+nFrameResolution, psDecodeHandle->m_psFrameYUV->data[1], nFrameResolution/4);
				memcpy(pcFrameBuf+(nFrameResolution+nFrameResolution/4), psDecodeHandle->m_psFrameYUV->data[2], nFrameResolution/4);

				(psDecodeHandle->m_nFrameCount)++;
			}
		}

		av_free_packet(psDecodeHandle->m_psPacket);			
		printf("decode a frame end...\n");
		return 0;
	}
	else
	{
		printf("decode a frame continue...\n");
		return -1;		
	}
}

