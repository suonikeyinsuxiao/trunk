#include "lcd.h"

void initLcd(unsigned int nAddr)
{
	//GPIO
	GPF0CON = 0x22222222;    
    GPF1CON = 0x22222222;
    GPF2CON = 0x22222222;
    GPF3CON = 0x22222222;

	//System Register
	LCDCBLK_CFG |=  (1 << 1);

	//CMU
	//SCLKMPLL_USER_T (800MHZ)
    //800MHZ / (1+1) = SCLK_FIMD0 (400MHZ)
	CLK_SRC_LCD0 &= ~(0xf);
	CLK_SRC_LCD0 |= 0x6;

	CLK_DIV_LCD &= ~(0xf);
	CLK_DIV_LCD |= (0x1);

	//Displayer Controller
	VIDCON0 = 9 << 6;	//VCLK=SCLK_FIMD0 / (CLKVAL + 1)
	VIDCON1 = (1 << 7)| (1 << 6)| (1 << 5);	
	VIDTCON0 = (22 << 16) | (21 << 8);
	VIDTCON1 = (45 << 16) | (209 << 8);
	VIDTCON2 = (479 << 11) | (799);
#if BMP24
	WINCON0 = (1 << 15)(0xb << 2) | 1;//888
#elif BMP16
	WINCON0 = (1 << 16)|(0x5 << 2) | 1;//565
#else
	WINCON0 = (1 << 15)|(0xb << 2) | 1;//888
#endif
	VIDOSD0A = 0;
	VIDOSD0B = (800 << 11) | (480);
	VIDW00ADD0B0 = nAddr;
	VIDW00ADD1B0 = nAddr + 800 * 480 * 4;
	VIDW00ADD2 =  800 * 4;
	SHADOWCON |= 1;

	VIDCON0 |= 0x3;
}
