#ifndef __HARDWARE_H
#define __HARDWARE_H

#define REGL(base,addr)	(*(volatile unsigned int *)  ((base)+(addr)))
#define REGW(base,addr)	(*(volatile unsigned short *)((base)+(addr)))
#define REGB(base,addr)	(*(volatile unsigned char *) ((base)+(addr)))

#define CHIPIDBASE		0x10000000
#define PRO_ID     		REGL(CHIPIDBASE,0x0000) 				// Product information (ID, package, and revision) 0xE441_2XXX
#define PACKAGE_ID 		REGL(CHIPIDBASE,0x0004) 				// Package information (POP type and package)      0xXXXX_XXXX


#define GPIOBASE_PART2             	0x11000000 					 //Base Address: 0x1100_0000
//led register
#define GPM4CON    					REGL(GPIOBASE_PART2,0x02E0) //Port group GPM4 configuration register 0x0000_0000
#define GPM4DAT    					REGL(GPIOBASE_PART2,0x02E4) //Port group GPM4 data register              0x00
//key register
#define GPX3CON 					REGL(GPIOBASE_PART2,0x0C60) //Port group GPX3 configuration register 0x0000_0000
#define GPX3DAT 					REGL(GPIOBASE_PART2,0x0C64) //Port group GPX3 data register              0x00


