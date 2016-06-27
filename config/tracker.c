#include "tracker.h"

#define KM_5_TOGO_F 0
#define KM_1_TOGO_F 1

train_struct* cur_train;
train_status_struct train_status;
//station* prev_sta, cur_sta, next_sta;

//tracker_states cur_state = ERROR, prev_state = ERROR;
uint8_t flags_1 = 0;

float temp_dist = 0;

int tracker_process()
{
	//gps_get_data();
	int x = gps_process_data();
	
	//if data received is valid
	if(x == 0)
	{
		//compute the distance to next station
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
				if((RelDif(train_status.dist_to_next_sta, 1000.0) < TOLERANCE_DIF))
				{
					train_status.cur_tracker_state = CROSSED_1KM;
				
					x = temp_dist;
						
					//update message
					//repeat times 3 audio, display loop
				}
			case ENROUTE:
				//if course over ground == bearing of the station
				if((RelDif(train_status.dist_to_next_sta, 5000.0) < TOLERANCE_DIF))
				{
					train_status.cur_tracker_state = TOGO_5KM;
					//update message
					//repeat times 3 audio and display
				}
			case TOGO_5KM:
				//if course over ground == bearing of the station
				if((RelDif(train_status.dist_to_next_sta, 1000.0) < TOLERANCE_DIF))
				{
					train_status.cur_tracker_state = TOGO_1KM;
					//update message
					//repeat times 3 audio and display
				}
			case TOGO_1KM:
				if((RelDif(train_status.dist_to_next_sta, 50.0) < TOLERANCE_DIF))
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
				if((RelDif(train_status.dist_to_next_sta, 1000.0) < TOLERANCE_DIF))
				{
					train_status.cur_tracker_state = CROSSED_1KM;
					
					x = temp_dist;
					
					//free train_status.cur_sta
					train_status.cur_sta = train_status.next_sta;
					//raise flag to update next station info
					
					//update message
					//repeat times 3 audio, display loop		
				}
			case CROSSED_1KM:
				//check next sta data updated flag
				
				//adjustment to avoid storage of prev station
				if((RelDif(train_status.dist_to_dest_sta, x - 2.0) < TOLERANCE_DIF))
				{
					train_status.cur_tracker_state = CROSSED_3KM;
					
					//update message
					//repeat times 3 audio, display loop
				}
			case CROSSED_3KM:
				if((RelDif(train_status.dist_to_dest_sta, x - 4.0) < TOLERANCE_DIF))
				{
					x = -500.0;
					train_status.cur_tracker_state = ENROUTE;
				
					//update message
					//repeat times 3 audio, display loop
				}
			case DESTINATION:
				//update message
				break;
			case ERROR:
			case WRONG_DIRECTION:
			case GPS_LOST:
				//put default error message
				break;
			case GPS_WEAK:
				//put slogan and error message
				break;	
		}	
	}
	else
	{
		
	}
	return 0;
}