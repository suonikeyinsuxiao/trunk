#ifndef __SPI_H
#define __SPI_H

#define    iccbase         0x10480000
#define    ICCICR_CPU0     *(volatile unsigned int *)( 0x0000 + iccbase )
#define    ICCPMR_CPU0     *(volatile unsigned int *)( 0x0004 + iccbase )
#define    ICCBPR_CPU0     *(volatile unsigned int *)( 0x0008 + iccbase )
#define    ICCIAR_CPU0     *(volatile unsigned int *)( 0x000C + iccbase )
#define    ICCEOIR_CPU0    *(volatile unsigned int *)( 0x0010 + iccbase )

#define    icdbase         0x10490000
#define    ICDDCR          *(volatile unsigned int *)( 0x0000 + icdbase )
#define    ICDICTR         *(volatile unsigned int *)( 0x0004 + icdbase )
#define    ICDIIDR         *(volatile unsigned int *)( 0x0008 + icdbase )
#define    ICDISR0_CPU0    *(volatile unsigned int *)( 0x0080 + icdbase )
#define    ICDISR1         *(volatile unsigned int *)( 0x0084 + icdbase )
#define    ICDISR2         *(volatile unsigned int *)( 0x0088 + icdbase )
#define    ICDISR3         *(volatile unsigned int *)( 0x008C + icdbase )
#define    ICDISR4         *(volatile unsigned int *)( 0x0090 + icdbase )
#define    ICDISER0_CPU0   *(volatile unsigned int *)( 0x0100 + icdbase )
#define    ICDISER1        *(volatile unsigned int *)( 0x0104 + icdbase )
#define    ICDISER2        *(volatile unsigned int *)( 0x0108 + icdbase )
#define    ICDISER3        *(volatile unsigned int *)( 0x010C + icdbase )
#define    ICDISER4        *(volatile unsigned int *)( 0x0110 + icdbase )
#define    ICDIPR0_CPU0    *(volatile unsigned int *)( 0x0400 + icdbase )
#define    ICDIPR1_CPU0    *(volatile unsigned int *)( 0x0404 + icdbase )
#define    ICDIPR2_CPU0    *(volatile unsigned int *)( 0x0408 + icdbase )
#define    ICDIPR3_CPU0    *(volatile unsigned int *)( 0x040C + icdbase )
#define    ICDIPR4_CPU0    *(volatile unsigned int *)( 0x0410 + icdbase )
#define    ICDIPR5_CPU0    *(volatile unsigned int *)( 0x0414 + icdbase )
#define    ICDIPR6_CPU0    *(volatile unsigned int *)( 0x0418 + icdbase )
#define    ICDIPR7_CPU0    *(volatile unsigned int *)( 0x041C + icdbase )
#define    ICDIPTR0_CPU0   *(volatile unsigned int *)( 0x0800 + icdbase )
#define    ICDIPTR1_CPU0   *(volatile unsigned int *)( 0x0804 + icdbase )
#define    ICDIPTR2_CPU0   *(volatile unsigned int *)( 0x0808 + icdbase )
#define    ICDIPTR3_CPU0   *(volatile unsigned int *)( 0x080C + icdbase )
#define    ICDIPTR4_CPU0   *(volatile unsigned int *)( 0x0810 + icdbase )
#define    ICDIPTR5_CPU0   *(volatile unsigned int *)( 0x0814 + icdbase )
#define    ICDIPTR6_CPU0   *(volatile unsigned int *)( 0x0818 + icdbase )
#define    ICDIPTR7_CPU0   *(volatile unsigned int *)( 0x081C + icdbase )
#define    ICDIPTR8        *(volatile unsigned int *)( 0x0820 + icdbase )
#define    ICDIPTR9        *(volatile unsigned int *)( 0x0824 + icdbase )
#define    ICDIPTR10       *(volatile unsigned int *)( 0x0828 + icdbase )
#define    ICDIPTR11       *(volatile unsigned int *)( 0x082C + icdbase )
#define    ICDIPTR12       *(volatile unsigned int *)( 0x0830 + icdbase )
#define    ICDIPTR13       *(volatile unsigned int *)( 0x0834 + icdbase )
#define    ICDIPTR14       *(volatile unsigned int *)( 0x0838 + icdbase )
#define    ICDSGIR         *(volatile unsigned int *)( 0x0F00 + icdbase )

#define   EXT_INT_BASE  0x11000E00

void enableInterruptCPSR();
void enableSPI(int nID);

#endif //__SPI_H
