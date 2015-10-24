#ifndef __FRAMEBUFFER_H
#define __FRAMEBUFFER_H

#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<linux/fb.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>


/*
 *description: init framebuffer
 *
 * parameters:
 * 		pcFbPath: path of framebuffer device 
 *
 * return: success, return the file descriptor of fb device;otherwise return -1;
 * 		
 */
int initFrameBuf(char* pcFbPath, int flag);

/*
 *description: get the fb_var_screeninfo of fb devie,and print 
 *
 * parameters:
 * 		nFd(in): file descriptor of framebuffer device 
 *		psVarInfo(out): store  fb_var_screeninfo
 *
 * return: void
 * 		
 */
void getFbVarSrceenInfo(int nFd, struct fb_var_screeninfo* psVarInfo);

/*
 *description: get the fb_fix_screeninfo of fb devie,and print 
 *
 * parameters:
 * 		nFd(in): file descriptor of framebuffer device 
 *		psFixInfo(out): store  fb_fix_screeninfo
 *
 * return: void
 * 		
 */
void getFbFixSrceenInfo(int nFd, struct fb_fix_screeninfo* psFixInfo);

/*
 *description: mmap memory of fb devie to process space 
 *
 * parameters:
 * 		nMapSize: the size of maping
 *		nMode   : protection mode
 * 		flags	: The  flags  argument  determines whether updates to the mapping are visible to other processes mapping the same region, and whether updates are carried through to the underlying file. 
 * 		nFd		: the file desp return by initFrameBuf 
 *		nFileOffset: offset of memory 
 *
 * return: the point to kernel memory of fb device ;otherwise MAP_FAILED 
 * 		
 */
char* mapFrameBuf(int nMapSize, int nMode, int flags, int nFd, int nFileOffset);

/*
 *description: draw a picture start at (x0, y0), size is width*height
 *
 * parameters:
 *
 * return: void
 * 		
 */
void drawFb(int x0, int y0, int width, int height, int color, char* pFb);




#endif		// __FRAMEBUFFER_H
