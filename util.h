#ifndef UTIL_H_
#define UTIL_H_

#include <inttypes.h>
#include "datatypes.h"
#include <math.h>
//#include <arm_math.h>
//#include <fastmath.h>

#define u_abs(x)		((x) < 0 ? -(x) : (x))
#define u_max(a, b)		((a) > (b) ? (a) : (b))
#define u_radians(x)	(((x) * M_PI) / 180.0)

#define TOLERANCE_DIF	0.001
#define	RADIUS_EARTH	6371000.0

double RelDif(double a, double b);
float distance_between(co_ord loc1, co_ord loc2);

#endif /* UTIL_H_ */