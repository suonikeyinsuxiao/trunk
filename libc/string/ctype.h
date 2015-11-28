#ifndef _LINUX_CTYPE_H
#define _LINUX_CTYPE_H

/*
 * NOTE! This ctype does not handle EOF like the standard C
 * library is required to.
 */

// 1 				1 		1				1 	   1	  1	      1		 1
// hard space      hex 	 white space      punct  cntrl  digit    lower  upper
#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */

extern unsigned char _ctype[];

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)	((__ismask(c)&(_U|_L|_D)) != 0)//是否为字母数字
#define isalpha(c)	((__ismask(c)&(_U|_L)) != 0)//字母
#define iscntrl(c)	((__ismask(c)&(_C)) != 0)//控制字符
#define isdigit(c)	((__ismask(c)&(_D)) != 0)//数字
#define isgraph(c)	((__ismask(c)&(_P|_U|_L|_D)) != 0)//图形字符
#define islower(c)	((__ismask(c)&(_L)) != 0)//小写字母
#define isprint(c)	((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)//可打印字符
#define ispunct(c)	((__ismask(c)&(_P)) != 0)//标点
#define isspace(c)	((__ismask(c)&(_S)) != 0)//空格字符(制表符。回车符，换行符等)
#define isupper(c)	((__ismask(c)&(_U)) != 0)//大写字母
#define isxdigit(c)	((__ismask(c)&(_D|_X)) != 0)//16进制数字

#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)

static inline unsigned char __tolower(unsigned char c)
{
	if (isupper(c))
		c -= 'A'-'a';
	return c;
}

static inline unsigned char __toupper(unsigned char c)
{
	if (islower(c))
		c -= 'a'-'A';
	return c;
}

#define tolower(c) __tolower(c)
#define toupper(c) __toupper(c)

#endif
