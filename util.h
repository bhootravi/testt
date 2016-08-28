#ifndef UTIL_H_
#define UTIL_H_

#include <inttypes.h>
#include "datatypes.h"
#include <math.h>
#include <string.h>
#include "ring_buf.h"

//#include <arm_math.h>
//#include <fastmath.h>

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))

//math functions used for calculations
#define u_abs(x)		((x) < 0 ? -(x) : (x))
#define u_max(a, b)		((a) > (b) ? (a) : (b))
#define u_radians(x)	(((x) * M_PI) / 180.0)

//#define	RADIUS_EARTH	6371000.0
#define	RADIUS_EARTH	6371.0
//tolerance level for result of RelDif
#define TOLERANCE_DIF	0.1

//computes relative difference of two rational numbers to compare them
double relative_difference(double a, double b);

//calculate distance between two co-ordinates in meters
float distance_between(co_ord loc1, co_ord loc2);

#endif /* UTIL_H_ */