#ifndef TRACKER_H_
#define TRACKER_H_

#include "uart_helper.h"
#include "util.h"
#include "datatypes.h"
#include "gps.h"

/*
enum tracker_states
{
	SOURCE,
	DESTINATION,
	TOGO_5KM,
	TOGO_1KM,
	ENROUTE,
	STATION_REACHED,
	CROSSED_1KM,
	CROSSED_3KM,
	WRONG_DIRECTION,
	GPS_WEAK,
	GPS_LOST,
	ERROR
};
*/
extern train_struct* cur_train;
extern station* prev_sta, cur_sta, next_sta;

station* get_next_station(void);

int tracker_process(void);
int set_train(void);
int get_cur_sta(void);
int dist_from_cur(station* loc2);

#endif /* TRACKER_H_ */