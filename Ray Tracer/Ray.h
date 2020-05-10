#pragma once
#ifndef RAY_H
#define RAY_H

#include "vec3.h"
class Ray
{
public:
	Ray() {}
	Ray(const Point3 &rayOrigin, const Vec3& direction)
		:
		origin(rayOrigin),
		dir(direction)
	{}
	Point3 getOrigin() const { return origin; }
	Vec3 getDir() const { return dir; }

	Point3 at(double t) const
	{
		return origin + t * dir;
	}

public:
	Point3 origin;
	Vec3 dir;
};


#endif