#include <asf.h>
#include "graphicLcd.h"

uint8_t  picture[128][30];

void graphicLcd_init(void) {

	uint32_t lcd_all_mask = (LCD_CD|LCD_FS|LCD_RESET|LCD_CE|LCD_WR|LCD_RD|LCD_DIR|
					LCD_DATA_BUS_D0|LCD_DATA_BUS_D1|LCD_DATA_BUS_D2|LCD_DATA_BUS_D3|LCD_DATA_BUS_D4|LCD_DATA_BUS_D5|
					LCD_DATA_BUS_D6|LCD_DATA_BUS_D7);
	//set pins controllable by PIO controller
	
	ioport_enable_port(LCD_DATA_PORT, lcd_all_mask);
	ioport_set_port_dir(LCD_DATA_PORT, lcd_all_mask, IOPORT_DIR_OUTPUT);

	//set RD,WR,CE,RESET pins high since they are active low
	ioport_set_port_mode(LCD_DATA_PORT, (LCD_WR|LCD_RD|LCD_CE|LCD_RESET), IOPORT_MODE_PULLUP);
	uart_write(DEBUG_SERIAL, 'a');
	Initial_T6963C();
	while(!uart_is_tx_ready(DEBUG_SERIAL));
	uart_write(DEBUG_SERIAL, 'b');
}


//==================================

//      initial T6963C

//==================================

void Initial_T6963C()
{
	uint8_t    LOW_BYTE1;

	ioport_set_pin_level(LCD_RESET_PIN, 0);
	delay_ms(1);
	delay_us(100);
	delay_us(100);
	ioport_set_pin_level(LCD_RESET_PIN, 1);
	delay_ms(1);
	delay_us(100);
	delay_us(100);	
	ioport_set_pin_level(LCD_FS_PIN, 0);
	ioport_set_port_dir(LCD_DATA_PORT,LCD_DATA_MASK,IOPORT_DIR_OUTPUT); 
	uart_write(DEBUG_SERIAL,'c');
	//write text home address=0000h
	Status_check(0x03);
	Write_data(0x00);
	Status_check(0x03);
	Write_data(0x00);
	Status_check(0x03);
	Write_command(0x40);
	Status_check(0x03);
	Write_data(LOW_BYTE);
	Status_check(0x03);
	Write_data(HI_BYTE);
	Status_check(0x03);
	Write_command(0x42);
		uart_write(DEBUG_SERIAL,'d');
	//write text area address
	LOW_BYTE1 = MaxColumn /8;
	Status_check(0x03);
	Write_data(LOW_BYTE1);
	Status_check(0x03);
	Write_data(0x00);
	Status_check(0x03);
	Write_command(0x41);
	
	while(!uart_is_tx_ready(DEBUG_SERIAL));
	uart_write(DEBUG_SERIAL, 'e');
	
	//write graphic area address
	LOW_BYTE1 = MaxColumn /8;
	Status_check(0x03);
	Write_data(LOW_BYTE1);
	Status_check(0x03);
	Write_data(0x00);
	Status_check(0x03);
	Write_command(0x43);

	while(!uart_is_tx_ready(DEBUG_SERIAL));
	uart_write(DEBUG_SERIAL, 'f');
	
	//set display mode Display mode set (Graphic only enable)
	Status_check(0x03);
	Write_command(0x80); //internal CG ROM mode
}

//-------------------------------------------------
//          Write Data Function
//-------------------------------------------------
void Write_data(uint8_t data){
		ioport_set_port_dir(LCD_DATA_PORT,LCD_DATA_MASK,IOPORT_DIR_OUTPUT); 	//LCD_DATA_PORT = datap << LCD_DATA_MASK;	ioport_set_port_level(LCD_DATA_PORT,LCD_DATA_MASK, (data << LCD_SHIFT));	ioport_set_pin_level(LCD_DIR_PIN, 1); 	ioport_set_pin_level(LCD_CD_PIN, 0); 
	delay_us(100);	ioport_set_pin_level(LCD_CE_PIN, 0); 	delay_us(100);	ioport_set_pin_level(LCD_WR_PIN, 0); 
	delay_us(200);	ioport_set_pin_level(LCD_CE_PIN, 1); 
	delay_us(100);
	ioport_set_pin_level(LCD_WR_PIN, 1); }
