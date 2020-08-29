#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility Functions
inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}
inline double random_double()
{
	//Return a random real number in the range [0,1.0)
	return rand() / (RAND_MAX + 1.0); /* + 1.0 to exlude 1.0*/
}
inline double random_double(double min, double max)
{
	//Return a random real number in the range [min,max)
	return min + (max - min) * random_double();

}
inline int random_int(int min, int max)
{
	/*Returns a random number between [min,max]*/
	return static_cast<int>(random_double(min, max + 1));
}
inline double clamp(double x,double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}



// Common Headers
#include "Ray.h"
#include "vec3.h"
#include "Color.h"
#include "Aabb.h"
#include "onb.h"


#endif
