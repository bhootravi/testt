#ifndef GPS_H_
#define GPS_H_

#include "uart.h"

void gps_init(void);

void gps_get_data(void);
void gps_process_data(void);
int gps_load_settings(void);

void gps_cold_start(void);
void gps_warm_start(void);
void gps_hot_start(void);


#endif /* GPS_H_ */