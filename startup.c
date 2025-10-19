// below are defined in linker script. Hence extern
extern void _estack(void);
extern long _sdata, _edata, _sidata, _sbss, _ebss;

// Interrupt handler prototypes
extern void systick_handler(void);
extern void exti13_iqr(void);

// _reset is called on boot
void _reset(void);
// main function prototype
extern int main(void);

// Create section for NVIC vector table
__attribute__((section(".vectors"))) void(*const table[16 + 163])(void) = {
	// set _estack, reset to first elements of table.
	_estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, systick_handler, // systick is the 16th word
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, exti13_iqr
};

__attribute__((naked, noreturn)) void _reset(void)
{
	for (long *dst = &_sdata, *source = &_sidata; dst < &_edata;)
	{
		*dst++ = *source++; // Copy data from flash to RAM
	}

	for (long *ptr = &_sbss; ptr < &_ebss; ptr++)
	{
		*ptr = 0; // zero sbss data in RAM
	}

	main(); // call main

	for (;;) (void) 0; // loop forever if main returns.
}