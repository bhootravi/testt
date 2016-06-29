#include "uart_helper.h"

//const sam_uart_opt_t uart0_settings;


void uart_h_init()
{
	#define CONF_BOARD_UART_CONSOLE
	uint32_t xxx = sysclk_get_peripheral_hz();
	const sam_uart_opt_t debug_usart_settings = {
		xxx,
		9600,
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
	
	const sam_usart_opt_t gps_usart_settings = {
		GPS_SERIAL_BAUDRATE,
		GPS_SERIAL_CHAR_LENGTH,
		GPS_SERIAL_PARITY,
		GPS_SERIAL_STOP_BIT,
		US_MR_CHMODE_NORMAL
	};	
	
	sysclk_enable_peripheral_clock(GPS_SERIAL_ID);
	usart_init_rs232(GPS_SERIAL, &gps_usart_settings, 
						sysclk_get_peripheral_hz());
	usart_enable_rx(GPS_SERIAL);
	usart_enable_tx(GPS_SERIAL);
	usart_disable_interrupt(GPS_SERIAL, ALL_INTERRUPT_MASK);
	usart_enable_interrupt(GPS_SERIAL, US_IER_RXRDY);
	NVIC_EnableIRQ(GPS_SERIAL_IRQn);	
}