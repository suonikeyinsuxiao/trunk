#ifndef __DECODER_H
#define __DECODER_H

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>

#include "common.h"

typedef struct _DecoderHandle
{
	AVFormatContext*	m_psFormatCxt;
	AVCodecContext*		m_psCodecCxt;
	AVCodec*			m_psCodec;
	AVFrame*			m_psFrame;
	AVPacket*			m_psPacket;
	struct SwsContext*	m_psSwsCxt;
}DecoderHandle_S;

int initDecoder();


#endif	//__DECODER_H
