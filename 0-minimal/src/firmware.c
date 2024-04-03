int
main(void)
{
  for (;;) {
    (void)0;
  }

  return 0;
}

// Startup code
__attribute__((naked, noreturn)) void
_reset(void)
{
  // memset .bss to zero, and copy .data section to RAM region
  extern unsigned long _sbss, _ebss, _sdata, _edata, _sidata;

  for (unsigned long *dst = &_sbss; dst < &_ebss; dst++) {
    *dst = 0;
  }

  for (unsigned long *dst = &_sdata, *src = &_sidata; dst < &_edata;) {
    *dst++ = *src++;
  }

  main();

  for (;;) {
    (void)0; // Infinite loop in the case if main() returns
  }
}

extern void _estack(void); // Defined in link.ld

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const vector_table[])(void)
    = { _estack, _reset };
