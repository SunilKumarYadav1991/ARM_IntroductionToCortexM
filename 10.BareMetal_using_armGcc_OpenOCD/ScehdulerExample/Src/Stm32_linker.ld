ENTRY(Reset_Handler)

/* Memories definition */
MEMORY
{
  SRAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 128K
  FLASH    (rx)    : ORIGIN = 0x8000000,   LENGTH = 512K
}

/* Sections */
SECTIONS
{
/*Code in flash memory is organized in following order to work orrectlyly
Vector Table | .txt | .rodata |  .data | */
   .text :
   {
      
	  *(.isr_vector)
	  *(.text)        /*Verbose: .txt of main.o led.o startup.o*/
	  *(.rodata)
	  . = ALIGN(4);    /* location counter is word aligned*/
	  _etext = .;     /*symbol for end of text section. Note: symbol tracks VMA address*/
   } > FLASH
	
	.data :
	{
	  _sdata = .;     /*symbol declaration should followed by whitespace before and after = */
	  *(.data)
	  . = ALIGN(4);    /* location counter is word aligned*/
	  _edata = .;
	} >SRAM AT> FLASH
	
	.bss :
	{
	  _sbss = .;
	  *(.bss)
	   . = ALIGN(4);    /* location counter is word aligned*/
	  _ebss = .;
	} >SRAM

}