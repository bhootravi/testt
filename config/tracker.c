#include "tracker.h"

#define KM_5_TOGO_F 0
#define KM_1_TOGO_F 1

train* cur_train;
//station* prev_sta, cur_sta, next_sta;

//tracker_states cur_state = ERROR, prev_state = ERROR;
uint8_t flags_1 = 0;

int tracker_process()
{
	gps_get_data();
	gps_process_data();
	
	return 0;
}