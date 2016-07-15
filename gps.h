#ifndef GPS_H_
#define GPS_H_

/*
Provides helper functions for tracker to:
	get periodic updates of location, time and speed data
	recover from fix lost or error conditions
	change settings(update intervals, serial config)
*/

#include "asf.h"

#include "minmea.h"
#include "util.h"

#define GPS_BUF_LEN	500

#define INDENT_SPACES "  "

extern volatile uint8_t tx_flag;

//extern volatile int8_t common_buf[GPS_BUF_LEN];
extern volatile ring_buffer* gps_buf;

extern gps_data gps_reading; 

#define ALL_INTERRUPT_MASK  0xffffffff

#define GPS_SERIAL					 USART1
#define GPS_SERIAL_ID				 ID_USART1  //USART0 for sam4l
#define GPS_SERIAL_BAUDRATE			 9600
#define GPS_SERIAL_CHAR_LENGTH		 US_MR_CHRL_8_BIT
#define GPS_SERIAL_PARITY			 US_MR_PAR_NO
#define GPS_SERIAL_STOP_BIT			 US_MR_NBSTOP_1_BIT
#define GPS_SERIAL_IRQn				 USART1_IRQn
#define GPS_SERIAL_Handler			 USART1_Handler

void gps_init(void);

void gps_get_data(void);
int gps_process_data(const char* line);
int gps_load_settings(void);

void gps_cold_start(void);
void gps_warm_start(void);
void gps_hot_start(void);


#endif /* GPS_H_ */