//-------------------------------------------------
//     WRITE_COMMAND
//-------------------------------------------------

void Write_command(uint8_t command){
	ioport_set_port_dir(LCD_DATA_PORT,LCD_DATA_MASK,IOPORT_DIR_OUTPUT); 
	//LCD_DATA_PORT = command << LCD_DATA_MASK;
	ioport_set_port_level(LCD_DATA_PORT,LCD_DATA_MASK,(command << LCD_SHIFT));
	ioport_set_pin_level(LCD_DIR_PIN, 1);	ioport_set_pin_level(LCD_CD_PIN, 1);
	delay_us(100);	ioport_set_pin_level(LCD_CE_PIN, 0);	delay_us(100);	ioport_set_pin_level(LCD_WR_PIN, 0);
	delay_us(200);	ioport_set_pin_level(LCD_CE_PIN, 1);
	delay_us(100);
	ioport_set_pin_level(LCD_WR_PIN, 1);
}
//-------------------------------------------------
//     Read_data
//-------------------------------------------------
uint8_t Read_data(void){
	
	uint32_t read_data = 0;
	uint8_t data = 0;
	
	ioport_set_port_dir(LCD_DATA_PORT,LCD_DATA_MASK, IOPORT_DIR_OUTPUT);	ioport_set_port_level(LCD_DATA_PORT,LCD_DATA_MASK, (0xFF << LCD_SHIFT));		ioport_set_pin_level(LCD_DIR_PIN, 0);	ioport_set_pin_level(LCD_CD_PIN, 0);	ioport_set_pin_level(LCD_CE_PIN, 0);	ioport_set_pin_level(LCD_RD_PIN, 0);
	
	ioport_set_port_dir(LCD_DATA_PORT,LCD_DATA_MASK, IOPORT_DIR_INPUT);
	read_data = ioport_get_port_level(LCD_DATA_PORT,LCD_DATA_MASK);		data = read_data >> LCD_SHIFT;		ioport_set_pin_level(LCD_RD_PIN, 1);	ioport_set_pin_level(LCD_CE_PIN, 1);
	
	return data;
}

//-------------------------------------------------
//     Read_status
//-------------------------------------------------
uint8_t Read_status()
{
	ioport_set_port_dir(LCD_DATA_PORT,LCD_DATA_MASK,IOPORT_DIR_INPUT); 
	uint32_t read_data;
	uint8_t status = 0;
	//LCD_DATA_PORT = 0xff << LCD_DATA_MASK;	ioport_set_port_level(LCD_DATA_PORT,LCD_DATA_MASK,(0xff << LCD_SHIFT));	ioport_set_pin_level(LCD_DIR_PIN, 0);	ioport_set_pin_level(LCD_CD_PIN, 1);	ioport_set_pin_level(LCD_CE_PIN, 0);	ioport_set_pin_level(LCD_RD_PIN, 0);
	ioport_set_port_dir(LCD_DATA_PORT,LCD_DATA_MASK,IOPORT_DIR_INPUT);
	read_data = ioport_get_port_level(LCD_DATA_PORT,LCD_DATA_MASK);
	ioport_set_port_dir(LCD_DATA_PORT,LCD_DATA_MASK,IOPORT_DIR_OUTPUT); 
	status = (read_data >> LCD_SHIFT) & 0xff;	ioport_set_pin_level(LCD_RD_PIN, 1);	ioport_set_pin_level(LCD_CE_PIN, 1);
	return status;
}
void Status_check(uint8_t sta)
{
	uint8_t tmp=0,satus=0,counter=0;
	do {
		tmp = Read_status();
		satus = tmp&sta; 
		counter++;
	}
	while(satus != sta);
	//return satus;
}


//==================================

// show picture

//==================================