//Exynos 4412 SCP contains 304 multi-functional input/output port pins and 164 memory port pins. There are 37 general port groups and two memory port groups. 
//GPIO registers
#define GPIOBASE_PART1    			0x11400000 		  			//Base Address: 0x1140_0000
#define GPA0CON     				REGL(GPIOBASE_PART1,0x0000) //Port group GPA0 configuration register                 0x0000_0000
#define GPA0DAT     				REGL(GPIOBASE_PART1,0x0004) //Port group GPA0 data register                              0x00
#define GPA0PUD     				REGL(GPIOBASE_PART1,0x0008) //Port group GPA0 pull-up/ pull-down register               0x5555
#define GPA0DRV     				REGL(GPIOBASE_PART1,0x000C) // Port group GPA0 drive strength control register         0x00_0000
#define GPA0CONPDN  				REGL(GPIOBASE_PART1,0x0010) //Port group GPA0 power down mode configuration register    0x0000
#define GPA0PUDPDN  				REGL(GPIOBASE_PART1,0x0014) //Port group GPA0 power down mode pull-up/ pull-down  register      0x0000
#define GPA1CON     				REGL(GPIOBASE_PART1,0x0020) //Port group GPA1 configuration register                 0x0000_0000
#define GPA1DAT     				REGL(GPIOBASE_PART1,0x0024) //Port group GPA1 data register                              0x00
#define GPA1PUD     				REGL(GPIOBASE_PART1,0x0028) //Port group GPA1 pull-up/ pull-down register               0x0555
#define GPA1DRV     				REGL(GPIOBASE_PART1,0x002C) // Port group GPA1 drive strength control register         0x00_0000
#define GPA1CONPDN  				REGL(GPIOBASE_PART1,0x0030) //Port group GPA1 power down mode configuration register    0x0000
#define GPA1PUDPDN  				REGL(GPIOBASE_PART1,0x0034) // Port group GPA1 power down mode pull-up/ pull-down register      0x0000
#define GPBCON      				REGL(GPIOBASE_PART1,0x0040) //Port group GPB configuration register                  0x0000_0000
#define GPBDAT      				REGL(GPIOBASE_PART1,0x0044) //Port group GPB data register                               0x00
#define GPBPUD      				REGL(GPIOBASE_PART1,0x0048) //Port group GPB pull-up/ pull-down register                0x5555
#define GPBDRV      				REGL(GPIOBASE_PART1,0x004C) // Port group GPB drive strength control register          0x00_0000
#define GPBCONPDN   				REGL(GPIOBASE_PART1,0x0050) //Port group GPB power down mode configuration register     0x0000
#define GPBPUDPDN   				REGL(GPIOBASE_PART1,0x0054) // Port group GPB power down mode pull-up/ pull-down register        0x0000
#define GPC0CON     				REGL(GPIOBASE_PART1,0x0060) //Port group GPC0 configuration register                 0x0000_0000
#define GPC0DAT     				REGL(GPIOBASE_PART1,0x0064) //Port group GPC0 data register                              0x00
#define GPC0PUD     				REGL(GPIOBASE_PART1,0x0068) //Port group GPC0 Pull-up/ pull-down register               0x0155
#define GPC0DRV     				REGL(GPIOBASE_PART1,0x006C) // Port group GPC0 drive strength control register         0x00_0000
#define GPC0CONPDN  				REGL(GPIOBASE_PART1,0x0070) //Port group GPC0 power down mode configuration register    0x0000
#define GPC0PUDPDN  				REGL(GPIOBASE_PART1,0x0074) //Port group GPC0 power down mode pull-up/ pull-down register       0x0000
#define GPC1CON     				REGL(GPIOBASE_PART1,0x0080) //Port group GPC1 configuration register                 0x0000_0000
#define GPC1DAT     				REGL(GPIOBASE_PART1,0x0084) //Port group GPC1 data register                              0x00
#define GPC1PUD     				REGL(GPIOBASE_PART1,0x0088) //Port group GPC1 pull-up/ pull-down register               0x0155
#define GPC1DRV     				REGL(GPIOBASE_PART1,0x008C) // Port group GPC1 drive strength control register         0x00_0000
#define GPC1CONPDN  				REGL(GPIOBASE_PART1,0x0090) //Port group GPC1 power down mode configuration register    0x0000
#define GPC1PUDPDN  				REGL(GPIOBASE_PART1,0x0094) //Port group GPC1 power down mode pull-up/ pull-down register  0x0000
//buzzer register
#define GPD0CON     				REGL(GPIOBASE_PART1,0x00A0) // Port group GPD0 configuration register                 0x0000_0000
#define GPD0DAT     				REGL(GPIOBASE_PART1,0x00A4) // Port group GPD0 data register                              0x00
#define GPD0PUD     				REGL(GPIOBASE_PART1,0x00A8) // Port group GPD0 pull-up/ pull-down register               0x0055
#define GPD0DRV     				REGL(GPIOBASE_PART1,0x00AC) // Port group GPD0 drive strength control register         0x00_0000
#define GPD0CONPDN  				REGL(GPIOBASE_PART1,0x00B0) // Port group GPD0 power down mode configuration register    0x0000
#define GPD0PUDPDN 					REGL(GPIOBASE_PART1,0x00B4) // Port group GPD0 power down mode pull-up/ pull-down register      0x0000
#define GPD1CON    					REGL(GPIOBASE_PART1,0x00C0) // Port group GPD1 configuration register                 0x0000_0000
#define GPD1DAT    					REGL(GPIOBASE_PART1,0x00C4) // Port group GPD1 data register                              0x00
#define GPD1PUD    					REGL(GPIOBASE_PART1,0x00C8) // Port group GPD1 Pull-up/ pull-down register               0x0055
#define GPD1DRV    					REGL(GPIOBASE_PART1,0x00CC) // Port group GPD1 drive strength control register         0x00_0000
#define GPD1CONPDN 					REGL(GPIOBASE_PART1,0x00D0) // Port group GPD1 power down mode configuration register    0x0000
#define GPD1PUDPDN  				REGL(GPIOBASE_PART1,0x00D4) // Port group GPD1 power down mode pull-up/ pull-down register
#define GPF0CON     				REGL(GPIOBASE_PART1,0x0180) //Port group GPF0 configuration register                 0x0000_0000
#define GPF0DAT     				REGL(GPIOBASE_PART1,0x0184) //Port group GPF0 data register                              0x00
#define GPF0PUD     				REGL(GPIOBASE_PART1,0x0188) //Port group GPF0 pull-up/ pull-down register               0x5555
#define GPF0DRV     				REGL(GPIOBASE_PART1,0x018C) // Port group GPF0 drive strength control register         0x00_0000
#define GPF0CONPDN  				REGL(GPIOBASE_PART1,0x0190) //Port group GPF0 power down mode configuration register    0x0000
#define GPF0PUDPDN  				REGL(GPIOBASE_PART1,0x0194) // Port group GPF0 power down mode pull-up/ pull-down register  0x0000
#define GPF1CON     				REGL(GPIOBASE_PART1,0x01A0) // Port group GPF1 configuration register                 0x0000_0000
#define GPF1DAT     				REGL(GPIOBASE_PART1,0x01A4) // Port group GPF1 data register                              0x00
#define GPF1PUD     				REGL(GPIOBASE_PART1,0x01A8) // Port group GPF1 pull-up/ pull-down register               0x5555
#define GPF1DRV     				REGL(GPIOBASE_PART1,0x01AC) // Port group GPF1 drive strength control register         0x00_0000
#define GPF1CONPDN  				REGL(GPIOBASE_PART1,0x01B0) // Port group GPF1 power down mode configuration register    0x0000
#define GPF1PUDPDN  				REGL(GPIOBASE_PART1,0x01B4) // Port group GPF1 power down mode pull-up/ pull-down        0x0000
#define GPF2CON     				REGL(GPIOBASE_PART1,0x01C0) // Port group GPF2 configuration register                 0x0000_0000
#define GPF2DAT     				REGL(GPIOBASE_PART1,0x01C4) // Port group GPF2 data register                              0x00
#define GPF2PUD     				REGL(GPIOBASE_PART1,0x01C8) // Port group GPF2 pull-up/ pull-down register               0x5555
#define GPF2DRV     				REGL(GPIOBASE_PART1,0x01CC) // Port group GPF2 drive strength control register         0x00_0000
#define GPF2CONPDN  				REGL(GPIOBASE_PART1,0x01D0) // Port group GPF2 power down mode configuration register    0x0000
#define GPF2PUDPDN  				REGL(GPIOBASE_PART1,0x01D4) // Port group GPF2 power down mode pull-up/ pull-down                                               
#define GPF3CON     				REGL(GPIOBASE_PART1,0x01E0) // Port group GPF3 configuration register                 0x0000_0000
#define GPF3DAT     				REGL(GPIOBASE_PART1,0x01E4) // Port group GPF3 data register                              0x00
#define GPF3PUD         			REGL(GPIOBASE_PART1,0x01E8) //Port group GPF3 pull-up/ pull-down register                    0x0555
#define GPF3DRV         			REGL(GPIOBASE_PART1,0x01EC) //Port group GPF3 drive strength control register              0x00_0000
#define GPF3CONPDN      			REGL(GPIOBASE_PART1,0x01F0) //Port group GPF3 power down mode configuration register         0x0000
#define GPF3PUDPDN      			REGL(GPIOBASE_PART1,0x01F4) //Port group GPF3 power down mode pull-up/ pull-down             0x0000
#define ETC1PUD         			REGL(GPIOBASE_PART1,0x0228) //Port group ETC1 pull-up/ pull-down register                    0x0005
#define ETC1DRV         			REGL(GPIOBASE_PART1,0x022C) //Port group ETC1 drive strength control register              0x00_0000
#define GPJ0CON         			REGL(GPIOBASE_PART1,0x0240) //Port group GPJ0 configuration register                      0x0000_0000
#define GPJ0DAT         			REGL(GPIOBASE_PART1,0x0244) //Port group GPJ0 data register                                   0x00
#define GPJ0PUD         			REGL(GPIOBASE_PART1,0x0248) //Port group GPJ0 pull-up/ pull-down register                    0x5555
#define GPJ0DRV         			REGL(GPIOBASE_PART1,0x024C) //Port group GPJ0 drive strength control register              0x00_0000
#define GPJ0CONPDN      			REGL(GPIOBASE_PART1,0x0250) //Port group GPJ0 power down mode configuration register         0x0000
#define GPJ0PUDPDN      			REGL(GPIOBASE_PART1,0x0254) //Port group GPJ0 power down mode pull-up/ pull-down             0x0000
#define GPJ1CON         			REGL(GPIOBASE_PART1,0x0260) //Port group GPJ1 configuration register                      0x0000_0000
#define GPJ1DAT         			REGL(GPIOBASE_PART1,0x0264) //Port group GPJ1 data register                                   0x00
#define GPJ1PUD         			REGL(GPIOBASE_PART1,0x0268) //Port group GPJ1 pull-up/ pull-down register                    0x0155
#define GPJ1DRV         			REGL(GPIOBASE_PART1,0x026C) //Port group GPJ1 drive strength control register              0x00_0000
#define GPJ1CONPDN      			REGL(GPIOBASE_PART1,0x0270) //Port group GPJ1 power down mode configuration register         0x0000
#define GPJ1PUDPDN      			REGL(GPIOBASE_PART1,0x0274) //Port group GPJ1 power down mode pull-up/ pull-down              0x0000
#define EXT_INT1_CON    			REGL(GPIOBASE_PART1,0x0700) //External interrupt EXT_INT1 configuration register          0x0000_0000
#define EXT_INT2_CON    			REGL(GPIOBASE_PART1,0x0704) //External interrupt EXT_INT2 configuration register          0x0000_0000
#define EXT_INT3_CON    			REGL(GPIOBASE_PART1,0x0708) //External interrupt EXT_INT3 configuration register          0x0000_0000
#define EXT_INT4_CON    			REGL(GPIOBASE_PART1,0x070C) //External interrupt EXT_INT4 configuration register          0x0000_0000
#define EXT_INT5_CON    			REGL(GPIOBASE_PART1,0x0710) //External interrupt EXT_INT5 configuration register          0x0000_0000
#define EXT_INT6_CON    			REGL(GPIOBASE_PART1,0x0714) //External interrupt EXT_INT6 configuration register          0x0000_0000
#define EXT_INT7_CON    			REGL(GPIOBASE_PART1,0x0718) //External interrupt EXT_INT7 configuration register          0x0000_0000
#define EXT_INT13_CON   			REGL(GPIOBASE_PART1,0x0730) //External interrupt EXT_INT13 configuration register         0x0000_0000
#define EXT_INT14_CON   			REGL(GPIOBASE_PART1,0x0734) //External interrupt EXT_INT14 configuration register         0x0000_0000
#define EXT_INT15_CON   			REGL(GPIOBASE_PART1,0x0738) //External interrupt EXT_INT15 configuration register         0x0000_0000
#define EXT_INT16_CON   			REGL(GPIOBASE_PART1,0x073C) //External interrupt EXT_INT16 configuration register         0x0000_0000
#define EXT_INT21_CON   			REGL(GPIOBASE_PART1,0x0740) //External interrupt EXT_INT21 configuration register         0x0000_0000
#define EXT_INT22_CON   			REGL(GPIOBASE_PART1,0x0744) //External interrupt EXT_INT22 configuration register         0x0000_0000
#define EXT_INT1_FLTCON0			REGL(GPIOBASE_PART1,0x0800) //External interrupt EXT_INT1 filter configuration register 0 0x0000_0000
#define EXT_INT1_FLTCON1			REGL(GPIOBASE_PART1,0x0804) //External interrupt EXT_INT1 filter configuration register 1 0x0000_0000
#define EXT_INT2_FLTCON0			REGL(GPIOBASE_PART1,0x0808) //External interrupt EXT_INT2 filter configuration register 0 0x0000_0000
#define EXT_INT2_FLTCON1  			REGL(GPIOBASE_PART1,0x080C) //External interrupt EXT_INT2 filter configuration register 1  0x0000_0000
#define EXT_INT3_FLTCON0  			REGL(GPIOBASE_PART1,0x0810) //External interrupt EXT_INT3 filter configuration register 0  0x0000_0000
#define EXT_INT3_FLTCON1  			REGL(GPIOBASE_PART1,0x0814) //External interrupt EXT_INT3 filter configuration register 1  0x0000_0000
#define EXT_INT4_FLTCON0  			REGL(GPIOBASE_PART1,0x0818) //External interrupt EXT_INT4 filter configuration register 0  0x0000_0000
#define EXT_INT4_FLTCON1  			REGL(GPIOBASE_PART1,0x081C) //External interrupt EXT_INT4 filter configuration register 1  0x0000_0000
#define EXT_INT5_FLTCON0  			REGL(GPIOBASE_PART1,0x0820) //External interrupt EXT_INT5 filter configuration register 0  0x0000_0000
#define EXT_INT5_FLTCON1  			REGL(GPIOBASE_PART1,0x0824) //External interrupt EXT_INT5 filter configuration register 1  0x0000_0000
#define EXT_INT6_FLTCON0  			REGL(GPIOBASE_PART1,0x0828) //External interrupt EXT_INT6 filter configuration register 0  0x0000_0000
#define EXT_INT6_FLTCON1  			REGL(GPIOBASE_PART1,0x082C) //External interrupt EXT_INT6 filter configuration register 1  0x0000_0000
#define EXT_INT7_FLTCON0  			REGL(GPIOBASE_PART1,0x0830) //External interrupt EXT_INT7 filter configuration register 0  0x0000_0000
#define EXT_INT7_FLTCON1  			REGL(GPIOBASE_PART1,0x0834) //External interrupt EXT_INT7 filter configuration register 1  0x0000_0000
#define EXT_INT13_FLTCON0 			REGL(GPIOBASE_PART1,0x0860) //External interrupt EXT_INT13 filter configuration register 0 0x0000_0000
#define EXT_INT13_FLTCON1 			REGL(GPIOBASE_PART1,0x0864) //External interrupt EXT_INT13 filter configuration register 1 0x0000_0000
#define EXT_INT14_FLTCON0 			REGL(GPIOBASE_PART1,0x0868) //External interrupt EXT_INT14 filter configuration register 0 0x0000_0000
#define EXT_INT14_FLTCON1 			REGL(GPIOBASE_PART1,0x086C) //External interrupt EXT_INT14 filter configuration register 1 0x0000_0000
#define EXT_INT15_FLTCON0 			REGL(GPIOBASE_PART1,0x0870) //External interrupt EXT_INT15 filter configuration register 0 0x0000_0000
#define EXT_INT15_FLTCON1 			REGL(GPIOBASE_PART1,0x0874) //External interrupt EXT_INT15 filter configuration register 1 0x0000_0000
#define EXT_INT16_FLTCON0 			REGL(GPIOBASE_PART1,0x0878) //External interrupt EXT_INT16 filter configuration register 0 0x0000_0000
#define EXT_INT16_FLTCON1 			REGL(GPIOBASE_PART1,0x087C) //External interrupt EXT_INT16 filter configuration register 1 0x0000_0000
#define EXT_INT21_FLTCON0 			REGL(GPIOBASE_PART1,0x0880) //External interrupt EXT_INT21 filter configuration register 0 0x0000_0000
#define EXT_INT21_FLTCON1 			REGL(GPIOBASE_PART1,0x0884) //External interrupt EXT_INT21 filter configuration register 1 0x0000_0000
#define EXT_INT22_FLTCON0 			REGL(GPIOBASE_PART1,0x0888) //External interrupt EXT_INT22 filter configuration register 0 0x0000_0000
#define EXT_INT22_FLTCON1 			REGL(GPIOBASE_PART1,0x088C) //External interrupt EXT_INT22 filter configuration register 1 0x0000_0000
#define EXT_INT1_MASK     			REGL(GPIOBASE_PART1,0x0900) //External interrupt EXT_INT1 mask register                    0x0000_00FF
#define EXT_INT2_MASK     			REGL(GPIOBASE_PART1,0x0904) //External interrupt EXT_INT2 mask register                    0x0000_003F
#define EXT_INT3_MASK     			REGL(GPIOBASE_PART1,0x0908) //External interrupt EXT_INT3 mask register                    0x0000_00FF
#define EXT_INT4_MASK     			REGL(GPIOBASE_PART1,0x090C) //External interrupt EXT_INT4 mask register                    0x0000_001F
#define EXT_INT5_MASK     			REGL(GPIOBASE_PART1,0x0910) //External interrupt EXT_INT5 mask register                    0x0000_001F
#define EXT_INT6_MASK     			REGL(GPIOBASE_PART1,0x0914) //External interrupt EXT_INT6 mask register                    0x0000_000F
#define EXT_INT7_MASK     			REGL(GPIOBASE_PART1,0x0918) //External interrupt EXT_INT7 mask register                    0x0000_000F
#define EXT_INT13_MASK    			REGL(GPIOBASE_PART1,0x0930) //External interrupt EXT_INT13 mask register                   0x0000_00FF
#define EXT_INT14_MASK    			REGL(GPIOBASE_PART1,0x0934) //External interrupt EXT_INT14 mask register                   0x0000_00FF
#define EXT_INT15_MASK    			REGL(GPIOBASE_PART1,0x0938) //External interrupt EXT_INT15 mask register                   0x0000_00FF
#define EXT_INT16_MASK    			REGL(GPIOBASE_PART1,0x093C) //External interrupt EXT_INT16 mask register                   0x0000_003F
#define EXT_INT21_MASK    			REGL(GPIOBASE_PART1,0x0940) //External interrupt EXT_INT21 mask register                   0x0000_00FF
#define EXT_INT22_MASK    			REGL(GPIOBASE_PART1,0x0944) //External interrupt EXT_INT22 mask register                   0x0000_001F
#define EXT_INT1_PEND     			REGL(GPIOBASE_PART1,0x0A00) //External interrupt EXT_INT1 pending register                 0x0000_0000
#define EXT_INT2_PEND     			REGL(GPIOBASE_PART1,0x0A04) //External interrupt EXT_INT2 pending register             0x0000_0000
#define EXT_INT3_PEND     			REGL(GPIOBASE_PART1,0x0A08) //External interrupt EXT_INT3 pending register             0x0000_0000
#define EXT_INT4_PEND     			REGL(GPIOBASE_PART1,0x0A0C) //External interrupt EXT_INT4 pending register             0x0000_0000
#define EXT_INT5_PEND     			REGL(GPIOBASE_PART1,0x0A10) //External interrupt EXT_INT5 pending register             0x0000_0000
#define EXT_INT6_PEND     			REGL(GPIOBASE_PART1,0x0A14) //External interrupt EXT_INT6 pending register             0x0000_0000
#define EXT_INT7_PEND     			REGL(GPIOBASE_PART1,0x0A18) //External interrupt EXT_INT7 pending register             0x0000_0000
#define EXT_INT13_PEND    			REGL(GPIOBASE_PART1,0x0A30) //External interrupt EXT_INT13 pending register            0x0000_0000
#define EXT_INT14_PEND    			REGL(GPIOBASE_PART1,0x0A34) //External interrupt EXT_INT14 pending register            0x0000_0000
#define EXT_INT15_PEND    			REGL(GPIOBASE_PART1,0x0A38) //External interrupt EXT_INT15 pending register            0x0000_0000
#define EXT_INT16_PEND    			REGL(GPIOBASE_PART1,0x0A3C) //External interrupt EXT_INT16 pending register            0x0000_0000
#define EXT_INT21_PEND    			REGL(GPIOBASE_PART1,0x0A40) //External interrupt EXT_INT21 pending register            0x0000_0000
#define EXT_INT22_PEND    			REGL(GPIOBASE_PART1,0x0A44) //External interrupt EXT_INT22 pending register            0x0000_0000
#define EXT_INT_SERVICE_XB      	REGL(GPIOBASE_PART1,0x0B08) //Current service register                                 0x0000_0000
#define EXT_INT_SERVICE_PEND_XB 	REGL(GPIOBASE_PART1,0x0B0C) //Current service pending register                         0x0000_0000
#define EXT_INT_GRPFIXPRI_XB    	REGL(GPIOBASE_PART1,0x0B10) //External interrupt group fixed priority control register 0x0000_0000
#define EXT_INT1_FIXPRI   			REGL(GPIOBASE_PART1,0x0B14) //External interrupt 1 fixed priority control register     0x0000_0000
#define EXT_INT2_FIXPRI   			REGL(GPIOBASE_PART1,0x0B18) //External interrupt 2 fixed priority control register     0x0000_0000
#define EXT_INT3_FIXPRI   			REGL(GPIOBASE_PART1,0x0B1C) //External interrupt 3 fixed priority control register     0x0000_0000
#define EXT_INT4_FIXPRI   			REGL(GPIOBASE_PART1,0x0B20) //External interrupt 4 fixed priority control register     0x0000_0000
#define EXT_INT5_FIXPRI   			REGL(GPIOBASE_PART1,0x0B24) //External interrupt 5 fixed priority control register     0x0000_0000
#define EXT_INT6_FIXPRI   			REGL(GPIOBASE_PART1,0x0B28) //External interrupt 6 fixed priority control register     0x0000_0000
#define EXT_INT7_FIXPRI   			REGL(GPIOBASE_PART1,0x0B2C) //External interrupt 7 fixed priority control register     0x0000_0000
#define EXT_INT13_FIXPRI  			REGL(GPIOBASE_PART1,0x0B44) //External interrupt 13 fixed priority control register    0x0000_0000
#define EXT_INT14_FIXPRI  			REGL(GPIOBASE_PART1,0x0B48) //External interrupt 14 fixed priority control register    0x0000_0000
#define EXT_INT15_FIXPRI  			REGL(GPIOBASE_PART1,0x0B4C) //External interrupt 15 fixed priority control register    0x0000_0000
#define EXT_INT16_FIXPRI  			REGL(GPIOBASE_PART1,0x0B50) //External interrupt 16 fixed priority control register    0x0000_0000
#define EXT_INT21_FIXPRI  			REGL(GPIOBASE_PART1,0x0B54) //External interrupt 21 fixed priority control register    0x0000_0000
#define EXT_INT22_FIXPRI  			REGL(GPIOBASE_PART1,0x0B58) //External interrupt 22 fixed priority control register    0x0000_0000
#define PDNEN             			REGL(GPIOBASE_PART1,0x0F80) //Power down mode pad configure register                       0x00


