#include <stdbool.h>
#include <stdint.h>

#define PERIPH_BASE ((uint32_t)0x40000000)

// Peripheral memory map
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000)
#define AHB2PERIPH_BASE (PERIPH_BASE + 0x08000000)

// AHB1 peripherals
#define RCC_BASE (AHB1PERIPH_BASE + 0x00001000)

// AHB2 peripherals
#define GPIOA_BASE (AHB2PERIPH_BASE + 0x0000)
#define GPIOB_BASE (AHB2PERIPH_BASE + 0x0400)
#define GPIOC_BASE (AHB2PERIPH_BASE + 0x0800)
#define GPIOD_BASE (AHB2PERIPH_BASE + 0x0C00)
#define GPIOE_BASE (AHB2PERIPH_BASE + 0x1000)
#define GPIOF_BASE (AHB2PERIPH_BASE + 0x1400)

#define RCC_AHBENR_GPIOAEN ((uint32_t)0x00020000)
#define RCC_AHBENR_GPIOBEN ((uint32_t)0x00040000)
#define RCC_AHBENR_GPIOCEN ((uint32_t)0x00080000)
#define RCC_AHBENR_GPIODEN ((uint32_t)0x00100000)
#define RCC_AHBENR_GPIOFEN ((uint32_t)0x00400000)
#define RCC_AHBENR_GPIOEN                                                     \
  (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN               \
   | RCC_AHBENR_GPIODEN | RCC_AHBENR_GPIOFEN)

/*
 * Reset and Clock Control (RCC)
 *  page 130 from rm0365
 */
typedef struct {
  volatile uint32_t CR;
  volatile uint32_t CFGR;
  volatile uint32_t CIR;
  volatile uint32_t APB2RSTR;
  volatile uint32_t APB1RSTR;
  volatile uint32_t AHBENR;
  volatile uint32_t APB2ENR;
  volatile uint32_t APB1ENR;
  volatile uint32_t BDCR;
  volatile uint32_t CSR;
  volatile uint32_t AHBRSTR;
  volatile uint32_t CFGR2;
  volatile uint32_t CFGR3;
} RCC_Type;

#define RCC ((RCC_Type *)RCC_BASE)

/*
 * General-purpose I/Os (GPIO)
 *  page 167 from rm0365
 */
typedef struct {
  volatile uint32_t MODER;
  volatile uint16_t OTYPER;
  volatile uint16_t RESERVED0;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint16_t IDR;
  volatile uint16_t RESERVED1;
  volatile uint16_t ODR;
  volatile uint16_t RESERVED2;
  volatile uint32_t BSRR;
  volatile uint32_t LCKR;
  volatile uint32_t AFR[2];
  volatile uint16_t BRR;
  volatile uint16_t RESERVED3;
} GPIO_Type;

#define GPIOA ((GPIO_Type *)GPIOA_BASE)
#define GPIOB ((GPIO_Type *)GPIOB_BASE)
#define GPIOC ((GPIO_Type *)GPIOC_BASE)
#define GPIOD ((GPIO_Type *)GPIOD_BASE)
#define GPIOF ((GPIO_Type *)GPIOF_BASE)

/*
 * General-purpose I/Os (GPIO) module
 *  page 167 from rm0365
 */
enum {
  GPIO_MODE_INPUT,
  GPIO_MODE_OUTPUT,
  GPIO_MODE_AF,
  GPIO_MODE_ANALOG,
};

static inline void
spin(volatile uint32_t count)
{
  while (count--) {
    (void)0;
  }
}

int
main(void)
{
  // Enable clock for GPIOB peripheral
  RCC->AHBENR |= RCC_AHBENR_GPIOEN;

  // Configure PXXX as output
  GPIOB->MODER &= ~(3U << (13 * 2));
  GPIOB->MODER |= (GPIO_MODE_OUTPUT & 3U)
                  << (13 * 2); // Set MODERX bits to 01 (output mode)

  for (;;) {
    // Set PXXX high
    GPIOB->BSRR = (1U << 13) << (true ? 0 : 16);
    spin(999999);
    // Set PXXX low
    GPIOB->BSRR = (1U << 13) << (false ? 0 : 16);
    spin(999999);
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
