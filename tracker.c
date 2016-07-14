#include "tracker.h"

#define KM_5_TOGO_F 0
#define KM_1_TOGO_F 1

train_struct* cur_train;
train_status_struct train_status;
//station* prev_sta, cur_sta, next_sta;

//tracker_states cur_state = ERROR, prev_state = ERROR;
uint8_t flags_1 = 0;

float temp_dist = 0;

//updates the tracker status using the new gps data
//assigns the relevant messages for audio and display
//requires 
//new gps data to to be updated in the train status struct

int tracker_process()
{
	//gps_get_data();
	//int x = gps_process_data();
	int16_t temp = 0;
	int8_t line[80];
	//printf("tracker tx flag %d\n\r", tx_flag);
	while(tx_flag)
	{
		do
		{
			*(line+temp) = RB_pop(gps_buf);
			temp++;
		} while (*(line+temp-1) != '\n');
		tx_flag--;
		if(*(line+temp-1) == '\n' && *(line+temp-2) == '\r')
		{
			printf("got a valid string\n\r");
			
			temp -= 2;
			*(line+temp) = *(line+temp + 1) = 0;
		}
		else
		{
			printf("error condition in gps receive\n\r");
		}
		printf("len = %d\n\r", temp);
		temp = gps_process_data((const char*)line);
		
		temp = 0;
	}
	
	/*
	//if data received is valid
	if(x == 0)
	{
		//compute the distance to dest station
		//WON'T WORK
		train_status.dist_to_dest_sta = distance_between
							(train_status.cur_gps_data.cur_loc, 
								train_status.train_data->dest->location);
		//compute the distance to next station
		train_status.dist_to_next_sta = distance_between
							(train_status.cur_gps_data.cur_loc,
								train_status.cur_sta->location);
		//update the tracker state according to the need
		switch(train_status.cur_tracker_state)
		{
			case SOURCE:
				//opposite bearings
				if((relative_difference(train_status.dist_to_next_sta, 1000.0) < TOLERANCE_DIF))
				{
					train_status.cur_tracker_state = CROSSED_1KM;
				
					temp_dist = train_status.dist_to_dest_sta;
						
					//update message
					//repeat times 3 audio, display loop
				}
			case ENROUTE:
				//if course over ground == bearing of the station
				if((relative_difference(train_status.dist_to_next_sta, 5000.0) < TOLERANCE_DIF))
				{
					train_status.cur_tracker_state = TOGO_5KM;
					//update message
					//repeat times 3 audio and display
				}
			case TOGO_5KM:
				//if course over ground == bearing of the station
				if((relative_difference(train_status.dist_to_next_sta, 1000.0) < TOLERANCE_DIF))
				{
					train_status.cur_tracker_state = TOGO_1KM;
					//update message
					//repeat times 3 audio and display
				}
			case TOGO_1KM:
				if((relative_difference(train_status.dist_to_next_sta, 50.0) < TOLERANCE_DIF))
				{
					train_status.cur_tracker_state = STATION_REACHED;
					if (train_status.next_sta == NULL)
					{
						train_status.cur_tracker_state = DESTINATION;
					}
					//update message
					//repeat times 3 audio, display loop
					
				}
			case STATION_REACHED:				
				//opposite bearings										
				if((relative_difference(train_status.dist_to_next_sta, 1000.0) < TOLERANCE_DIF))
				{
					train_status.cur_tracker_state = CROSSED_1KM;
					
					temp_dist = train_status.dist_to_dest_sta;
					
					//free train_status.cur_sta
					train_status.cur_sta = train_status.next_sta;
					//raise flag to update next station info
					
					//update message
					//repeat times 3 audio, display loop		
				}
			case CROSSED_1KM:
				//check next sta data updated flag
				
				//adjustment to avoid storage of prev station
				if((relative_difference(train_status.dist_to_dest_sta, temp_dist - 2.0) < TOLERANCE_DIF))
				{
					train_status.cur_tracker_state = CROSSED_3KM;
					
					//update message
					//repeat times 3 audio, display loop
				}
			case CROSSED_3KM:
				if((relative_difference(train_status.dist_to_dest_sta, temp_dist - 4.0) < TOLERANCE_DIF))
				{
					temp_dist = -500000.0;
					train_status.cur_tracker_state = ENROUTE;
				
					//update message
					//repeat times 3 audio, display loop
				}
			case DESTINATION:
				//update message
				break;
			case ERROR:
			case WRONG_DIRECTION:
				//decide the position and status
				//break;
			case GPS_WEAK:
				//put slogan and error message
				break;	
		}	
	}
	else
	{
		
		//put default error message
	}
	*/
	return 0;
}