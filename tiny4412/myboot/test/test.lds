/*GNU ld (GNU Binutils) 2.20.1.20100303
  Supported emulations:
   armelf_linux_eabi
   armelfb_linux_eabi
using internal linker script:
==================================================
*/
/* Script for -z combreloc: combine and sort reloc sections */
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm",
	      "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SEARCH_DIR("=/usr/local/lib"); SEARCH_DIR("=/lib"); SEARCH_DIR("=/usr/lib");
SECTIONS
{
	. = 0x40008000;
	.text :
	{
		test.o(*) ;
		*.o(*) ;
	}

	align = 4;
}
