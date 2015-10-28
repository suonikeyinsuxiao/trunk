
#ifndef _RES_H
#define _RES_H

//#include "uart.h"
//#include "cpu.h"

#define  NULL    0
typedef  unsigned int u32 ; 

//抄内核的
# define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))

//只能用来做检测，没有任何意义。值为零，为1时不通过
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))

#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

//led register
#define    GPM4CON    *(volatile unsigned int *)0x110002E0
#define    GPM4DAT	  *(volatile unsigned int *)0x110002E4

//buzzer register
#define    GPD0CON    *(volatile unsigned int *)0x114000A0
#define    GPD0DAT	  *(volatile unsigned int *)0x114000A4

//key register
#define    GPX3CON    *(volatile unsigned int *)0x11000C60
#define    GPX3DAT    *(volatile unsigned int *)0x11000C64

//PRO_ID
#define    PACKET_ID    *(volatile unsigned int *)0x10000004
#define    PRO_ID_T		*(volatile unsigned int *)0x10000000

//RTC  register
#define  rtcbase    0x10070000

#define    INTP        *(volatile unsigned int *)( rtcbase + 0x0030)
#define    RTCCON      *(volatile unsigned int *)( rtcbase + 0x0040)
#define    TICCNT      *(volatile unsigned int *)( rtcbase + 0x0044)
#define    RTCALM      *(volatile unsigned int *)( rtcbase + 0x0050)
#define    ALMSEC      *(volatile unsigned int *)( rtcbase + 0x0054)
#define    ALMMIN      *(volatile unsigned int *)( rtcbase + 0x0058)
#define    ALMHOUR     *(volatile unsigned int *)( rtcbase + 0x005C)
#define    ALMDAY      *(volatile unsigned int *)( rtcbase + 0x0060)
#define    ALMMON      *(volatile unsigned int *)( rtcbase + 0x0064)
#define    ALMYEAR     *(volatile unsigned int *)( rtcbase + 0x0068)
#define    BCDSEC      *(volatile unsigned int *)( rtcbase + 0x0070)
#define    BCDMIN      *(volatile unsigned int *)( rtcbase + 0x0074)
#define    BCDHOUR     *(volatile unsigned int *)( rtcbase + 0x0078)
#define    BCDDAYWEEK  *(volatile unsigned int *)( rtcbase + 0x007C)
#define    BCDDAY      *(volatile unsigned int *)( rtcbase + 0x0080)
#define    BCDMON      *(volatile unsigned int *)( rtcbase + 0x0084)
#define    BCDYEAR     *(volatile unsigned int *)( rtcbase + 0x0088)
#define    CURTICCNT   *(volatile unsigned int *)( rtcbase + 0x0090)

//*****************串口命令包 
struct UartCommand
{
	char Command[20];
	unsigned int  Type ; 
};

enum 
{
	LED_ON,
	LED_OFF,
	BUZZER_ON,
	BUZZER_OFF,
	RESET,
	COMMAND_NO_FOUND,
};

//uart register

#define    GPA0CON    *(volatile unsigned int *)(0x11400000)
#define    GPA1CON    *(volatile unsigned int *)(0x11400020)

#define    uartbase   0x13830000
#define    ULCONn     *(volatile unsigned int *)(uartbase + 0x0000)
#define    UCONn      *(volatile unsigned int *)(uartbase + 0x0004)
#define    UFCONn     *(volatile unsigned int *)(uartbase + 0x0008)
#define    UMCONn     *(volatile unsigned int *)(uartbase + 0x000C)
#define    UTRSTATn   *(volatile unsigned int *)(uartbase + 0x0010) 
#define    UERSTATn   *(volatile unsigned int *)(uartbase + 0x0014)
#define    UFSTATn    *(volatile unsigned int *)(uartbase + 0x0018)
#define    UMSTATn    *(volatile unsigned int *)(uartbase + 0x001C)
#define    UTXHn      *(volatile unsigned int *)(uartbase + 0x0020)
#define    URXHn      *(volatile unsigned int *)(uartbase + 0x0024)
#define    UBRDIVn    *(volatile unsigned int *)(uartbase + 0x0028)
#define    UFRACVALn  *(volatile unsigned int *)(uartbase + 0x002C)
#define    UINTPn     *(volatile unsigned int *)(uartbase + 0x0030)
#define    UINTSPn    *(volatile unsigned int *)(uartbase + 0x0034)
#define    UINTMn     *(volatile unsigned int *)(uartbase + 0x0038)

//watch dog register
#define    basedog    0x10060000
#define    WTCON      *(volatile unsigned int *)(basedog + 0x0000)
#define    WTDAT      *(volatile unsigned int *)(basedog + 0x0004)
#define    WTCNT      *(volatile unsigned int *)(basedog + 0x0008)
#define    WTCLRINT   *(volatile unsigned int *)(basedog + 0x000C)

#define    AUTOMATIC_WDT_RESET_DISABLE  *(volatile unsigned int *)(0x10020408)
#define    MASK_WDT_RESET_REQUEST *(volatile unsigned int *)(0x1002040C)



//reset register
	
#define    RESET_REG      *(volatile unsigned int *)(0x10020400)

typedef  int (*PRINT_T)(const char *fmt , ...) ; 