void Show_picture()
{
	uint8_t x,y;
	Status_check(0x03);
	Write_command(0x98);
	home_address();
	Status_check(0x03);
	Write_command(0xB0);
	for (y=0; y<MaxRow;y++)
	{
		for (x=0; x<MaxColumn/8;x++)
		{
			Status_check(0x08);
			Write_data(picture[y][x]);
		}
	}
	Status_check(0x08);
	Write_command(0xB2); //set auto write reset
}


void home_address()
{
	Status_check(0x03);
	Write_data(LOW_BYTE);
	Status_check(0x03);
	Write_data(HI_BYTE);
	Status_check(0x03);
	Write_command(0x24);
}


//==================================

// 8*8 Character pattern test (Text)

//==================================

void Character_pattern()
{
	uint8_t x,y,k=0;
	Status_check(0x03);
	Write_command(0x94);
	Status_check(0x03);
	Write_data(0x00);
	Status_check(0x03);
	Write_data(0x00);
	Status_check(0x03);
	Write_command(0x24);
	Status_check(0x03);
	Write_command(0xB0);
	for (y = 0; y<MaxRow/8;y++)
	{
		for (x = 0; x < MaxColumn/8;x++)
		{
			if(k == 0x80)
				k = 0x00;
			Status_check(0x08);
			Write_data(k++);
		}
	}
	Status_check(0x08);
	Write_command(0xB2); //set auto write reset
}


//==================================

//        VERTICAL LINE

//==================================

void Vertical_line()
{
	uint8_t x,y;
	Status_check(0x03);
	Write_command(0x98);
	home_address();
	Status_check(0x03);
	Write_command(0xB0);
	for (y = 0; y<MaxRow;y++)
	{
		for (x = 0; x<MaxColumn/8;x++)
		{
			Status_check(0x08);
			Write_data(0xaa);
		}
	}
	Status_check(0x08);
	Write_command(0xB2); //set auto write reset
}

//==================================

//      CROSS DOT

//==================================

void Cross_Dot()
{
	uint8_t x,y;
	Status_check(0x03);
	Write_command(0x98);
	home_address();
	Status_check(0x03);
	Write_command(0xB0);
	for (y = 0; y<MaxRow/2;y++)
	{
		for (x = 0; x<MaxColumn/8;x++)
		{
			Status_check(0x08);
			Write_data(0xaa);
		}
		for (x = 0; x<MaxColumn/8;x++)
		{
			Status_check(0x08);
			Write_data(0x55);
		}
	}
	Status_check(0x08);
	Write_command(0xB2); //set auto write reset
}

//==================================

//   HORIZONTAL LINE

//==================================

void Horizontal_line()
{
	uint8_t x,y;
	Status_check(0x03);
	Write_command(0x98);
	home_address();
	Status_check(0x03);
	Write_command(0xB0);
	for (y=0; y<MaxRow/2;y++)
	{
		for (x = 0; x<MaxColumn/8;x++)
		{
			Status_check(0x08);
			Write_data(0xff);
		}

		for (x = 0; x<MaxColumn/8;x++)
		{
			Status_check(0x08);
			Write_data(0x00);
		}
	}
	Status_check(0x08);
	Write_command(0xB2); //set auto write reset
}


//==================================

// full on

//==================================

void full_on()
{
	uint8_t x,y,status=0;
	Status_check(0x03);
	Write_command(0x98);
	home_address();
	Status_check(0x03);
	Write_command(0xB0);
	for (y = 0; y<MaxRow;y++)
	{
		for (x = 0; x<MaxColumn/8;x++)
		{
			Status_check(0x08);
			Write_data(0xff);
		}
	}
	Status_check(0x08);
	Write_command(0xB2); //set auto write reset
}

//==================================

