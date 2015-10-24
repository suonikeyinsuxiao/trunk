#include "cpu.h"
#include "types.h"
#include "uart.h"
#include "io.h"
#include "serial.h"
//#include "clock.h"

//DECLARE_GLOBAL_DATA_PTR;

static inline struct tiny4412_uart *tiny4412_get_base_uart(int dev_index)
{
	u32 offset = dev_index * sizeof(struct tiny4412_uart);
//	return (struct tiny4412_uart *)(samsung_get_base_uart() + offset);
	return (struct tiny4412_uart *)samsung_get_base_uart();

}

/*
 * The coefficient, used to calculate the baudrate on tiny4412 UARTs is
 * calculated as
 * C = UBRDIV * 16 + number_of_set_bits_in_UDIVSLOT
 * however, section 31.6.11 of the datasheet doesn't recomment using 1 for 1,
 * 3 for 2, ... (2^n - 1) for n, instead, they suggest using these constants:
 */
static const int udivslot[] = {
	0,
	0x0080,
	0x0808,
	0x0888,
	0x2222,
	0x4924,
	0x4a52,
	0x54aa,
	0x5555,
	0xd555,
	0xd5d5,
	0xddd5,
	0xdddd,
	0xdfdd,
	0xdfdf,
	0xffdf,
};

void serial_setbrg_dev(const int dev_index)
{
	struct tiny4412_uart *const uart = tiny4412_get_base_uart(dev_index);
	u32 uclk = 100000000;//get_uart_clk(dev_index);
	u32 baudrate = 115200;//gd->baudrate;
	u32 val;

	val = uclk / baudrate;

	writel(val / 16 - 1, &uart->ubrdiv);

	if (use_divslot)
		writew(udivslot[val % 16], &uart->rest.slot);
	else
		writeb(val % 16, &uart->rest.value);
}

/*
 * Initialise the serial port with the given baudrate. The settings
 * are always 8 data bits, no parity, 1 stop bit, no start bits.
 */
int serial_init_dev(const int dev_index)
{
	struct tiny4412_uart *const uart = tiny4412_get_base_uart(dev_index);

	/* reset and enable FIFOs, set triggers to the maximum */
	writel(0, &uart->ufcon);
	writel(0, &uart->umcon);
	/* 8N1 */
	writel(0x3, &uart->ulcon);
	/* No interrupts, no DMA, pure polling */
	writel(0x245, &uart->ucon);

	serial_setbrg_dev(dev_index);

	return 0;
}

static int serial_err_check(const int dev_index, int op)
{
	struct tiny4412_uart *const uart = tiny4412_get_base_uart(dev_index);
	unsigned int mask;

	/*
	 * UERSTAT
	 * Break Detect	[3]
	 * Frame Err	[2] : receive operation
	 * Parity Err	[1] : receive operation
	 * Overrun Err	[0] : receive operation
	 */
	if (op)
		mask = 0x8;
	else
		mask = 0xf;

	return readl(&uart->uerstat) & mask;
}
	
/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise. When the function is succesfull, the character read is
 * written into its argument c.
 */
int serial_getc_dev(const int dev_index)
{
	struct tiny4412_uart *const uart = tiny4412_get_base_uart(dev_index);

	/* wait for character to arrive */
	while (!(readl(&uart->utrstat) & 0x1)) {
		if (serial_err_check(dev_index, 0))
			return 0;
	}

	return (int)(readb(&uart->urxh) & 0xff);
}
	
/*
 * Output a single byte to the serial port.
 */
void serial_putc_dev(const char c, const int dev_index)
{
	struct tiny4412_uart *const uart = tiny4412_get_base_uart(dev_index);

	/* wait for room in the tx FIFO */
	while (!(readl(&uart->utrstat) & 0x2)) {
		if (serial_err_check(dev_index, 1))
			return;
	}

	writeb(c, &uart->utxh);

	/* If \n, also do \r */
	if (c == '\n')
		serial_putc('\r');
}

/*
 * Test whether a character is in the RX buffer
 */
int serial_tstc_dev(const int dev_index)
{
	struct tiny4412_uart *const uart = tiny4412_get_base_uart(dev_index);

	return (int)(readl(&uart->utrstat) & 0x1);
}

void serial_puts_dev(const char *s, const int dev_index)
{
	while (*s)
		serial_putc_dev(*s++, dev_index);
}

/* Multi serial device functions */
#define DECLARE_tiny4412_SERIAL_FUNCTIONS(port) \
int tiny4412_serial##port##_init(void) { return serial_init_dev(port); } \
void tiny4412_serial##port##_setbrg(void) { serial_setbrg_dev(port); } \
int tiny4412_serial##port##_getc(void) { return serial_getc_dev(port); } \
int tiny4412_serial##port##_tstc(void) { return serial_tstc_dev(port); } \
void tiny4412_serial##port##_putc(const char c) { serial_putc_dev(c, port); } \
void tiny4412_serial##port##_puts(const char *s) { serial_puts_dev(s, port); }

#define INIT_tiny4412_SERIAL_STRUCTURE(port, name, bus) { \
	name, \
	bus, \
	tiny4412_serial##port##_init, \
	NULL, \
	tiny4412_serial##port##_setbrg, \
	tiny4412_serial##port##_getc, \
	tiny4412_serial##port##_tstc, \
	tiny4412_serial##port##_putc, \
	tiny4412_serial##port##_puts, }

DECLARE_tiny4412_SERIAL_FUNCTIONS(0);
struct serial_device tiny4412_serial0_device =
	INIT_tiny4412_SERIAL_STRUCTURE(0, "tiny4412ser0", "tiny4412UART0");
DECLARE_tiny4412_SERIAL_FUNCTIONS(1);
struct serial_device tiny4412_serial1_device =
	INIT_tiny4412_SERIAL_STRUCTURE(1, "tiny4412ser1", "tiny4412UART1");
DECLARE_tiny4412_SERIAL_FUNCTIONS(2);
struct serial_device tiny4412_serial2_device =
	INIT_tiny4412_SERIAL_STRUCTURE(2, "tiny4412ser2", "tiny4412UART2");
DECLARE_tiny4412_SERIAL_FUNCTIONS(3);
struct serial_device tiny4412_serial3_device =
	INIT_tiny4412_SERIAL_STRUCTURE(3, "tiny4412ser3", "tiny4412UART3");


