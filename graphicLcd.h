#ifndef GRAPHIC_LCD_H
#define GRAPHIC_LCD_H

#include "uart_helper.h"

#define   MaxColumn    240              //  128 * 64 ~ 240 *128
#define   MaxRow       128              //

#define	  graphic_home_address  ((MaxColumn /8) *(MaxRow/8))
#define   LOW_BYTE 	(graphic_home_address % 0x0100)
#define   HI_BYTE  	(graphic_home_address / 0x0100)
//=================================
//  I/O define
//=================================
#define LCD_DATA_PORT			IOPORT_PIOA		 //  DB0~DB7,DATA BUS#define	LCD_DATA_IN				REG_PIOA_PDSR#define	LCD_IO_PORT_CLEAR		REG_PIOA_CODR#define	LCD_IO_PORT_SET			REG_PIOA_SODR			#define LCD_DATA_BUS_D0			PIO_PER_P24
#define LCD_DATA_BUS_D1			PIO_PER_P25
#define LCD_DATA_BUS_D2			PIO_PER_P26
#define LCD_DATA_BUS_D3			PIO_PER_P27
#define LCD_DATA_BUS_D4			PIO_PER_P28
#define LCD_DATA_BUS_D5			PIO_PER_P29
#define LCD_DATA_BUS_D6			PIO_PER_P30
#define LCD_DATA_BUS_D7			PIO_PER_P31

#define LCD_DATA_MASK_LN		((LCD_DATA_BUS_D0)|(LCD_DATA_BUS_D1)|(LCD_DATA_BUS_D2)|(LCD_DATA_BUS_D3))
#define LCD_DATA_MASK_UN		((LCD_DATA_BUS_D4)|(LCD_DATA_BUS_D5)|(LCD_DATA_BUS_D6)|(LCD_DATA_BUS_D7))
#define LCD_DATA_MASK			(LCD_DATA_MASK_LN|LCD_DATA_MASK_UN)
#define LCD_SHIFT				24
			
#define LCD_CD					PIO_PER_P16
#define LCD_RD					PIO_PER_P17
#define LCD_WR					PIO_PER_P18
#define LCD_CE					PIO_PER_P19
#define LCD_RESET				PIO_PER_P20
#define LCD_FS					PIO_PER_P22
#define LCD_DIR					PIO_PER_P23

#define LCD_CD_PIN					IOPORT_CREATE_PIN(PIOA, 16)
#define LCD_RD_PIN					IOPORT_CREATE_PIN(PIOA, 17)
#define LCD_WR_PIN					IOPORT_CREATE_PIN(PIOA, 18)
#define LCD_CE_PIN					IOPORT_CREATE_PIN(PIOA, 19)
#define LCD_RESET_PIN				IOPORT_CREATE_PIN(PIOA, 20)
#define LCD_FS_PIN					IOPORT_CREATE_PIN(PIOA, 22)
#define LCD_DIR_PIN					IOPORT_CREATE_PIN(PIOA, 23)

void graphicLcd_init(void);

void Write_command(uint8_t );
void Write_data(uint8_t);
void Show_picture(void);
void Vertical_line(void);
void Cross_Dot(void);
void home_address(void);
void Horizontal_line(void);
void full_on(void);
void Initial_T6963C(void);
void Character_pattern(void);
uint8_t Read_status(void);
void Status_check(uint8_t);
void SYSCLK_Init (void);
uint8_t Read_data(void);


#endif