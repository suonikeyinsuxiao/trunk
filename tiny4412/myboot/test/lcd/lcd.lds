/* Script for -z combreloc: combine and sort reloc sections */
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm",
	      "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SEARCH_DIR("=/usr/local/lib"); SEARCH_DIR("=/lib"); SEARCH_DIR("=/usr/lib");
SECTIONS
{
	. = 0x40008000 ; 

	.text :
	{
		test.o(.text) ; 
		*.o(.text) ;
	}

	. = ALIGN(4);
	.rodata :
	{
		*(.rodata)	
	}
	
	. = ALIGN(4);
	.data :
	{
		*(.data)	
	}

	. = ALIGN(4);
	.bss :
	{
		*(.bss)	
	}
}
