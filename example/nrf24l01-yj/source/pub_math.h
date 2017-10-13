#ifndef __C_PUBLIC_MATH_H
#define __C_PUBLIC_MATH_H

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "pub_types.h"

#define B_G					(9.80665f)
#define B_E					(2.71828f)
#define B_PI				(3.141592653589793f)


#define b_min(a,b) 			(((a)<(b))?(a):(b))
#define b_max(a,b) 			(((a)>(b))?(a):(b))
#define b_clip(val,min,max)	(((val)<(min)) ? (min) : (((val)>(max)) ? (max) : (val)))
#define b_arraysize(a)		(sizeof(a) / sizeof((a)[0]))
#define b_radians(a)		(a * B_PI / 180.0f)
#define b_degrees(a)		(a * 180.0f / B_PI)
#define b_checkrange(v,min,max) (v >= min && v <= max)
#define b_read(a)			(a == m_read)
#define b_write(a)			(a == m_write)

#endif