//RTC registers
#define RTCBASE    0x10070000    //RTC Base Address: 0x1007_0000
#define INTP       REGL(RTCBASE,0x0030) 		//Specifies the interrupt pending register   
#define RTCCON     REGL(RTCBASE,0x0040) 		//Specifies the RTC control register         
#define TICCNT     REGL(RTCBASE,0x0044) 		//Specifies the tick time count register     
#define RTCALM     REGL(RTCBASE,0x0050) 		//Specifies the RTC alarm control register   
#define ALMSEC     REGL(RTCBASE,0x0054) 		//Specifies the alarm second data register   
#define ALMMIN     REGL(RTCBASE,0x0058) 		//Specifies the alarm minute data register   
#define ALMHOUR    REGL(RTCBASE,0x005C) 		//Specifies the alarm hour data register     
#define ALMDAY     REGL(RTCBASE,0x0060) 		//Specifies the alarm day data register      
#define ALMMON     REGL(RTCBASE,0x0064) 		//Specifies the alarm month data register    
#define ALMYEAR    REGL(RTCBASE,0x0068) 		//Specifies the alarm year data register     
#define BCDSEC     REGL(RTCBASE,0x0070) 		//Specifies the BCD second register          
#define BCDMIN     REGL(RTCBASE,0x0074) 		//Specifies the BCD minute register          
#define BCDHOUR    REGL(RTCBASE,0x0078) 		//Specifies the BCD hour register            
#define BCDDAYWEEK REGL(RTCBASE,0x007C) 		//Specifies the BCD day of the week register 
#define BCDDAY     REGL(RTCBASE,0x0080) 		//Specifies the BCD day register             
#define BCDMON     REGL(RTCBASE,0x0084) 		//Specifies the BCD month register           
#define BCDYEAR    REGL(RTCBASE,0x0088) 		//Specifies the BCD year register            
#define CURTICCNT  REGL(RTCBASE,0x0090) 		//Specifies the current tick time counter register 

