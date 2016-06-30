#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <inttypes.h>

#define STA_NAME_LEN 50
#define STA_CODE_LEN 4
#define TRAIN_NO_LEN 6
#define TRAIN_NAME_LEN 120

typedef struct  
{
	float lat;
	float lon;
}co_ord;

typedef struct 
{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}time_struct;

typedef struct 
{
	uint8_t date;
	uint8_t month;
	uint16_t year;
	uint8_t day;
}date_struct;

typedef struct 
{
	time_struct t;
	date_struct d;	
}timeDate_struct;

typedef struct 
{
	co_ord cur_loc;
	float speed;
	timeDate_struct timeDate;  
}gps_data;

typedef struct
{
	timeDate_struct arrival;
	timeDate_struct departure;
	uint8_t platform_no;
}halt_details;

typedef struct
{
	uint8_t code[STA_CODE_LEN];
	co_ord location;
	uint8_t reg_lang;
	uint8_t name_eng[STA_NAME_LEN];
	uint8_t name_hin[STA_NAME_LEN];
	uint8_t name_reg[STA_NAME_LEN];
	halt_details* halt;
}station;

typedef struct
{
	uint8_t number[TRAIN_NO_LEN];
	uint8_t name_eng[TRAIN_NAME_LEN];
	uint8_t name_hin[TRAIN_NAME_LEN];
	uint8_t name_reg[TRAIN_NAME_LEN];
	uint16_t stops;
	station* src;
	station* dest;
	//????//halt_station* tt_tail;
	uint8_t type;
	uint8_t direction;
	////////intermediate stops
}train_struct;

typedef enum
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
	//GPS_LOST,
	ERROR
}tracker_state_enum;


typedef struct
{
	train_struct* train_data;
	
	station* cur_sta;
	station* next_sta;
	
	gps_data cur_gps_data;
	
	float dist_to_next_sta;
	float dist_to_dest_sta;
	
	time_struct late_duration;
	
	timeDate_struct last_update_time;
	
	tracker_state_enum cur_tracker_state;
	tracker_state_enum prev_tracker_state;
}train_status_struct;


#endif