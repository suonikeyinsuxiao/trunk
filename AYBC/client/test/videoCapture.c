#include"videoCapture.h"

static U8* pcYuv[COUNT]={NULL};
static int vidDevFd = -1;
static int nLength = 0;
static struct v4l2_buffer dequeBuffer;
static struct v4l2_buffer queueBuffer;

int initCamera()
{
	//open /dev/video*
	//open video device
	vidDevFd = open(VIDEODEVNAME, O_RDONLY);
	if (vidDevFd < 0)
	{
		fprintf(stderr, "open %s failed %s\n",VIDEODEVNAME, strerror(errno));			
		return -1;
	}

	//printf("vidDevFd = %d!\n", vidDevFd);			

	//set video device format
	struct v4l2_format vidDevFmt;
	vidDevFmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	vidDevFmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	vidDevFmt.fmt.pix.width = WIDTH;
	vidDevFmt.fmt.pix.height = HEIGHT;
	
	if (0 != ioctl(vidDevFd, VIDIOC_S_FMT, &vidDevFmt))
	{
		fprintf(stderr, "VIDIOC_S_FMT failed %s\n", strerror(errno));			
		unInitCamera();
		return -1;
	}

	//request buffers
	//max count need to be considered
	struct v4l2_requestbuffers reqBuffer;
	reqBuffer.count = COUNT;
	reqBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqBuffer.memory = V4L2_MEMORY_MMAP;

	if (0 != ioctl(vidDevFd, VIDIOC_REQBUFS, &reqBuffer))
	{
		fprintf(stderr, "VIDIOC_REQBUFS failed %s\n", strerror(errno));			
		unInitCamera();
		return -1;
	}

	//query buffers
	int i = 0;
	struct v4l2_buffer qryBuffer;
	qryBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	qryBuffer.memory = V4L2_MEMORY_MMAP;

	for (i = 0; i < COUNT; i++)
	{
		qryBuffer.index = i;
		
		if (0 != ioctl(vidDevFd, VIDIOC_QUERYBUF, &qryBuffer))
		{
			fprintf(stderr, "VIDIOC_QUERYBUF failed %s\n", strerror(errno));			
			unInitCamera();
			return -1;
		}

		printf("qryBuffer.m.offset=%d, qryBuffer.lenght=%d\n", qryBuffer.m.offset, qryBuffer.length);
		//mmap video device memory allocated by VIDIOC_REQBUFS
		pcYuv[i] = mmap(NULL, qryBuffer.length, PROT_READ, MAP_SHARED, vidDevFd, qryBuffer.m.offset);
		nLength = qryBuffer.length;

		//queue buffers
		struct v4l2_buffer queBuffer;
		queBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		queBuffer.memory = V4L2_MEMORY_MMAP;
		queBuffer.index = qryBuffer.index;
		if (0 != ioctl(vidDevFd, VIDIOC_QBUF, &queBuffer))
		{
			fprintf(stderr, "init VIDIOC_QBUF failed %s\n", strerror(errno));			
			unInitCamera();
			return -1;
		}

	}

	dequeBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	dequeBuffer.memory = V4L2_MEMORY_MMAP;
	queueBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	queueBuffer.memory = V4L2_MEMORY_MMAP;

	return 0;	
}

void unInitCamera()
{
	int i = 0;

	for (i = 0; i < COUNT; i++)
	{
		if (NULL != pcYuv[i])			
		{
			if (-1 == munmap(pcYuv[i], nLength));
			{
				//fprintf(stderr,"munmap error:%s!\n",strerror(errno));
			}
			pcYuv[i] = NULL;
		}
	}

	if (vidDevFd >= 0)
		close(vidDevFd);
}

int startCamera()
{
	//start camera		

	int nArg = 1;
	if (0 != ioctl(vidDevFd, VIDIOC_STREAMON, &nArg))
	{
		fprintf(stderr, "VIDIOC_STREAMON failed %s\n", strerror(errno));			
		unInitCamera();
		return -1;
	}

	return 0;
}

int stopCamera()
{
	//stop camera		

	int nArg = 1;
	if (0 != ioctl(vidDevFd, VIDIOC_STREAMOFF, &nArg))
	{
		fprintf(stderr, "VIDIOC_STREAMOFF failed %s\n", strerror(errno));			
		unInitCamera();
		return -1;
	}

	return 0;
}

int getOneFrame(unsigned char* pcYuyvBuffer)
{
	if (0 != ioctl(vidDevFd, VIDIOC_DQBUF, &dequeBuffer))
	{
		fprintf(stderr, "VIDIOC_DQBUF failed %s\n", strerror(errno));			
		unInitCamera();
		return -1;
	}

	//get frame
	memcpy(pcYuyvBuffer, pcYuv[dequeBuffer.index], dequeBuffer.length);
//	printf("len=%d\n",dequeBuffer.length);

	//queue
	queueBuffer.index = dequeBuffer.index;
	if (0 != ioctl(vidDevFd, VIDIOC_QBUF, &queueBuffer))
	{
		fprintf(stderr, "VIDIOC_QBUF failed %s\n", strerror(errno));			
		unInitCamera();
		return -1;
	}
	return 0;
}
