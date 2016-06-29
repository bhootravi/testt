/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "tracker.h"
//#include "ASF/sam/utils/cmsis/sam4s/include/component/component_uart.h"
int gps_data_rx = 0;

uint8_t common_buf[200];
volatile uint8_t tx_flag = 0;
uint8_t* buf_tx_ptr = &common_buf[0];

void GPS_SERIAL_Handler(void)
{
	uint32_t ua_status;
	ua_status = usart_get_status(GPS_SERIAL);
	if(ua_status & US_CSR_RXRDY)
	{
		uint32_t t;
		usart_getchar(GPS_SERIAL, &t);
		uart_write(DEBUG_SERIAL, t&0xff);
	}
}
	
/*
int self_test(void)
{
	//component_check(gps)
	//component_check(file sys)
	//component_check(usb)
	//sufficient mem available
	return 0;
}
*/

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	
	board_init();
	
	uart_h_init();
	//sam_uart_opt uart0_settings;
	/* Insert application code here, after the board has been initialized. */
	int t = 0;
	
	common_buf[0] = 'a';
	common_buf[1] = 'b';
	common_buf[2] = 'c';
	//int x = sysclk_get_peripheral_hz();
	//itoa(x, &common_buf[3], 8);
	common_buf[3] = 'd';
	
	while(1)
	{
		if(t < 10 && uart_is_tx_ready(DEBUG_SERIAL))
		{
			uart_write(DEBUG_SERIAL, *buf_tx_ptr);
			buf_tx_ptr++;
			tx_flag = 1;
			if((buf_tx_ptr == &common_buf[4]))
			{
				buf_tx_ptr = &common_buf[0];
				t++;
			}
		}
		/*
		if(t < 10 && usart_is_tx_ready(USART_SERIAL))
		{
			t++;
			usart_write(USART_SERIAL, 'a');
		}
		*/
	}
	/*
	int x = 0;
	
	if(self_test())
	{
		//self test failed,
		//log
	}
	
	//loop start
	if((x = key_pressed()))
	{
		
	}
	*/
	/*
	gps_data_rx = 1;
	if(gps_data_rx)
	{
		tracker_process();
	}
	
	if(gps_avilable())
	{
		gps_update_location();
		gps_avg_speed();
		gps_avg_speed_from(&prev_station);
		gps_distance_from(&next_station);
		gps_distance_from(&dest_station);
		
	}
	*/
}
