#ifndef __CONVERT_H
#define __CONVERT_H

#include "common.h"

#pragma pack(1)

struct bmp_header_t{
    U16        magic;
    U32       file_size;
    U32       RESERVED1;
    U32       offset;         //54 bytes

    U32       head_num;    //40
    U32       width;
    U32       height;
    U16       color_planes; //1
    U16       bit_count;
    U32       bit_compression; //0
    U32       image_size; //except the size of header
    U32       h_resolution;
    U32       v_resolution;
    U32       color_num;
    U32       important_colors;
};
#pragma pack()

void set_bmp_header(struct bmp_header_t * header, U32 width, U32 height);
int yuyv2rgb24(U8 *yuyv, U8 *rgb, U32 width, U32 height);

void I422yuv420(U8 *YUV422, U8 *YUV, U32 width, U32 height);

void yuyv2I422(U8 *yuyv, U8 *YUV422, U32 width, U32 height);

void YUV422p_to_RGB24(unsigned char *yuv422[3], unsigned char *rgb24, int width, int height);

void YUV420p_to_YUV422p(unsigned char *yuv420[3], unsigned char *yuv422, int width, int height)  ;

void YUV420p_to_RGB24(unsigned char *yuv420[3], unsigned char *rgb24, int width, int height);
#endif	//__CONVERT_H
