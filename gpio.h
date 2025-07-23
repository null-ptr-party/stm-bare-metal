#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// STRUCTS
// Struct for gpio control registers
// MODER: Pin Mode. 00-> Input, 01->GPIO, 10->alternate func, 11->analog mode
// TYPER: Output Type. 0->push/pull, 1->open drain
// SPEEDR: Speed. 00-> Low, 01->Med, 10->high, 11->very high
// PUPDR: Pull up/down. 00->None, 01->pull up, 10->pull down, 11->res
// IDR: Input Data. Read Only
// ODR: Output Data. [Bits 15:0]
// BSRR: Bit Set/Reset. 31:16 portx reset. 0->no action, 1->reset ODR bit.
// 15:0 portx set. 0->no action, 1->set ODR bit
// LCKR: Config Lock. 15:0 0->not locked, 1->locked
// AFRL: Alternate Function Low. 31:0. See 11.4.9 of ref manual
// AFRH: Alternate Function High. 31:0. See 11.4.10 of ref manual
struct gpio
{
	volatile uint32_t MODER, TYPER, SPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFRL, AFRH;
};

// GPIO macros
#define GPIOA (struct gpio *)0x58020000
#define GPIOB (struct gpio *)0x58020400
#define GPIOC (struct gpio *)0x58020800
#define GPIOD (struct gpio *)0x58020C00
#define GPIOE (struct gpio *)0x58021000
#define GPIOF (struct gpio *)0x58021400
#define GPIOG (struct gpio *)0x58021800
#define GPIOH (struct gpio *)0x58021C00
#define GPIOJ (struct gpio *)0x58022400
#define GPIOK (struct gpio *)0x58022800

// ENUMS
// pinmode
enum pinmode {GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_ALT_FUNC, GPIO_MODE_ANALOG};
// output type
enum outtype {OUTTYPE_PUSHPULL, OUTTYPE_OPEN_DRAIN};
// iospeed
enum iospeed {IOSPEED_LOW, IOSPEED_MED, IOSPEED_HIGH, IOSPEED_VHIGH};
// pulltype
enum pulltype {PULLTYPE_NONE, PULLTYPE_PULLUP, PULLTYPE_PULLDOWN};
// lock setting
enum cnfg_lock {LCK_CFG_NOTLOCKED, LCK_CFG_LOCKED};
// alternate function
enum alt_func {AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7,
AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15};

enum bank {
	GPIO_BANK_A, GPIO_BANK_B, GPIO_BANK_C, GPIO_BANK_D, GPIO_BANK_E, GPIO_BANK_F, GPIO_BANK_G,
	GPIO_BANK_H, GPIO_BANK_J = 10, GPIO_BANK_K = 11
}; // Note between banks H and J there is a larger offset.

// functions
void set_gpio_mode(struct gpio *gpio_port, uint8_t pin_num, enum pinmode mode);

void set_gpio_speed(struct gpio *gpio_port, uint8_t pin_num, enum iospeed spd);

void set_gpio_output(struct gpio *gpio_port, uint8_t pin_num);

void reset_gpio_output(struct gpio *gpio_port, uint8_t pin_num);

void set_alt_func(struct gpio *gpio_port, uint8_t pin_num, uint8_t af_num);

#endif
