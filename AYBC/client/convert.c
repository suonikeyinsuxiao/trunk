#include"convert.h"

#define BIT_COUNT   24

void set_bmp_header(struct bmp_header_t *header, U32 width, U32 height)
{
    header->magic = 0x4d42;
    header->image_size = width * height * BIT_COUNT/8;
    header->file_size = header->image_size + 54;
    header->RESERVED1 = 0;
    header->offset = 54;

    header->head_num = 40;
    header->width = width;
    header->height = height;
    header->color_planes = 1;
    header->bit_count = BIT_COUNT;
    header->bit_compression = 0;
    header->h_resolution = 0;
    header->v_resolution = 0;
    header->color_num = 0;
    header->important_colors = 0;
}

int yuyv2rgb24(U8 *yuyv, U8 *rgb, U32 width, U32 height)
{
    U32 i, in, rgb_index = 0;
    U8 y0, u0, y1, v1;
    int r, g, b;
    U32 out = 0, x, y;
 
    for(in = 0; in < width * height * 2; in += 4)
    {
	y0 = yuyv[in+0];
	u0 = yuyv[in+1];
	y1 = yuyv[in+2];
	v1 = yuyv[in+3];

	for (i = 0; i < 2; i++)
	{
		if (i)
			y = y1;
		else
			y = y0;	
		r = y + (140 * (v1-128))/100;  //r
		g = y - (34 * (u0-128))/100 - (71 * (v1-128))/100; //g
 		b = y + (177 * (u0-128))/100; //b
		if(r > 255)   r = 255;
        	if(g > 255)   g = 255;
        	if(b > 255)   b = 255;
       		if(r < 0)     r = 0;
        	if(g < 0)     g = 0;
	        if(b < 0)     b = 0;

		y = height - rgb_index/width -1;
		x = rgb_index%width;
		rgb[(y*width+x)*3+0] = b;
		rgb[(y*width+x)*3+1] = g;
		rgb[(y*width+x)*3+2] = r;
		rgb_index++;
	}
    }
    return 0;
}

//packed格式yuyv转成planner格式I422
void yuyv2I422(U8 *yuyv, U8 *YUV422, U32 width, U32 height)
{
	U8 *Y = NULL;
	U8 *U = NULL;
	U8 *V = NULL;
	U8 *p = yuyv;
	
	//422
	Y = YUV422;
	U = Y + width*height;
	V = U + (width*height>>1);
	
	int row, col;
	for(row = 0; row < height; ++row)
	{ 
		for(col = 0; col < (width>>1); ++col)
		{    
			Y[col*2]   = p[4*col];    
			U[col]     = p[4*col+1];
			Y[col*2+1] = p[4*col+2];
			V[col]     = p[4*col+3];
		}
		p = p + width*2;

		Y = Y + width;
		U = U + (width>>1);
		V = V + (width>>1);
	}
}

//planner格式I422转成planner格式I420
void I422yuv420(U8 *YUV422, U8 *YUV, U32 width, U32 height)
{
	unsigned char *Y, *U, *V;
	unsigned char * Y2, *U2, *V2;
	
	//I422
  	Y = YUV422;
	U = Y + width*height;
	V = U + (width*height>>1);
		
	//I420
	Y2 = YUV;
	U2 = Y2 + width*height;
	V2 = U2 +(width*height>>2);

	int i = 0;
	for(i = 0; i < (height/2); ++i)
	{
		memcpy(U2, U, width>>1);
		memcpy(V2, V, width>>1);

		U2 = U2 + (width>>1);
		V2 = V2 + (width>>1);

		U = U + (width);
		V = V + (width);
	}

	memcpy(Y2, Y, width*height);
}

//平面YUV420转RGB24
void YUV420p_to_RGB24(unsigned char *yuv420[3], unsigned char *rgb24, int width, int height)
{
	int R,G,B,Y,U,V;
	int x,y;
	int nWidth = width>>1; //色度信号宽度

	for (y = 0; y < height; y++)
	{
  		for (x = 0;x < width ; x++)
		{
   			Y = *(yuv420[0] + y*width + x);
   			U = *(yuv420[1] + ((y>>1)*nWidth) + (x>>1));
   			V = *(yuv420[2] + ((y>>1)*nWidth) + (x>>1));

   			R = Y + 1.402*(V-128);
   			G = Y - 0.34414*(U-128) - 0.71414*(V-128);
   			B = Y + 1.772*(U-128);

   			//防止越界
   			if (R>255)R=255;
   			if (R<0)R=0;
   			if (G>255)G=255;
   			if (G<0)G=0;
   			if (B>255)B=255;
   			if (B<0)B=0;
   			
   			*(rgb24 + ((height-y-1)*width + x)*3) = B;
   			*(rgb24 + ((height-y-1)*width + x)*3 + 1) = G;
   			*(rgb24 + ((height-y-1)*width + x)*3 + 2) = R;
// 	 	    *(rgb24 + (y*width + x)*3) = B;
// 		    *(rgb24 + (y*width + x)*3 + 1) = G;
// 		    *(rgb24 + (y*width + x)*3 + 2) = R;   
  		}
 	}
}

void YUV422p_to_RGB24(unsigned char *yuv422[3], unsigned char *rgb24, int width, int height)
{
	int R,G,B,Y,U,V;
	int x,y;
	int nWidth = width>>1; //色度信号宽度

	for (y = 0; y < width; y++) 
	{
  		for (x = 0; x < height; x++) 
		{
   			Y = *(yuv422[0] + y*width + x);
   			U = *(yuv422[1] + y*nWidth + (x>>1));
   			V = *(yuv422[2] + y*nWidth + (x>>1));

   			R = Y + 1.402*(V-128);
   			G = Y - 0.34414*(U-128) - 0.71414*(V-128);
   			B = Y + 1.772*(U-128);
   			
   			//防止越界
   			if (R>255)R=255;
   			if (R<0)R=0;
   			if (G>255)G=255;
   			if (G<0)G=0;
   			if (B>255)B=255;
   			if (B<0)B=0;
   			
   			*(rgb24 + ((height-y-1)*width + x)*3) = B;
   			*(rgb24 + ((height-y-1)*width + x)*3 + 1) = G;
   			*(rgb24 + ((height-y-1)*width + x)*3 + 2) = R;  
  		}
 	}
}

//平面YUV420转平面YUV422
void YUV420p_to_YUV422p(unsigned char *yuv420[3], unsigned char *yuv422, int width, int height)  
{
	int x, y;

 	//亮度信号Y复制
 	int Ylen = width*height;
 	memcpy(yuv422, yuv420[0], Ylen);

 	//色度信号U复制
 	unsigned char *pU422 = yuv422 + Ylen; //指向U的位置
 	int Uwidth = width>>1; //422色度信号U宽度
 	int Uheight = height>>1; //422色度信号U高度 

 	for (y = 0; y < Uheight; y++) 
 	{         
		memcpy(pU422 + y*width, yuv420[1] + y*Uwidth, Uwidth);
 	 	memcpy(pU422 + y*width + Uwidth, yuv420[1] + y*Uwidth, Uwidth);
 	}

 	//色度信号V复制
 	unsigned char *pV422 = yuv422 + Ylen + (Ylen>>1); //指向V的位置
 	int Vwidth = Uwidth; //422色度信号V宽度
 	int Vheight = Uheight; //422色度信号U宽度

 	for (y = 0; y < Vheight; y++) 
 	{  
		memcpy(pV422 + y*width, yuv420[2] + y*Vwidth, Vwidth);
		memcpy(pV422 + y*width + Vwidth, yuv420[2] + y*Vwidth, Vwidth);
 	} 
}
