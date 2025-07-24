#ifndef _UART_H
#define _UART_H

#include <stdint.h>

// usart configuration struct. Note this is struct does not
// currently expose all usart options.
struct usart_cfg
{
	uint8_t fifo_en, word_len, eob_intrpt_en, rcvr_tmt_intrpt_en, drvr_en_assert_time,
		drvr_en_deassert_time, oversmpl_mthd, char_match_intrpt_en, mute_mode_en,
		wkeup_mthd, prty_ctrl_en, parity_sel, pe_intrpt_en, tx_cplt_intrpt,
		idle_intrpt_en, stp_bits;

	uint32_t baud;
};
// macros for usart cfg
#define OVERSAMPLE8 1U
#define OVERSAMPLE16 0U
#define DATABITS7 2U
#define DATABITS8 0U
#define DATABITS9 1U

// usart struct used for registers
struct usart
{
	volatile uint32_t USART_CR1, USART_CR2, USART_CR3, USART_BRR,
	USART_RTOR, USART_RQR, USART_ISR, USART_ICR, USART_RDR, USART_TDR, USART_PRESC;
};

struct
	// Defines USARTs
#define USART1 (struct usart *)0x40011000 // points to first register of USART (CR1)
#define USART2 (struct usart *)0x40004400
#define USART3 (struct usart *)0x40004800
// will define additional as needed

// note stlink on usart3 which is pins PD8 PD9

// setup functions
void setup_usart(struct usart* usart, struct usart_cfg cfg);
void enable_tx(struct usart* usart);
void disable_tx(struct usart* usart);


#endif