/***********************************************************************
*   Copyright (C) 2016 pilot-lab.inc All rights reserved.
*   
*   @file:       double_pound_sign.c
*   @brief:      
*   @author:     Pilot labs
*   @maintainer: frank.fu@pilot-lab.com.cn
*   @version:    1.0
*   @date:       2016-09-13
*   
***********************************************************************/
//#include "double_pound_sign.h"
#include <stdio.h>
#include <stdlib.h>

// ##被称为连接符（concatenator），用来将两个Token连接为一个Token。注意这里连接的对象是Token就行，而不一定是宏的变量。
// 比如你要做一个菜单项命令名和函数指针组成的结构体的数组，并且希望在函数名和菜单项命令名之间有直观的、名字上的关系。

#define COMMAND(NAME) { #NAME, NAME##_command }
//#define QUIT "quit"

struct command
{
	char* name;
	void (*function) (void);
};

void quit_command()
{
	printf("quit_command\n");
}

void help_command()
{
	printf("help_command\n");
}

int main(int argc, char** argv)
{
	struct command asCommands[] = {
		COMMAND(quit),
		COMMAND(help),
	};

	printf("1 cmd is %s\n", asCommands[0].name);
	asCommands[0].function();
	printf("2 cmd is %s\n", asCommands[1].name);
	asCommands[1].function();

	return 0;
}


/*
有时我们写程序可能会定义许多DEBUG模式下的调试打印信息宏来替代原有的打印函数,这时就会出现编写可变量参数宏的写法问题。

首先在gcc中,对可变量参数宏的支持要强的多,一般我们可以这么写:
#define MACRO_1(fmt, args...) printf(fmt, args)
#define MACRO_2(fmt, ...) printf(fmt, __VA_ARGS__)

这两种写法都可以,后面一种是使用了关键字__VA_ARGS__,它代表宏中出现的...,而前面的写法就是给可变量的参数定义了一个名字,就是说在第一种写法中,args就是代表可变量参数。
给出例子:
#include <stdio.h>
#define MACRO_1(fmt, args...) printf(fmt, args)
#define MACRO_2(fmt, ...)     printf(fmt, __VA_ARGS__)
int main()
{
	MACRO_1("MACRO_1:%s\n", "Hello");
	MACRO_2("MACRO_2:%s\n", "Hello");
	return 0;
}
我们可以用gcc -E的参数来得到程序的宏展开源代码,主要部分如下:

int main()
{
	printf("MACRO_1:%s\n", "Hello");
	printf("MACRO_2:%s\n", "Hello");
	return 0;
}

大家可以看到这里的对比.

但有一个问题就是,如果我们只传递一个参数会怎么呢?也就是把源代码改写如下:
#include <stdio.h>
#define MACRO_1(fmt, args...) printf(fmt, args)
#define MACRO_2(fmt, ...)     printf(fmt, __VA_ARGS__)
int main()
{
	MACRO_1("MACRO_1\n");
	MACRO_2("MACRO_2\n");
	return 0;
}
这样会不会有问题呢,我们看一下宏展开后的代码:
int main()
{
	printf("MACRO_1\n", );
	printf("MACRO_2\n", );
	return 0;
}
这里多出现了一个",",问题的解决方法是在args和__VA_ARGS__前面加一个##,##表示去除它前面的","（显然这是##的第二个功能，至于为什么能去除“，”还有待研究），但当我们传递多于一个参数的时候这也能工作，就像第一段代码一样，现在看一下加了##之后的代码及相应宏展开后的代码：
#include <stdio.h>
#define MACRO_1(fmt, args...) printf(fmt, ##args)
#define MACRO_2(fmt, ...)     printf(fmt, ##__VA_ARGS__)
int main()
{
	MACRO_1("MACRO_1\n");
	MACRO_2("MACRO_2\n");
	return 0;
}
展开宏之后：
int main()
{
	printf("MACRO_1\n");
	printf("MACRO_2\n");
	return 0;
}

*/
