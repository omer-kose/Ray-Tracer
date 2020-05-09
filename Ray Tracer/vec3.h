#pragma once
#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;
class Vec3
{
public:
	/*Constructors*/
	Vec3() : e{0, 0, 0} {}/*Default Constructor*/
	Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
	/*Getters*/
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }
	/*Operator Overloads*/
	Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); } /*Negation*/
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	Vec3& operator+=(const Vec3 &v)
	{								
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	Vec3& operator*=(const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	Vec3& operator/=(const double t)
	{
		return *this *= 1 / t;
	}
	double length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	double length() const
	{
		return sqrt(length_squared());
	}

	/*Utility Functions*/



public:
	double e[3];
};

//Type aliases for vec3 since we are going to use it for both colors and points.
//Note that these are just aliases, We use them only clarify intent and use
using Point3 = Vec3; /*3D Point*/
using Color = Vec3; /*RGB Color*/

#endif