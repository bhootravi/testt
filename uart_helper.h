#ifndef UART_HELPER_H_
#define UART_HELPER_H_

#include "asf.h"
#include <inttypes.h>

#define ALL_INTERRUPT_MASK  0xffffffff

#define DEBUG_SERIAL                 UART0
#define DEBUG_SERIAL_ID              ID_UART0  //USART0 for sam4l
#define DEBUG_SERIAL_BAUDRATE        9600
#define DEBUG_SERIAL_CHAR_LENGTH     US_MR_CHRL_8_BIT
#define DEBUG_SERIAL_PARITY          US_MR_PAR_NO
#define DEBUG_SERIAL_STOP_BIT        US_MR_NBSTOP_1_BIT
#define DEBUG_SERIAL_IRQn			 UART0_IRQn
#define DEBUG_SERIAL_Handler		 UART0_Handler

#define GPS_SERIAL					 USART1
#define GPS_SERIAL_ID				 ID_USART1  //USART0 for sam4l
#define GPS_SERIAL_BAUDRATE			 9600
#define GPS_SERIAL_CHAR_LENGTH		 US_MR_CHRL_8_BIT
#define GPS_SERIAL_PARITY			 US_MR_PAR_NO
#define GPS_SERIAL_STOP_BIT			 US_MR_NBSTOP_1_BIT
#define GPS_SERIAL_IRQn				 USART1_IRQn
#define GPS_SERIAL_Handler			 USART1_Handler

//extern const sam_uart_opt_t uart0_settings;

void uart_h_init(void);

#endif /* UART_HELPER_H_ */