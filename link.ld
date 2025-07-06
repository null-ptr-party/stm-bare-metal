ENTRY(_reset);

MEMORY {
	flash(rx)	: ORIGIN = 0x08000000, LENGTH = 1024K
	tcram(rwx)	: ORIGIN = 0x20000000, LENGTH = 128K
}

_estack =ORIGIN(tcram) + LENGTH(tcram);

/* flash sections */
SECTIONS {
.vectors	: { KEEP(*(.vectors)) }   > flash /* Keep tells linker to keep .vectors even if unreferenced*/
.text		: { *(.text*) }           > flash
.rodata		: { *(.rodata*) }         > flash

/* data in ram */
  .data : {
    _sdata = .;   /* .data section start */
    *(.first_data) /* asterisk before indicates match all files. asterisk inside is used to match any section*/
    *(.data SORT(.data.*))
    _edata = .;  /* .data section end */
  } > tcram AT > flash
  _sidata = LOADADDR(.data); /* data loaded at flash but moved to RAM */

/* bss */
  .bss : {
    _sbss = .;              /* .bss section start */
    *(.bss SORT(.bss.*) COMMON)
    _ebss = .;              /* .bss section end */
  } > tcram

  . = ALIGN(8); /* Align on 8 byte boundary */
  _end = .;
}