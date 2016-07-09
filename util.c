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
	float lat1 = u_radians(loc1.lat);
	float lat2 = u_radians(loc2.lat);
	float d_lat = lat1 - lat2;
	float d_lon = u_radians(loc1.lon - loc2.lon);
	
	float a = sin(d_lat) * sin(d_lat)
					+ sin(lat1) * sin(lat2)
						* sin(d_lon) * sin(d_lon);
	float c = 2 * atan2(sqrt(a), sqrt(1-a));
	
	return RADIUS_EARTH*c;
	
}

void clear_buf(buf_struct *in)
{
	memset(in->buf_ptr, 0, in->len);
	
	in->ptr = in->buf_ptr;
	memset(in->eol, 0, 16);
	
	//for (int i = 0; i < 8; i++)
		//in->eol[i] = 0;
	//in->eol = {0,0,0,0,0,0,0,0};
	in->eol_i = 1;
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