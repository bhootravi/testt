#include "util.h"

double relative_difference(double a, double b)
{
	double c = u_abs(a);
	double d = u_abs(b);

	d = u_max(c, d);

	return d == 0.0 ? 0.0 : u_abs(a - b) / d;
}

//convention - loc1 
float distance_between(co_ord loc1, co_ord loc2)
{
	double lat1 = u_radians(loc1.lat);
	double lat2 = u_radians(loc2.lat);
	double d_lat = lat1 - lat2;
	double d_lon = u_radians(loc1.lon - loc2.lon);
	
	double a = sin(d_lat/2) * sin(d_lat/2)
					+ cos(lat1) * cos(lat2)
						* sin(d_lon/2) * sin(d_lon/2);
	//reusing lat1 and lat2
	lat1 = sqrt(a);
	lat2 = sqrt(1-a);
	
	double c = 2 * atan2(lat1, lat2);
	
	return RADIUS_EARTH*c;
}

/*
float bearing(co_ord loc1, co_ord loc2)
{
	
	float lat1 = u_radians(loc1.lat);
	float lat2 = u_radians(loc2.lat);
	float d_lat = lat1 - lat2;
	var y = Math.sin(?2-?1) * Math.cos(?2);
	var x = Math.cos(?1)*Math.sin(?2) -
	Math.sin(?1)*Math.cos(?2)*Math.cos(?2-?1);
	var brng = Math.atan2(y, x).toDegrees();
	
}
*/