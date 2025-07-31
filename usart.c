#include "usart.h"
#include "shared_tools.h"

// setup functions
void setup_usart(struct usart* usart, struct usart_cfg* cfg)
{
	// disable usart
	usart->USART_CR1 &= ~0x01U;
	// set worlen
	// m0
	usart->USART_CR1 |= ((0x01 & cfg->word_len) << 12);
	// m1
	usart->USART_CR1 |= ((0x01 & cfg->word_len) << 28);
	// set baud rate. Note currently only supporting over16
	usart->USART_BRR &= ~(MASK(15)); // clear current value
	usart->USART_BRR |= (MASK(15) & cfg->baud);
	// keep stop bits at default (1) for now
	// set stop bit placeholder
	// enable usart
	usart->USART_CR1 |= BIT(0) | BIT(3) | BIT(2);

}

void enable_tx(struct usart* usart)
{
	usart->USART_CR1 |= BIT(3);
}

void disable_tx(struct usart* usart)
{
	usart->USART_CR1 &= ~BIT(3);
}

void usart_transmit_byte(struct usart* usart, char byte)
{
	usart->USART_TDR = byte;
	while ((usart->USART_ISR & BIT(7)) == 0);
}

void usart_transmit_bytes(struct usart* usart, char buff[], uint32_t num_bytes)
{
	for (uint32_t byte_idx = 0; byte_idx < num_bytes; byte_idx++)
	{
		usart_transmit_byte(usart, bytes[byte_idx]);
	}
	while ((usart->USART_ISR & BIT(6)) == 0);
}