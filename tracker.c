#include "tracker.h"

#define KM_5_TOGO_F 0
#define KM_1_TOGO_F 1


train_status_struct train_status;
//station* prev_sta, cur_sta, next_sta;

//tracker_states cur_state = ERROR, prev_state = ERROR;
uint8_t flags_1 = 0;

//updates the tracker status using the new gps data
//assigns the relevant messages for audio and display
//requires 
//new gps data to to be updated in the train status struct

int tracker_process()
{
	//gps_get_data();
	//int x = gps_process_data();
	int16_t temp = 0;
	int8_t line[80], x = 0, xx = 0;
	float distance_covered = 0;
	//printf("tracker tx flag %d\n\r", tx_flag);
	
	while(tx_flag)
	{
		printf("\t processing\n\r");
		do
		{
			*(line+temp) = RB_pop(gps_buf);
			temp++;
		} while (*(line+temp-1) != '\n');
		tx_flag--;
		if(*(line+temp-1) == '\n' && *(line+temp-2) == '\r')
		{
			xx++;
			//printf("got a valid string\n\r");
			temp -= 2;
			*(line+temp) = *(line+temp + 1) = 0;
		}
		else
		{
			printf("error condition in gps receive\n\r");
		}
		printf("\t len = %d\n\r", temp);
		x += gps_process_data((const char*)line);
		
		temp = 0;
	}
	
	char te[10];
	
	if(xx > 0)
	{
		
		printf("\t invalid count %d\n\r", gps_reading.invalid_counts);
		sprintf(te, "%f\n\r", gps_reading.cur_loc.lat);
		printf("\t lat %s", te);
		sprintf(te, "%f\n\r", gps_reading.cur_loc.lon);
		printf("\t lon %s", te);
		
		if(train_status.flags&8)
		{
			train_status.cur_gps_data = gps_reading;
			train_status.flags &= ~8;
		}
		
		distance_covered = distance_between(train_status.cur_gps_data.cur_loc,
							gps_reading.cur_loc);
		
		//gps_reading from gps.h
		train_status.cur_gps_data = gps_reading;
		
	}
	
	//if data received is valid
	
	if(xx > 0)
	{
		//computing the distance to dest station
		//WON'T WORK because the method calculates the shortest path
		/*
		train_status.dist_to_dest_sta = distance_between
							(train_status.cur_gps_data.cur_loc, 
								train_status.train_details.dest->location);
		*/
		
		//train_status.dist_to_dest_sta -= distance_covered;
		train_status.dist_traveled_last += distance_covered;
		
		train_status.dist_to_next_sta = distance_between(train_status.cur_gps_data.cur_loc, 
															train_status.next_sta->location);
		//compute the distance to next station
		/*
		train_status.dist_to_next_sta = distance_between
							(train_status.cur_gps_data.cur_loc,
								train_status.cur_sta->location);
		*/
		//update the tracker state according to the need
		printf("tracker status %d\n\r", train_status.cur_tracker_state);
		printf("\t station %s\n\r", train_status.cur_sta->name_eng);
		sprintf(te,"%f\n\r", distance_covered);
		printf("\t dist covered %s", te);
		sprintf(te,"%f\n\r", train_status.dist_to_next_sta);
		printf("\t dist to next %s", te);
		sprintf(te,"%f\n\r", train_status.dist_traveled_last);
		printf("\t dist traveled %s\n\r", te);
		printf("--------------------------------------\n");
		float rad;
		switch(train_status.cur_tracker_state)
		{
			case SOURCE:
				rad = 1.0;
				//opposite bearings
				if((relative_difference(train_status.dist_traveled_last, 1.0) < TOLERANCE_DIF))
				{
					train_status.cur_tracker_state = CROSSED_1KM;
						
					//update message
					//repeat times 3 audio, display loop
				}
				break;
			case ENROUTE:
				rad = 5.0;
				//if course over ground == bearing of the station
				if((relative_difference(train_status.dist_to_next_sta, 5.0) < TOLERANCE_DIF/5))
				{
					train_status.cur_tracker_state = TOGO_5KM;
					//update message
					//repeat times 3 audio and display
				}
				break;
			case TOGO_5KM:
				rad = 1.0;
				//if course over ground == bearing of the station
				if((relative_difference(train_status.dist_to_next_sta, 1.0) < TOLERANCE_DIF)
					|| train_status.dist_to_next_sta < 0.9)
				{
					train_status.cur_tracker_state = TOGO_1KM;
					//update message
					//repeat times 3 audio and display
				}
				break;
			case TOGO_1KM:
				rad = 0.050;
				if((relative_difference(train_status.dist_to_next_sta, 0.050) < TOLERANCE_DIF)
					|| train_status.dist_to_next_sta < 0.05)
				{
					train_status.cur_tracker_state = STATION_REACHED;
					train_status.dist_traveled_last = 0;
					
					if (train_status.next_sta == NULL)
					{
						train_status.cur_tracker_state = DESTINATION;
					}
					//update message
					//repeat times 3 audio, display loop
					
				}
				break;
			case STATION_REACHED:				
				rad = 1.0;
				//opposite bearings										
				if((relative_difference(train_status.dist_traveled_last, 1.0) < TOLERANCE_DIF)
					|| train_status.dist_traveled_last > 1.1)
				{
					train_status.cur_tracker_state = CROSSED_1KM;
					//free train_status.cur_sta
					train_status.cur_sta = train_status.next_sta;
					train_status.next_sta = NULL;
					//update next station info
					
					//update message
					//repeat times 3 audio, display loop		
				}
				break;
			case CROSSED_1KM:
				//check next sta data updated flag
				
				//adjustment to avoid storage of prev station
				if((relative_difference(train_status.dist_traveled_last, 3.0) < TOLERANCE_DIF)
					|| train_status.dist_traveled_last > 3.1)
				{
					train_status.cur_tracker_state = CROSSED_3KM;
					
					//update message
					//repeat times 3 audio, display loop
				}
				break;
			case CROSSED_3KM:
				if((relative_difference(train_status.dist_traveled_last, 5.0) < TOLERANCE_DIF)
					|| train_status.dist_traveled_last > 5.1)
				{
					train_status.cur_tracker_state = ENROUTE;
				
					//update message
					//repeat times 3 audio, display loop
				}
				break;
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
	
	return 0;
}