#define UART0BASE    0X13800000                 //UART0 base address
#define ULCON0    REGL(UART0BASE,0x0000) 		//Specifies line control             0x0000_0000
#define UCON0     REGL(UART0BASE,0x0004) 		//Specifies control                  0x0000_3000
#define UFCON0    REGL(UART0BASE,0x0008) 		//Specifies FIFO control             0x0000_0000
#define UMCON0    REGL(UART0BASE,0x000C) 		//Specifies modem control            0x0000_0000
#define UTRSTAT0  REGL(UART0BASE,0x0010) 		//Specifies Tx/Rx status             0x0000_0006
#define UERSTAT0  REGL(UART0BASE,0x0014) 		//Specifies Rx error status          0x0000_0000
#define UFSTAT0   REGL(UART0BASE,0x0018) 		//Specifies FIFO status              0x0000_0000
#define UMSTAT0   REGL(UART0BASE,0x001C) 		//Specifies modem status             0x0000_0000
#define UTXH0     REGL(UART0BASE,0x0020) 		//Specifies transmit buffer           Undefined
#define URXH0     REGL(UART0BASE,0x0024) 		//Specifies receive buffer           0x0000_0000
#define UBRDIV0   REGL(UART0BASE,0x0028) 		//Specifies baud rate divisor        0x0000_0000
#define UFRACVAL0 REGL(UART0BASE,0x002C) 		//Specifies divisor fractional value 0x0000_0000
#define UINTP0    REGL(UART0BASE,0x0030) 		//Specifies interrupt pending        0x0000_0000
#define UINTSP0   REGL(UART0BASE,0x0034) 		//Specifies interrupt source pending 0x0000_0000
#define UINTM0    REGL(UART0BASE,0x0038) 		//Specifies interrupt mask           0x0000_0000


#endif	//__HARDWARE_H
