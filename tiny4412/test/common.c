#include "common.h"

unsigned int getPROID(void)
{
	return PRO_ID;
}

unsigned int getPACKAGEID(void)
{
	return PACKAGE_ID;
}

void delay(void)
{
	volatile unsigned int val = 0xfffff; 
		while(val--); 
}

void key_delay(void)
{
	volatile unsigned int val = 0xffff; 
		while(val--); 
}

void putchar(unsigned char ch)
{
	while(1)
	{
		if (UTRSTAT0 & (1<<2))	
			break;
	}

	UTXH0 &= ~0xff;
	UTXH0 |= ch;

	if ('\r' == ch)
		putchar('\n');
}

unsigned char getchar(void)
{
	unsigned char ch;
	while(1)
	{
		if (UTRSTAT0 & 1)	
			break;
	}

	ch = URXH0 & 0xff;
	
	return ch;
}

void puts(const char* pStr)
{
	while(*pStr)
	{
		putchar(*(pStr++));
	}
}

void gets(char* pcBuf)
{
	unsigned char ch;
	int i = 0;
	for (i = 0; ; i++)
	{
		ch = getchar();	
		putchar(ch);
		if ('\r' == ch)
			break;
		pcBuf[i] = ch;
	}

	pcBuf[i] = '\0';
}
void printnumber(int num, int base)
{
	if (0 == num)	
		return;

	printnumber(num/base, base);
	putchar("0123456789ABCDEF"[num%base]);
}

int printu(const char* format, ...)
{
	unsigned int* pnAddr = (unsigned int*)(&format);	
	pnAddr++;

	while(*format)
	{
		if (*format != '%')	
		{
			putchar(*format);	
			format++;
		}
		else
		{
			switch(*format)				
			{
				case  'd':	
				{
					printnumber(*(pnAddr++), 10);
				}
				break;
				case  'c':	
				{
					putchar(*(pnAddr++));	
				}
				break;
				case  'x':	
				{
					putchar('0');
					putchar('#');
					printnumber(*(pnAddr++), 16);
				}
				break;
				case  's':	
				{
					puts((char*)(*pnAddr++));	
				}
				break;
				case  'o':	
				{
					printnumber(*(pnAddr++), 8);
				}
				break;
			}
		}
	}
}
