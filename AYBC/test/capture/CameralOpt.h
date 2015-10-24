
#ifndef  _CAMERALOPT_H
#define  _CAMERALOPT_H

#include <stdio.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
//#include <mydefine.h>
#include <sys/mman.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define   COUNT  3

int Init_Cameral(int Width , int Hight);
int Exit_Cameral(void);

int Start_Cameral(void);
int Stop_Cameral(void);

int Get_Picture(char *buffer);

#endif  //_CAMERALOPT_H