//basefunction
void delay(void);
void key_delay(void);
int config_buzzer(void);
int config_led(void);
int config_key(void);
int led_all_on(void);
int led_all_off(void);
int led_which_on(int which);
int led_which_off(int which);
int buzzer_on(void);
int buzzer_off(void);
int get_key(void);

//获取产品ID
unsigned int Get_ID(unsigned int *tmp);

///抠

#define __arch_getb(a)			(*(volatile unsigned char *)(a))
#define __arch_getw(a)			(*(volatile unsigned short *)(a))
#define __arch_getl(a)			(*(volatile unsigned int *)(a))

#define __arch_putb(v,a)		(*(volatile unsigned char *)(a) = (v))
#define __arch_putw(v,a)		(*(volatile unsigned short *)(a) = (v))
#define __arch_putl(v,a)		(*(volatile unsigned int *)(a) = (v))


#define writeb(v,a)			__arch_putb(v,a)
#define writew(v,a)			__arch_putw(v,a)
#define writel(v,a)			__arch_putl(v,a)

#define readb(a)			__arch_getb(a)
#define readw(a)			__arch_getw(a)
#define readl(a)			__arch_getl(a)

struct s5p_gpio_bank {
	unsigned int	con;
	unsigned int	dat;
	unsigned int	pull;
	unsigned int	drv;
	unsigned int	pdn_con;
	unsigned int	pdn_pull;
	unsigned char	res1[8];
};

/* functions */
void gpio_cfg_pin(struct s5p_gpio_bank *bank, int gpio, int cfg);
void gpio_direction_output(struct s5p_gpio_bank *bank, int gpio, int en);
void gpio_direction_input(struct s5p_gpio_bank *bank, int gpio);
void gpio_set_value(struct s5p_gpio_bank *bank, int gpio, int en);
unsigned int gpio_get_value(struct s5p_gpio_bank *bank, int gpio);
void gpio_set_pull(struct s5p_gpio_bank *bank, int gpio, int mode);
void gpio_set_drv(struct s5p_gpio_bank *bank, int gpio, int mode);
void gpio_set_rate(struct s5p_gpio_bank *bank, int gpio, int mode);

/* Pin configurations */
#define GPIO_INPUT	0x0
#define GPIO_OUTPUT	0x1
#define GPIO_IRQ	0xf
#define GPIO_FUNC(x)	(x)

/* Pull mode */
#define GPIO_PULL_NONE	0x0
#define GPIO_PULL_DOWN	0x1
#define GPIO_PULL_UP	0x2

/* Drive Strength level */
#define GPIO_DRV_1X	0x0
#define GPIO_DRV_2X	0x1
#define GPIO_DRV_3X	0x2
#define GPIO_DRV_4X	0x3
#define GPIO_DRV_FAST	0x0
#define GPIO_DRV_SLOW	0x1

//****************************************rtc


unsigned int bcd2bin(unsigned char val);

unsigned char bin2bcd (unsigned int val);

/*
 * The struct used to pass data from the generic interface code to
 * the hardware dependend low-level code ande vice versa. Identical
 * to struct rtc_time used by the Linux kernel.
 *
 * Note that there are small but significant differences to the
 * common "struct time":
 *
 *		struct time:		struct rtc_time:
 * tm_mon	0 ... 11		1 ... 12
 * tm_year	years since 1900	years since 0
 */

struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

int rtc_get (struct rtc_time *);
int rtc_set (struct rtc_time *);
void rtc_reset (void);

int putchar(char ch);
char getchar(void);
int puts(const char *str);
void print_number(int number , int base);
int printu(const char *fmt , ...);
int Excute_Command(unsigned int Type);
unsigned int Find_Command(char *buffer , struct UartCommand *command , int size);
void Init_Device(void );
void Init_Serial(void);
void do_Serial(void);

//*********************string  interface
typedef    unsigned int  size_t ; 
char * strcpy(char * dest,const char *src);
char * strncpy(char * dest,const char *src,size_t count);
char * strcat(char * dest, const char * src);
char * strncat(char *dest, const char *src, size_t count);
int strcmp(const char * cs,const char * ct);
int strncmp(const char * cs,const char * ct,size_t count);
char * strchr(const char * s, int c);
char * strrchr(const char * s, int c);
size_t strlen(const char * s);
size_t strnlen(const char * s, size_t count);
size_t strspn(const char *s, const char *accept);
char * strpbrk(const char * cs,const char * ct);
char * strtok(char * s,const char * ct);
char * strsep(char **s, const char *ct);
char *strswab(const char *s);
void * memset(void * s,int c,size_t count);
char * bcopy(const char * src, char * dest, int count);
void * memcpy(void *dest, const void *src, size_t count);
void * memmove(void * dest,const void *src,size_t count);
int memcmp(const void * cs,const void * ct,size_t count);
void * memscan(void * addr, int c, size_t size);
char * strstr(const char * s1,const char * s2);
void *memchr(const void *s, int c, size_t n);

//**************************************************
//serial

void serial_setbrg_dev(const int dev_index , int baudrate );
int serial_init_dev(const int dev_index ,int  baudrate );
static int serial_err_check(const int dev_index, int op);
int serial_getc_dev(const int dev_index);
void serial_putc_dev(const char c, const int dev_index);
int serial_tstc_dev(const int dev_index);
void serial_puts_dev(const char *s, const int dev_index);



#endif //_RES_H
