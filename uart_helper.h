#ifndef UART_HELPER_H_
#define UART_HELPER_H_

#include "asf.h"
#include "datatypes.h"
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



//extern const sam_uart_opt_t uart0_settings;

void uart_h_init(void);

#endif /* UART_HELPER_H_ */