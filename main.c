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

#define INDENT_SPACES "  "

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
int parse_sent(char* line);

int parse_sent(char* line)
{
	printf("line %s\r\n", line);
	switch (minmea_sentence_id(line, false)) {
		case MINMEA_SENTENCE_RMC: {
			struct minmea_sentence_rmc frame;
			if (minmea_parse_rmc(&frame, line)) {
				printf(INDENT_SPACES "$xxRMC: raw coordinates and speed: (%d/%d,%d/%d) %d/%d\r\n",
						frame.latitude.value, frame.latitude.scale,
						frame.longitude.value, frame.longitude.scale,
						frame.speed.value, frame.speed.scale);
				printf(INDENT_SPACES "$xxRMC fixed-point coordinates and speed scaled to three decimal places: (%d,%d) %d\r\n",
						minmea_rescale(&frame.latitude, 1000),
						minmea_rescale(&frame.longitude, 1000),
						minmea_rescale(&frame.speed, 1000));
				printf(INDENT_SPACES "$xxRMC floating point degree coordinates and speed: (%f,%f) %f\r\n",
						minmea_tocoord(&frame.latitude),
						minmea_tocoord(&frame.longitude),
						minmea_tofloat(&frame.speed));
			}
			else {
				printf(INDENT_SPACES "$xxRMC sentence is not parsed\r\n");
			}
		} break;

		case MINMEA_SENTENCE_GGA: {
			struct minmea_sentence_gga frame;
			if (minmea_parse_gga(&frame, line)) {
				printf(INDENT_SPACES "$xxGGA: fix quality: %d\r\n", frame.fix_quality);
			}
			else {
				printf(INDENT_SPACES "$xxGGA sentence is not parsed\r\n");
			}
		} break;

		case MINMEA_SENTENCE_GST: {
			struct minmea_sentence_gst frame;
			if (minmea_parse_gst(&frame, line)) {
				printf(INDENT_SPACES "$xxGST: raw latitude,longitude and altitude error deviation: (%d/%d,%d/%d,%d/%d)\r\n",
				frame.latitude_error_deviation.value, frame.latitude_error_deviation.scale,
				frame.longitude_error_deviation.value, frame.longitude_error_deviation.scale,
				frame.altitude_error_deviation.value, frame.altitude_error_deviation.scale);
				printf(INDENT_SPACES "$xxGST fixed point latitude,longitude and altitude error deviation"
				" scaled to one decimal place: (%d,%d,%d)\r\n",
				minmea_rescale(&frame.latitude_error_deviation, 10),
				minmea_rescale(&frame.longitude_error_deviation, 10),
				minmea_rescale(&frame.altitude_error_deviation, 10));
				printf(INDENT_SPACES "$xxGST floating point degree latitude, longitude and altitude error deviation: (%f,%f,%f)",
				minmea_tofloat(&frame.latitude_error_deviation),
				minmea_tofloat(&frame.longitude_error_deviation),
				minmea_tofloat(&frame.altitude_error_deviation));
			}
			else {
				printf(INDENT_SPACES "$xxGST sentence is not parsed\r\n");
			}
		} break;

		case MINMEA_SENTENCE_GSV: {
			struct minmea_sentence_gsv frame;
			if (minmea_parse_gsv(&frame, line)) {
				printf(INDENT_SPACES "$xxGSV: message %d of %d\r\n", frame.msg_nr, frame.total_msgs);
				printf(INDENT_SPACES "$xxGSV: sattelites in view: %d\r\n", frame.total_sats);
				for (int i = 0; i < 4; i++)
				printf(INDENT_SPACES "$xxGSV: sat nr %d, elevation: %d, azimuth: %d, snr: %d dbm\r\n",
				frame.sats[i].nr,
				frame.sats[i].elevation,
				frame.sats[i].azimuth,
				frame.sats[i].snr);
			}
			else {
				printf(INDENT_SPACES "$xxGSV sentence is not parsed\r\n");
			}
		} break;

		case MINMEA_SENTENCE_VTG: {
			struct minmea_sentence_vtg frame;
			if (minmea_parse_vtg(&frame, line)) {
				printf(INDENT_SPACES "$xxVTG: true track degrees = %f\r\n",
				minmea_tofloat(&frame.true_track_degrees));
				printf(INDENT_SPACES "        magnetic track degrees = %f\r\n",
				minmea_tofloat(&frame.magnetic_track_degrees));
				printf(INDENT_SPACES "        speed knots = %f\r\n",
				minmea_tofloat(&frame.speed_knots));
				printf(INDENT_SPACES "        speed kph = %f\r\n",
				minmea_tofloat(&frame.speed_kph));
			}
			else {
				printf(INDENT_SPACES "$xxVTG sentence is not parsed\r\n");
			}
		} break;

		case MINMEA_INVALID: {
			printf(INDENT_SPACES "$xxxxx sentence is not valid\r\n");
		} break;

		default: {
			printf(INDENT_SPACES "$xxxxx sentence is not parsed\r\n");
		} break;
	}
	return 0;
}

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
	
	uart_h_init();
	
	ioport_init();
	
	sd_mmc_init();
	
	audio_init();
	
	if(audio_open_file(test_file_name))
	{
		printf("open error\n\r");
	}
	printf("file open successful\n\r");
	
	play();
	//graphicLcd_init();
	//sam_uart_opt uart0_settings;
	/* Insert application code here, after the board has been initialized. */

	printf("test\n");
	
	int t = 0;
	
	while(1)
	{
		char *line;
		if(tx_flag)
		{
			printf("%s\r\n", gps_buf.buf_ptr);
			
			t++;
			line = strtok((char*)gps_buf.buf_ptr,"\r\n");
			 while (line != NULL)
			 {
				 parse_sent(line);
				 line = strtok(NULL,"\r\n");
			 }
			 
			clear_buf(&gps_buf);
			tx_flag = 0;
		}
		
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
