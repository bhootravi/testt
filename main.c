/**
 * \file
 *
 * \brief Empty user application template
 *
 */

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

int gps_data_rx = 0;

int self_test(void)
{
	//component_check(gps)
	//component_check(file sys)
	//component_check(usb)
	//sufficient mem available
	return 0;
}

int key_pressed()
{
	return 0;
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	
	board_init();
	
	/* Insert application code here, after the board has been initialized. */
	
	int x = 0;
	
	if(self_test())
	{
		//self test failed,
		//log
	}
	
	//loop start
	if((x = key_pressed()))
	{
		/*
		switch(x)
		{
			case ....
		}
		*/
	}
	gps_data_rx = 1;
	if(gps_data_rx)
	{
		tracker_process();
	}
	/*
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
