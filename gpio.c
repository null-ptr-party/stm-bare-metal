#include "gpio.h"
#include <stdint.h>
// GPIO source code

void set_gpio_mode(struct gpio *gpio_port, uint8_t pin_num, enum pinmode mode)
{
	gpio_port->MODER &= ~(0x03U << (2 * (pin_num & 0xFF))); // clear 2 bit field at pin position. keep rest.
	gpio_port->MODER |= ((mode & 0x03U) << (2U * (pin_num & 0xFF))); // set bits based off mode.
}

void set_gpio_speed(struct gpio *gpio_port, uint8_t pin_num, enum iospeed spd)
{
	gpio_port->SPEEDR &= ~(0x03U << (2U * pin_num)); // clear 2 bit field at pin position. keep rest.
	gpio_port->SPEEDR |= ((spd & 0x03U) << (2U * pin_num)); // set bits based off speed.
}

void set_gpio_output(struct gpio *gpio_port, uint8_t pin_num)
{
	// Note: No clear bit here. Using clear bit messes this up.
	gpio_port->BSRR |= (0x01U << (pin_num)); // set bit.
}

void reset_gpio_output(struct gpio *gpio_port, uint8_t pin_num)
{
	gpio_port->BSRR |= (0x01U << (pin_num + 16U)); // set the reset bit
}