
#include "CameralOpt.h"


int video_fd ; 
int length ; 
char *yuv[COUNT] ; 
struct v4l2_buffer  enqueue  , dequeue ; 

int Init_Cameral(int Width , int Hight)
{
	//参数检查
	char *videodevname = NULL ; 
	videodevname = "/dev/video0" ; 
	
	//打开设备
	video_fd = open(videodevname , O_RDWR);
	if(-1 == video_fd )
	{
		perror("open video device fail");
		return -1 ; 
	}

	int i ; 
	int ret ; 
	struct v4l2_format  format ; 
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
	format.fmt.pix.width  = Width; 
	format.fmt.pix.height = Hight; 
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV ; 

	ret = ioctl(video_fd , VIDIOC_S_FMT , &format);
	if(ret != 0)
	{
		perror("set video format fail");
		return -2 ; 
	}


	//申请buffer,切割成几个部分
	//3
	struct v4l2_requestbuffers  requestbuffer ; 
	requestbuffer.count = COUNT ; 
	requestbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
	requestbuffer.memory = V4L2_MEMORY_MMAP ; 

	ret = ioctl(video_fd , VIDIOC_REQBUFS , &requestbuffer);
	if(ret != 0)
	{
		perror("request buffer fail ");
		return -3  ;
	}
	
	
	//querybuffer
	struct v4l2_buffer querybuffer ; 
	querybuffer.type =  V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
	querybuffer.memory = V4L2_MEMORY_MMAP ; 

	for(i = 0 ; i < COUNT ; i++)
	{
		querybuffer.index = i ; 	

		ret = ioctl(video_fd , VIDIOC_QUERYBUF , &querybuffer);
		if(ret != 0)
		{
			perror("query buffer fail");
			return -4 ; 
		}
	
//		printf("index:%d length:%d  offset:%d \n" , 
//		querybuffer.index , querybuffer.length , querybuffer.m.offset);
		length = querybuffer.length ; 

		//将摄像头内存印射到进程的内存地址
		yuv[i] = mmap(0,querybuffer.length , PROT_READ | PROT_WRITE , MAP_SHARED , video_fd , querybuffer.m.offset );


		//列队
		
		struct v4l2_buffer  queuebuffer ; 
		queuebuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
		queuebuffer.memory =  V4L2_MEMORY_MMAP ; 
		queuebuffer.index = i ; 	

		ret = ioctl(video_fd , VIDIOC_QBUF , &queuebuffer);
		if(ret != 0)
		{
			perror("queuebuffer fail");
			return -5 ; 
		}
	}
	//初始化入队出队
	enqueue.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
	dequeue.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
	enqueue.memory = V4L2_MEMORY_MMAP ; 
	dequeue.memory = V4L2_MEMORY_MMAP ; 

	return 0 ; 
}

int Exit_Cameral(void)
{
	int i ; 
	for(i = 0 ; i < COUNT ; i++)
		munmap(yuv+i , length);
	close(video_fd);
	return 0 ; 
}

int Start_Cameral(void)
{
	//开启摄像头
	int ret ; 
	int on = 1 ; 
	ret = ioctl(video_fd , VIDIOC_STREAMON , &on);
	if(ret != 0)
	{
		perror("start Cameral fail");
		return -1 ; 
	}
	return 0 ; 
}
int Stop_Cameral(void)
{
	//停止摄像头
	int ret ; 
	int off= 1 ; 
	ret = ioctl(video_fd , VIDIOC_STREAMOFF, &off);
	if(ret != 0)
	{
		perror("stop Cameral fail");
		return -1 ; 
	}
	return 0 ;
}

int Get_Picture(char *buffer)
{
	int ret ; 
	//出队
	ret = ioctl(video_fd , VIDIOC_DQBUF , &dequeue);
	if(ret != 0)
	{
		perror("dequeue fail");
		return -1 ; 
	}

	//获取图片数据 YUV   yuv[dequeue.index]
	memcpy(buffer , yuv[dequeue.index] , dequeue.length);
//	write(yuyv_fd , yuv[dequeue.index] , dequeue.length);

	enqueue.index = dequeue.index ; 
	ret = ioctl(video_fd , VIDIOC_QBUF , &enqueue);
	if(ret != 0)
	{
		perror("enqueue fail");
		return -2 ; 
	}
	return 0 ; 
}

