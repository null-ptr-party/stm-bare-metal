#include "uart.h"
#include "shared_tools.h"

// setup functions
void setup_usart(struct usart* usart, struct usart_cfg cfg)
{
	// set worlen
	// m0
	usart->USART_CR1 |= (0x01 & cfg->word_len) << 12;
	// m1
	usart->USART_CR1 |= (0x01 & cfg->word_len) << 28;
	// set baud rate. Note currently only supporting over16
	usart->USART_BRR &= ~(MASK(16)); // clear current value
	usart->USART_BRR |= (MASK(16) & cfg->baud);
	// keep stop bits at default (1) for now
	// set stop bit placeholder
	// enable usart
	usart->USART_CR1 |= BIT(0);

}

void enable_tx(struct usart* usart)
{
	usart |= BIT(3);
}

void disable_tx(struct usart* usart)
{
	usart &= ~BIT(3);
}