/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       macro.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-09-13
*   
***********************************************************************/
//#include "macro.h"
#include <stdio.h>
#include <stdlib.h>

//C语言中如何使用宏,C/C++中的宏（Macro）属于编译器预处理的范畴，属于编译期概念（而非运行期概念）。

//在C语言的宏中，#的功能是将其后面的宏参数进行字符串化操作（Stringification），简单说就是在被替换的宏变量左右各加上一个双引号。
#define PSQR(x) printf("the square of " #x " is %d.\n",(x)*(x))

#define WARN_IF(EXP)    do{ if (EXP)    fprintf(stderr, "Warning: " #EXP "\n"); }   while(0)



int main(int argc, char** argv)
{
	int y =4;
	PSQR(y);
	PSQR(2+4);

	int divider = 0;
	WARN_IF(divider == 0);


	return 0;
}
