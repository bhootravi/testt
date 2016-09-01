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
#include <stdio.h>
#include <string.h>
#include "tracker.h"
#include "graphicLcd.h"
#include "audio.h"

/** USART Interface  : Console UART */
#define CONF_TEST_USART      CONSOLE_UART
/** Baudrate setting : 115200 */
#define CONF_TEST_BAUDRATE   115200
/** Char setting     : 8-bit character length (don't care for UART) */
#define CONF_TEST_CHARLENGTH 0
/** Parity setting   : No parity check */
#define CONF_TEST_PARITY     UART_MR_PAR_NO
/** Stopbit setting  : No extra stopbit, i.e., use 1 (don't care for UART) */
#define CONF_TEST_STOPBITS   false

char test_file_name[] = "test_16khz.wav";
/* FAT FS variables */

int gps_data_rx = 0;

station station_list[5];

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
	const usart_serial_options_t usart_serial_options = {
		.baudrate   = CONF_TEST_BAUDRATE,
		.charlength = CONF_TEST_CHARLENGTH,
		.paritytype = CONF_TEST_PARITY,
		.stopbits   = CONF_TEST_STOPBITS,
	};

	stdio_serial_init(CONF_TEST_USART, &usart_serial_options);
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	
	board_init();
	
	ioport_init();
	
	uart_h_init();
	
	/*	
	sd_mmc_init();
	
	audio_init();
	
	if(audio_open_file(test_file_name))
	{
		printf("open error\n\r");
	}
	printf("file open successful\n\r");
	*/
	gps_init();
	
	//play();
	
	//graphicLcd_init();
	//sam_uart_opt uart0_settings;
	/* Insert application code here, after the board has been initialized. */

	printf("test\n\r");
	
	
	station_list[0] = (station)
	{
		.code = "BCT",
		.location.lat = 19.0624,
		.location.lon = 72.8410,
		.halt.arrival.d = {0},
		.halt.arrival.t = {0,0,0,0},
		.halt.departure.d = {0},
		.halt.departure.t = {0,0,0,0},
		.halt.platform_no = 1,
		.halt.distance_to_next = 18.4,
		.name_eng = "BANDRA TERMINUS",
		.reg_lang = 2
	};
	
	station_list[1] = (station)
	{
		.code = "BVI",
		.location.lat = 19.2303,
		.location.lon = 72.8577,
		.halt.arrival.d = {0},
		.halt.arrival.t = {0,0,0,0},
		.halt.departure.d = {0},
		.halt.departure.t = {0,0,0,0},
		.halt.platform_no = 1,
		.halt.distance_to_next = 68.4,
		.name_eng = "BORIVALI",
		.reg_lang = 1
	};
	
	station_list[2] = (station)
	{
		.code = "BOR",
		.location.lat = 19.7983,
		.location.lon = 72.7616,
		.halt.arrival.d = {0},
		.halt.arrival.t = {0,0,0,0},
		.halt.departure.d = {0},
		.halt.departure.t = {0,0,0,0},
		.halt.platform_no = 1,
		.halt.distance_to_next = 70.2,
		.name_eng = "BOISAR",
		.reg_lang = 1
	};
	
	station_list[3] = (station)
	{
		.code = "VAPI",
		.location.lat = 20.3737,
		.location.lon = 72.9086,
		.halt.arrival.d = {0},
		.halt.arrival.t = {0,0,0,0},
		.halt.departure.d = {0},
		.halt.departure.t = {0,0,0,0},
		.halt.platform_no = 1,
		.halt.distance_to_next = 94.8,
		.name_eng = "VAPI",
		.reg_lang = 1
	};
	
	station_list[4] = (station)
	{
		.code = "ST",
		.location.lat = 21.2049,
		.location.lon = 72.8408,
		.halt.arrival.d = {0},
		.halt.arrival.t = {0,0,0,0},
		.halt.departure.d = {0},
		.halt.departure.t = {0,0,0,0},
		.halt.platform_no = 1,
		.halt.distance_to_next = -2,
		.name_eng = "SURAT",
		.reg_lang = 1
	};
	
	// IMPORTANT - use str copy or memcpy in final version
	
	memcpy(train_status.train_details.number, "19021", 5);
	train_status.train_details.direction = 0;
	memcpy(train_status.train_details.name_eng, "Mumbai Bandra (T.) - Lucknow (Weekly) Express", 45);
	train_status.train_details.stops_count = 5;
	train_status.train_details.type = 0;
	train_status.train_details.src = &station_list[0];
	train_status.train_details.dest = &station_list[4];
	
	train_status.cur_sta = &station_list[0];
	train_status.cur_tracker_state = SOURCE;
	train_status.dist_to_next_sta = station_list[0].halt.distance_to_next;
	train_status.dist_traveled_last = 0;
	//train_status.dist_to_dest_sta = 251.9;
	train_status.next_sta = &station_list[1];
	 
	train_status.prev_tracker_state= ERROR;
	//TODO fix
	train_status.flags |= 8;
	
	while(1)
	{
		tracker_process();
		
		/*
		Vertical_line();
		delay_s(5);		Horizontal_line();		delay_s(5);		Cross_Dot();		delay_s(5);		Show_picture();
		delay_s(5);
		Character_pattern();
		delay_s(5);
		print_str("THIS is tesT message 1");
		delay_s(5);
		if(uart_is_tx_ready(DEBUG_SERIAL))
		{
			uart_write(DEBUG_SERIAL, 's');
		}
		delay_s(1);
		*/
		
		/*
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
