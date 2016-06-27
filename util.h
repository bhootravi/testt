#ifndef UTIL_H_
#define UTIL_H_

#include "datatypes.h"

#define u_abs(x)    ((x) < 0 ? -(x) : (x))
#define u_max(a, b) ((a) > (b) ? (a) : (b))

#define TOLERANCE_DIF	0.001

double RelDif(double a, double b);
float distance_between(co_ord loc1, co_ord loc2);

#endif /* UTIL_H_ */