#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>

#include "netsocket.h"

void showDuration(int nDuration, char* pcDuration);

FILE *fp_open=NULL;
static int nSockFd = -1;
static int nRecvLen;
char acip[24] = {0};
int port = 0;

//Callback
int read_buffer(void *opaque, uint8_t *buf, int buf_size)
{
	printf("recv...nSockFd=%d................\n",nSockFd);
	if (-1 == netSocketRecvfrom(nSockFd, buf, buf_size,acip, &port, &nRecvLen, 5000))	
	{
		printf("read_buffer return\n");			
		return -1;
	}
	
	printf("ip=%s,port=%d,nRecvLen=%d\n",acip, port, nRecvLen);
	return nRecvLen;
}

//int read_buffer(void *opaque, uint8_t *buf, int buf_size){
//	if(!feof(fp_open)){
//		int true_size=fread(buf,1,buf_size,fp_open);
//		return true_size;
//	}else{
//		return -1;
//	
//}

int main(int argc, char** argv)
{
	int nRet = -1;

	nSockFd = netSocketCreate(NET_SOCKET_UDP);
	if (-1 == nSockFd)
	{
		fprintf(stderr, "create socket failed:%s\n", strerror(errno));
		return -1;
	}

	netSocketBind(nSockFd,12271);

	
	//注册所有组件
	av_register_all();

	AVFormatContext* pFormatCxt = NULL;
	
	//Allocate an AVFormatContext
	pFormatCxt = avformat_alloc_context();

//	fp_open=fopen(argv[1],"rb+");
	

	//Init AVIOContext
	unsigned char *aviobuffer=(unsigned char *)av_malloc(1024);
	AVIOContext *avio =avio_alloc_context(aviobuffer, 1024,0,NULL,read_buffer,NULL,NULL);
	pFormatCxt->pb=avio;

	printf("xxxxxxxxxxxxxxxxx\n");
	nRet = avformat_open_input(&pFormatCxt,NULL, NULL, NULL);
	if (nRet < 0)
	{
		fprintf(stderr, "avformat_open_input failed:%s\n", strerror(errno));		
		avformat_free_context(pFormatCxt);
		pFormatCxt = NULL;
		return -1;
	}

	//获取输入视频文件信息
	nRet = avformat_find_stream_info(pFormatCxt, NULL);
	if (nRet < 0)
	{
		fprintf(stderr, "avformat_open_input failed:%s\n", strerror(errno));		
		avformat_free_context(pFormatCxt);
		pFormatCxt = NULL;

		av_free(aviobuffer);
		av_free(avio);
		return -1;
	}
	printf("yyyyyyyyyyyyyxx\n");

	char acDuration[32] = {0};

	showDuration(pFormatCxt->duration, acDuration);
	printf("duration:%s",acDuration);

	printf("pFormatCxt->nb_streams=%d\nfilename=%s\nduration=%lld\nbitrate=%d\n",pFormatCxt->nb_streams, pFormatCxt->filename, pFormatCxt->duration, pFormatCxt->bit_rate);

	int i = 0;
	int nVideoIndex = 0;
	for (i=0; i < pFormatCxt->nb_streams; ++i)
	{
		printf("pFormatCxt->streams[i]->index=%d\n",pFormatCxt->streams[i]->index);
		printf("pFormatCxt->streams[i]->id=%d\n",pFormatCxt->streams[i]->id);
		printf("pFormatCxt->streams[i]->codec->bit_rate=%d\n",pFormatCxt->streams[i]->codec->bit_rate);
		printf("pFormatCxt->streams[i]->time_base.num=%d,pFormatCxt->streams[i]->time_base.den=%d\n",pFormatCxt->streams[i]->time_base.num,pFormatCxt->streams[i]->time_base.den);
		if (AVMEDIA_TYPE_VIDEO == pFormatCxt->streams[i]->codec->codec_type)	
		{
			printf("AV_CODEC_ID_H264=%d,codec_id=%d\n",AV_CODEC_ID_H264,pFormatCxt->streams[i]->codec->codec_id);
			nVideoIndex = i;
			break;	
		}
	}

	av_dump_format(pFormatCxt,0,argv[1],0);
	
	AVCodec* pCodec = NULL;
	AVCodecContext* pCodecCxt = pFormatCxt->streams[nVideoIndex]->codec;

	printf("pCodecCxt->time_base.num=%d,pCodecCxt->time_base.den=%d\n",pCodecCxt->time_base.num,pCodecCxt->time_base.den);

	//查找空闲的解码器
	pCodec = avcodec_find_decoder(pCodecCxt->codec_id);
	if (NULL == pCodec)
	{
		printf("yyyyyyyyyyyyyyyyyyyyy\n");
		fprintf(stderr, "avcodec_find_decoder failed:%s\n", strerror(errno));		
		avformat_free_context(pFormatCxt);
		pFormatCxt = NULL;

		av_free(aviobuffer);
		av_free(avio);
		return -1;
	}

	printf("xxxxxxxxxxxxxxxxxxxxxxx\n");
	//打开解码器
	nRet = avcodec_open2(pCodecCxt, pCodec, NULL);
	if (nRet < 0)
	{
		fprintf(stderr, "avcodec_open2 failed:%s\n", strerror(errno));
		avformat_free_context(pFormatCxt);
		pFormatCxt = NULL;
		av_free(aviobuffer);
		av_free(avio);
		return -1;
	}

	AVFrame* pFrame = av_frame_alloc();
	AVFrame* pFrameYUV = av_frame_alloc();
	AVPacket* pPacket = NULL;
	int nGotPicture = 0;

	//分配存储视频数据的buffer
	uint8_t* pnOutBuf = av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCxt->width, pCodecCxt->height));
	//将buffer填充到pFrameYUV
	avpicture_fill((AVPicture*)pFrameYUV, pnOutBuf, AV_PIX_FMT_YUV420P, pCodecCxt->width, pCodecCxt->height);

	pPacket = av_malloc(sizeof(AVPacket));

	struct SwsContext* pSwsCxt = sws_getContext(pCodecCxt->width,pCodecCxt->height, AV_PIX_FMT_YUV420P,pCodecCxt->width,pCodecCxt->height, AV_PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL);
	if (NULL == pSwsCxt)
	{
		fprintf(stderr, "sws_getContext failed:%s\n", strerror(errno));
		goto ERR;
	}
	int framecount=0;

	int fd = -1;
	fd = open("xxx_mem.yuv",O_CREAT|O_EXCL|O_WRONLY|O_TRUNC,0777);
	if (-1 == fd)
	{
		fprintf(stderr, "open xxx.yuv failed:%s\n",strerror(errno));
		goto ERR;
	}

	printf("aaaaaaaaaaaaaaaaa\n");
	//读取一帧压缩的视频数据
	while(0 == av_read_frame(pFormatCxt, pPacket))
	{
		if (pPacket->stream_index == nVideoIndex)		
		{
//			printf("pts=%lld, dts=%lld,size=%d\n",pPacket->pts, pPacket->dts, pPacket->size);			
			//解码一帧压缩的视频数据
			nRet = avcodec_decode_video2(pCodecCxt, pFrame, &nGotPicture, pPacket);
			if (nRet < 0)
			{
						
				fprintf(stderr, "avcodec_decode_video2 failed:%s\n", strerror(errno));
				av_free_packet(pPacket);
				goto ERR;
			}
			
			//
			if (nGotPicture)
			{
				sws_scale(pSwsCxt, (const uint8_t *const*)pFrame->data, pFrame->linesize,0, pCodecCxt->height,pFrameYUV->data, pFrameYUV->linesize);
				//printf("decode frame count:%d\n",framecount);
				write(fd,pFrameYUV->data[0], pCodecCxt->width*pCodecCxt->height);
				write(fd,pFrameYUV->data[1], pCodecCxt->width*pCodecCxt->height/4);
				write(fd,pFrameYUV->data[2], pCodecCxt->width*pCodecCxt->height/4);

				framecount++;
			}
		}

		av_free_packet(pPacket);
	}

	printf("bbbbbbbbbbbbbba\n");
	
ERR:
	close(fd);
	sws_freeContext(pSwsCxt);
	
	av_frame_free(&pFrame);
	pFrame = NULL;
	
	av_frame_free(&pFrameYUV);
	pFrameYUV = NULL;

	printf("dddddddddddd\n");
	avformat_free_context(pFormatCxt);
	pFormatCxt = NULL;

	printf("zzzzzzzzzzzzzzzzzx\n");
//	av_free(aviobuffer);
//	av_free(avio);
	printf("cccccccccccccccczx\n");
	
	return 0;
}

void showDuration(int nDuration, char* pcDuration)
{
	//duration是以微秒为单位  
	//转换成hh:mm:ss形式  
	if (nDuration > 0 && pcDuration != NULL)
	{
		int ns, thh, tmm, tss;  
		ns 	= nDuration/1000000;  
		thh = ns / 3600;  
		tmm = (ns % 3600) / 60;  
		tss = (ns % 60);  
		sprintf(pcDuration,"%02d:%02d:%02d\n",thh,tmm,tss); 
	}
}
