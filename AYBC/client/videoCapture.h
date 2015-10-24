#ifndef __VIDEOCAPTURE_H
#define __VIDEOCAPTURE_H

#include"common.h"
#include<sys/mman.h>
#include<fcntl.h>
#include<linux/videodev2.h>

#define VIDEODEVNAME ("/dev/video0")		
#define COUNT  4

int initCamera();
void unInitCamera();
int startCamera();
int stopCamera();
int getOneFrame(U8* pcYuvBuffer);

#endif	//__VIDEOCAPTURE_H
