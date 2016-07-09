#ifndef UTIL_H_
#define UTIL_H_

#include <inttypes.h>
#include "datatypes.h"
#include <math.h>
#include <string.h>

//#include <arm_math.h>
//#include <fastmath.h>

//math functions used for calculations
#define u_abs(x)		((x) < 0 ? -(x) : (x))
#define u_max(a, b)		((a) > (b) ? (a) : (b))
#define u_radians(x)	(((x) * M_PI) / 180.0)

#define	RADIUS_EARTH	6371000.0
//tolerance level for result of RelDif
#define TOLERANCE_DIF	0.001

//computes relative difference of two rational numbers to compare them
double relative_difference(double a, double b);

//calculate distance between two co-ordinates in meters
float distance_between(co_ord loc1, co_ord loc2);

//re-initialize the buffer
void clear_buf(buf_struct* in);

#endif /* UTIL_H_ */