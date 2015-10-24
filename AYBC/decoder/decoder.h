#ifndef __DECODER_H
#define __DECODER_H

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>

#define BUFFERLENGTH 1024

typedef struct _DecoderHandle
{
	AVFormatContext* 	m_psFmtCxt;
	AVCodec*		 	m_psCodec;
	AVCodecContext*  	m_psCodecCxt;
	AVFrame* 		 	m_psFrameYUV;
	AVFrame* 		 	m_psFrame;
	AVPacket* 		 	m_psPacket;
	struct SwsContext*  m_psSwsCxt;
	int 				m_nVideoIndex;
	int 				m_nFrameCount;
	int 				m_nFd;
	int 				m_nDataFlag;
}decoderHandle_S;

typedef int(*readBufCB_F)(void *opaque, uint8_t *buf, int buf_size);

int initDecoder(decoderHandle_S* psDecoderHandle);
int unInitDecoder(decoderHandle_S* psDecodeHandle);
int registerCallBack(readBufCB_F readBuf);

//pcFrameBuf(out):size = resolution*3/2 
int decodeFrame(decoderHandle_S* psDecodeHandle, char* pcFrameBuf, int nFrameResolution);//nFrameResolution w*h

#endif	//__DECODER_H

