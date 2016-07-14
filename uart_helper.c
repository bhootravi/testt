#include "uart_helper.h"

void uart_h_init()
{
	#define CONF_BOARD_UART_CONSOLE
	//use peripheral clk and not cpu clk
	// uart can be used as usart with limitations 
	// (reading status register doesnt work)
	uint32_t xxx = sysclk_get_peripheral_hz();
	const sam_uart_opt_t debug_usart_settings = {
		xxx,
		115200,
		UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL
	};
	/*
	const sam_usart_opt_t debug_usart_settings = {
		DEBUG_SERIAL_BAUDRATE,
		DEBUG_SERIAL_CHAR_LENGTH,
		DEBUG_SERIAL_PARITY,
		DEBUG_SERIAL_STOP_BIT,
		US_MR_CHMODE_NORMAL
	};
	*/
	sysclk_enable_peripheral_clock(DEBUG_SERIAL_ID);
		
	uart_init(DEBUG_SERIAL, &debug_usart_settings);
	uart_enable(DEBUG_SERIAL);
	uart_disable_interrupt(DEBUG_SERIAL, ALL_INTERRUPT_MASK);
	//uart_enable_interrupt(DEBUG_SERIAL, UART_IER_RXRDY);
	
	/*
	usart_init_rs232(DEBUG_SERIAL, &debug_usart_settings, 
						sysclk_get_peripheral_hz());
	usart_enable_tx(DEBUG_SERIAL);
	usart_enable_rx(DEBUG_SERIAL);
	usart_disable_interrupt(DEBUG_SERIAL, ALL_INTERRUPT_MASK);
	usart_enable_interrupt(DEBUG_SERIAL, US_IER_TXRDY);
	*/
	
	/* Configure and enable interrupt of USART. */
	//NVIC_EnableIRQ(DEBUG_SERIAL_IRQn);	
}