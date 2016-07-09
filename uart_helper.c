#include "uart_helper.h"

volatile uint8_t tx_flag = 0;

volatile buf_struct gps_buf =
{
	.buf_ptr = &common_buf[0],
	.ptr = &common_buf[0],
	.len = GPS_BUF_LEN,
	.eol = {0},
	//use 0th index for 1st string
	.eol_i = 1
};

void GPS_SERIAL_Handler(void)
{
	uint32_t ua_status;
	ua_status = usart_get_status(GPS_SERIAL);
	if(ua_status & US_CSR_RXRDY)
	{	
		usart_getchar(GPS_SERIAL, &ua_status);
		
		*(gps_buf.ptr) = ua_status&0xff;
		if(*(gps_buf.ptr) == '\n')
			tx_flag = 1;
		//uart_write(DEBUG_SERIAL, *(gps_buf.ptr));
		
		if (*(gps_buf.ptr) == '\n' && *(gps_buf.ptr-1) == '\r')
		{
			//make the "\r\n" null characters for ease
			*(gps_buf.ptr) = *(gps_buf.ptr-1) = 0;
			gps_buf.eol[(gps_buf.eol_i++)%8] = ++gps_buf.ptr - gps_buf.buf_ptr;
		}
				
		/*
		if(gps_buf.ptr > &gps_buf.buf_ptr[gps_buf.len])
			gps_buf.ptr = gps_buf.buf_ptr;
		*/
	}
}


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