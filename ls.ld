ENTRY(main)

_end_stack = ORIGIN(SRAM) + LENGTH(SRAM);    /* end of RAM */

MEMORY
{
    FLASH(rx):ORIGIN =0x08000000,LENGTH =256K /* 16 KByte */
    SRAM(rwx):ORIGIN =0x20000000,LENGTH =64K /* 2 KByte */
}

SECTIONS
{
    .isr_vector :
    {
	. = ALIGN(4);

	/* KEEP required to link with option -Wl,--gc-sections */
	KEEP(*(.isr_vector))
	. = ALIGN(4);
	__end_isr_vector = .; /* debug */
    } > FLASH

    .text :
    {	
	__start_text = .;
        *(.text)
	*(.text*)
	KEEP (*(.init))
    	KEEP (*(.fini))
        . = ALIGN(4);
        __end_text = .;
    } > FLASH

    .rodata :
    {
	. = ALIGN(4);
	*(.rodata)
	*(.rodata*)
	. = ALIGN(4);
    } > FLASH

    __la_data = LOADADDR(.data);

    .data :
    {
	. = ALIGN(4);
        __start_data = .;
        *(.data)
	*(.data*)
        . = ALIGN(4);
        __end_data = .;
    } > SRAM AT> FLASH /* SRAM = final location, FLASH = initial location */

    .bss :
    {
	. = ALIGN(4);
        __start_bss = .;
	__bss_start__ = __start_bss;
        *(.bss)
	*(.bss*)

	/* https://stackoverflow.com/questions/28474675/arm-common-section-and-fno-common-flag */
    	*(COMMON)
	    
        *(.noinit)
        . = ALIGN(4);
        __end_bss = .;
	__bss_end__ = __end_bss;
    } > SRAM
}