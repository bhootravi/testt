#include "gps.h"

volatile uint8_t tx_flag = 0;

volatile int8_t common_buf[GPS_BUF_LEN];
volatile ring_buffer gps_buf_st;
volatile ring_buffer*  gps_buf = &gps_buf_st;

gps_data gps_reading; 

//TODO - check for overflow
void GPS_SERIAL_Handler(void)
{
	//printf("handler called\n\r");
	uint32_t ua_status;
	ua_status = usart_get_status(GPS_SERIAL);
	if(ua_status & US_CSR_RXRDY)
	{	
		ua_status = 0;
		usart_getchar(GPS_SERIAL, &ua_status);
		
		RB_push(gps_buf, ua_status&0xFF);
		if( (ua_status&0xFF) == '\n')
			tx_flag ++;
		//TODO - check for overflow
	}
}

void gps_init()
{
	printf("gps init called\n\r");
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
	printf("gps usart enabled\n\r");
	//usart_disable_interrupt(GPS_SERIAL, ALL_INTERRUPT_MASK);
	usart_enable_interrupt(GPS_SERIAL, US_IER_RXRDY);
	NVIC_EnableIRQ(GPS_SERIAL_IRQn);
	RB_init(gps_buf, common_buf, GPS_BUF_LEN);
	printf("gps init returning\n\r");
}

int gps_process_data(const char* line)
{
	//const char* line = (const char*)&(gps_buf.buf_ptr[gps_buf.eol[ii]]);
	printf("line %s\r\n", line);
	switch (minmea_sentence_id(line, false)) {
		case MINMEA_SENTENCE_RMC: {
			struct minmea_sentence_rmc frame;
			if (minmea_parse_rmc(&frame, line)) {
				
				if (frame.valid)
				{
					gps_reading.invalid_counts = 0;
					bit_clear(gps_reading.flags, GPS_INVALID);
					gps_reading.cur_loc.lat = minmea_tocoord(&frame.latitude);
					gps_reading.cur_loc.lon = minmea_tocoord(&frame.longitude);
					gps_reading.timeDate.d = frame.date;
					gps_reading.timeDate.t = frame.time;
				}
			}
			else {
				printf(INDENT_SPACES "$xxRMC sentence is not parsed\r\n");
				return -1;
			}
		} break;

		case MINMEA_SENTENCE_GGA: {
			struct minmea_sentence_gga frame;
			if (minmea_parse_gga(&frame, line)) {
				//printf(INDENT_SPACES "$xxGGA: fix quality: %d\r\n", frame.fix_quality);
			}
			else {
				printf(INDENT_SPACES "$xxGGA sentence is not parsed\r\n");
				return -1;
			}
		} break;

		case MINMEA_SENTENCE_GST: {
			struct minmea_sentence_gst frame;
			if (minmea_parse_gst(&frame, line)) {
				/*
				printf(INDENT_SPACES "$xxGST: raw latitude,longitude and altitude error deviation: (%d/%d,%d/%d,%d/%d)\r\n",
								frame.latitude_error_deviation.value, frame.latitude_error_deviation.scale,
								frame.longitude_error_deviation.value, frame.longitude_error_deviation.scale,
								frame.altitude_error_deviation.value, frame.altitude_error_deviation.scale);
				printf(INDENT_SPACES "$xxGST floating point degree latitude, longitude and altitude error deviation: (%f,%f,%f)",
								minmea_tofloat(&frame.latitude_error_deviation),
								minmea_tofloat(&frame.longitude_error_deviation),
								minmea_tofloat(&frame.altitude_error_deviation));
				*/
			}
			else {
				printf(INDENT_SPACES "$xxGST sentence is not parsed\r\n");
				return -1;
			}
		} break;

		case MINMEA_SENTENCE_GSV: {
			struct minmea_sentence_gsv frame;
			if (minmea_parse_gsv(&frame, line)) {
				/*
				printf(INDENT_SPACES "$xxGSV: message %d of %d\r\n", frame.msg_nr, frame.total_msgs);
				printf(INDENT_SPACES "$xxGSV: sattelites in view: %d\r\n", frame.total_sats);
				for (int i = 0; i < 4; i++)
					printf(INDENT_SPACES "$xxGSV: sat nr %d, elevation: %d, azimuth: %d, snr: %d dbm\r\n",
										frame.sats[i].nr,
										frame.sats[i].elevation,
										frame.sats[i].azimuth,
										frame.sats[i].snr);
				*/
			}
			else {
				printf(INDENT_SPACES "$xxGSV sentence is not parsed\r\n");
				return -1;
			}
		} break;

		case MINMEA_SENTENCE_VTG: {
			struct minmea_sentence_vtg frame;
			if (minmea_parse_vtg(&frame, line)) {
				gps_reading.speed = minmea_tofloat(&frame.speed_kph);
				/*
				printf(INDENT_SPACES "$xxVTG: true track degrees = %f\r\n",
							minmea_tofloat(&frame.true_track_degrees));
				printf(INDENT_SPACES "        magnetic track degrees = %f\r\n",
							minmea_tofloat(&frame.magnetic_track_degrees));
				printf(INDENT_SPACES "        speed knots = %f\r\n",
							minmea_tofloat(&frame.speed_knots));
				printf(INDENT_SPACES "        speed kph = %f\r\n",
							minmea_tofloat(&frame.speed_kph));
				*/
			}
			else {
				printf(INDENT_SPACES "$xxVTG sentence is not parsed\r\n");
				return -1;
			}
		} break;

		case MINMEA_INVALID: {
			printf(INDENT_SPACES "$xxxxx sentence is not valid\r\n");
			return -1;
		} break;

		default: {
			printf(INDENT_SPACES "$xxxxx sentence is not parsed\r\n");
			return -1;
		} break;
	}
	
	//gps_buf.eol[ii] = -1;
	
	return 0;
}