uint8_t  picture[128][30]=
{	
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x77,0xBD,

	0xEF,0x7B,0xDE,0xF7,0xBD,0xEF,0x7B,0xDE,

	0xF7,0xBD,0xEF,0x7B,0xDE,0xF7,0xBD,0xEF,

	0x7B,0xDE,0xF7,0xBD,0xEF,0x7B,0xDE,0xF7,

	0xBD,0xEF,0x7B,0xDE,0x7F,0xBF,0xEF,0xFB,

	0xFE,0xFF,0xBF,0xEF,0xFB,0xFE,0xFF,0xBF,

	0xEF,0xFB,0xFE,0xFF,0xBF,0xEF,0xFB,0xFE,

	0xFF,0xBF,0xEF,0x7B,0xFE,0xFF,0xBF,0xEF,

	0xFB,0xFE,0x7F,0xBF,0xEF,0xFB,0xFE,0xFF,

	0xBF,0xEF,0xFB,0xFE,0xFF,0xBF,0xEF,0xFB,

	0xFE,0xFF,0xBF,0xEF,0xFB,0xFE,0xFF,0xBF,

	0xEF,0xFB,0xFE,0xFF,0xBF,0xEF,0xFB,0xFE,

	0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,

	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,

	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,

	0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x7F,0xFF,

	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,

	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,

	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,

	0xFF,0xFF,0xFF,0xFE,0x7D,0x55,0x55,0x55,

	0x55,0x5F,0xFF,0x55,0x55,0x55,0x55,0x57,

	0xFF,0xF5,0x55,0x55,0x55,0x55,0xFF,0xFD,

	0x55,0x55,0x55,0x55,0xFF,0xFF,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xBC,

	0x47,0xAA,0xAA,0xAA,0xAA,0xAF,0x44,0x7A,

	0xAA,0xAA,0xAA,0xAB,0xD1,0x1E,0xAA,0xAA,

	0xAA,0xAB,0xC4,0x47,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x5D,0xD7,0x55,

	0x55,0x55,0x55,0x57,0x55,0x75,0x55,0x55,

	0x55,0x55,0xD7,0x5D,0x55,0x55,0x55,0x55,

	0xF5,0x57,0x55,0x55,0x55,0x54,0x0E,0xAA,

	0xAA,0xAA,0xAA,0xBC,0x57,0xAA,0xAA,0xAA,

	0xAA,0xAF,0x55,0x7A,0xAA,0xAA,0xAA,0xAB,

	0xD1,0x5E,0xAA,0xAA,0xAA,0xAB,0xC5,0x57,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x5F,0x57,0x55,0x55,0x55,0x55,0x57,

	0x55,0x75,0x55,0x55,0x55,0x55,0xDD,0x5D,

	0x55,0x55,0x55,0x55,0xDD,0x57,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xBC,

	0x47,0xAA,0xAA,0xAA,0xAA,0xAF,0x44,0x7A,

	0xAA,0xAA,0xAA,0xAB,0xD1,0x1E,0xAA,0xAA,

	0xAA,0xAB,0xC4,0x47,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x5F,0xFF,0x55,

	0x55,0x55,0x55,0x57,0xFF,0xF5,0x55,0x55,

	0x55,0x55,0xFF,0xFD,0x55,0x55,0x55,0x55,

	0xFF,0xFF,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xBF,0xFF,0xAA,0xAA,0xAA,

	0xAA,0xAF,0xFF,0xFA,0xAA,0xAA,0xAA,0xAB,

	0xFF,0xFE,0xAA,0xAA,0xAA,0xAB,0xFF,0xFF,

	0xAA,0xAA,0xAA,0xAA,0x3D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x0E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x3D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x0E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x3D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x0E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x3D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7F,0xFF,

	0xFA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7F,0xFF,0xF5,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7F,0xE2,0x3A,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7F,0xEE,0xB5,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x0F,0xE2,

	0xBA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7F,0xFA,0xB5,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7F,0xE2,0x3A,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7F,0xFF,0xF5,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7F,0xFF,

	0xFA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x3D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x0E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x3D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x0E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x3D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x0E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x3D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x0E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x3D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7F,0xFF,0xFA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7F,0xFF,0xF5,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7F,0xA2,

	0x3A,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7F,0xAA,0xB5,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x0F,0xAA,0xBA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7F,0xAA,0xB5,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7F,0xA2,

	0x3A,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7F,0xFF,0xF5,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7F,0xFF,0xFA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x3D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x0E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x3D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x0E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x7D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0x3D,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x54,0x7E,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,

	0xAA,0xAA,0xAA,0xAA,0x7D,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

	0x55,0x54,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};
