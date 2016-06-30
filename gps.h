#ifndef GPS_H_
#define GPS_H_

/*
Provides helper functions for tracker to:
	get periodic updates of location, time and speed data
	recover from fix lost or error conditions
	change settings(update intervals, serial config)
*/

#include "minmea.h"


void gps_init(void);

void gps_get_data(void);
int gps_process_data(void);
int gps_load_settings(void);

void gps_cold_start(void);
void gps_warm_start(void);
void gps_hot_start(void);


#endif /* GPS_H_ */