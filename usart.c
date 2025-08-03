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
		usart_transmit_byte(usart, buff[byte_idx]);
	}
	while ((usart->USART_ISR & BIT(6)) == 0);
}

char usart_read_byte(struct usart* usart)
{
	while ((usart->USART_ISR & BIT(5)) == 0); // block until char received
	// note below assumed 7 or 8 data bits. Note currently functional for 9.
	return (char)(MASK(7) & usart->USART_RDR);
}

void usart_read_bytes(struct usart* usart, char buff[], uint32_t buff_size, char termchar)
{
	// this function reads until a user specified termintor termchar is found
	// and places bytes in buffer buff. Each time a byte is read,
	// the byte is placed in buffer and the buffer pointer incremented by one.
	// if space remains in buffer, remaining elements are zeroed
	// There should be no need to reset the pointer as it is passed by value.
	// THIS FUNCTION DOES NOT CHECK IF num_bytes > len(buff).

	char cur_byte = 0;
	uint32_t byte_idx = 0;

	for (; byte_idx < buff_size; byte_idx++)
	{
		cur_byte = usart_read_byte(usart);

		if (cur_byte!=termchar) // end on termchar
		{
			buff[byte_idx] = cur_byte; // append data
		}
		else
		{
			// termchar not placed in buffer
			break;
		}
	}

	for (; byte_idx < buff_size; byte_idx++)
	{
		buff[byte_idx] = 0; // pad remainder of buff with 0s
	}
}