#ifndef __ASM_ARCH_UART_H_
#define __ASM_ARCH_UART_H_

#ifndef __ASSEMBLY__
/* baudrate rest value */
union br_rest {
	unsigned short	slot;		/* udivslot */
	unsigned char	value;		/* ufracval */
};
	
struct tiny4412_uart {
	unsigned int	ulcon;
	unsigned int	ucon;
	unsigned int	ufcon;
	unsigned int	umcon;
	unsigned int	utrstat;
	unsigned int	uerstat;
	unsigned int	ufstat;
	unsigned int	umstat;
	unsigned char	utxh;
	unsigned char	res1[3];
	unsigned char	urxh;
	unsigned char	res2[3];
	unsigned int	ubrdiv;
	union br_rest	rest;
	unsigned char	res3[0x3d0];
};

static int use_divslot = 1;

#endif	/* __ASSEMBLY__ */

#endif
