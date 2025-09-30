#include "gpio.h"
#include "shared_tools.h"
#include <stdint.h>
// GPIO source code

void set_gpio_mode(struct gpio *gpio_port, uint8_t pin_num, enum pinmode mode)
{	// sets the gpio mode for the port/pin.
	gpio_port->MODER &= ~(0x03U << (2 * (pin_num & 0xFF))); // clear 2 bit field at pin position. keep rest.
	gpio_port->MODER |= ((mode & 0x03U) << (2U * (pin_num & 0xFF))); // set bits based off mode.
}

void set_gpio_speed(struct gpio *gpio_port, uint8_t pin_num, enum iospeed spd)
{	// sets GPIO speed for the given port/pin.
	gpio_port->SPEEDR &= ~(0x03U << (2U * pin_num)); // clear 2 bit field at pin position. keep rest.
	gpio_port->SPEEDR |= ((spd & 0x03U) << (2U * pin_num)); // set bits based off speed.
}

void set_gpio_output(struct gpio *gpio_port, uint8_t pin_num)
{	// set (turn on) the gpio output.
	gpio_port->BSRR |= (0x01U << (pin_num)); // set bit.
}

void reset_gpio_output(struct gpio *gpio_port, uint8_t pin_num)
{	// reset (turn off) the gpio output.
	gpio_port->BSRR |= (0x01U << (pin_num + 16U)); // set the reset bit
}

void set_alt_func(struct gpio *gpio_port, uint8_t pin_num, uint8_t af_num)
{	// set alternate function for gpio pin/port.
	// note that AF mappings are available in the STM32H723ZG datasheet.
	pin_num &= (0x0fU & pin_num); // ensure pin num doesnt exceed 15.

	// pins 0 to 7 use AFRL. 8 to 15 use AFRH.
	if (pin_num <= 7)
	{
		gpio_port->AFRL |= (af_num << pin_num * 4);
	}
	else
	{
		gpio_port->AFRH |= (af_num << (pin_num - 8) * 4);
	}
	
}

void set_gpio_pullup(struct gpio *gpio_port, uint8_t pin_num, enum pulltype pull_type)
{	// configure pullup option for pin/port.
	gpio_port->PUPDR &= ~(0x03U << (2 * pin_num)); // Clear bit
	gpio_port->PUPDR |= (pull_type << (2 * pin_num)); // set mode by in
}

void set_gpio_outtype(struct gpio *gpio_port, uint8_t pin_num, enum outtype outtype)
{	// set output type for pin/port.
	gpio_port->OTYPER &= ~(0x01U << (pin_num)); // Clear bit
	gpio_port->OTYPER |= (outtype << (pin_num)); // set mode by pin
}