#include "util.h"


double RelDif(double a, double b)
{
	double c = u_abs(a);
	double d = u_abs(b);

	d = u_max(c, d);

	return d == 0.0 ? 0.0 : u_abs(a - b) / d;
}

//convention - loc1 
float distance_between(co_ord loc1, co_ord loc2)
{
	
}