#ifndef _IMAGE_H
#define _IMAGE_H

#include"init.h"
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>

/**
 * function:loadImage()
 *
 * Description: load image
 *
 * parameter:
 *		const char* pcFile 图片文件路径
 *
 * return:
 *
 */
//void loadImage(const char* pcFile, SDL_Surface* psOut);
int loadImage(const char* pcFile, SDL_Surface** ppsSurface);

/**
 * function:drawImage()
 *
 * Description:将psSrc表示的图片画在psDst中的psDstRect区域
 *
 * parameter:
 *		psSrc: 源图片Surface
 *		psDst: 画布
 *		psDstRect: 显示区域
 *
 * return:
 *
 */
void drawImage(SDL_Surface* psSrc, SDL_Surface* psDst, SDL_Rect* psDstRect);

#endif
