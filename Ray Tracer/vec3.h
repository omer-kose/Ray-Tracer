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

	inline static Vec3 random()
	{
		return Vec3(random_double(), random_double(), random_double());
	}
	inline static Vec3 random(double min, double max)
	{
		return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	

public:
	double e[3];
};

/*Utility Functions*/
/*When an operator is defined inside, left operand is current instance. Therefore, to be able to define
in this way we have to define them outside of the class(using any vector not only current instance we need to define it outside of class)*/

inline std::ostream& operator<<(std::ostream &out, const Vec3 &v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
{
	return Vec3(lhs.e[0] + rhs.e[0], lhs.e[1] + rhs.e[1], lhs.e[2] + rhs.e[2]);
}

inline Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
{
	return Vec3(lhs.e[0] - rhs.e[0], lhs.e[1] - rhs.e[1], lhs.e[2] - rhs.e[2]);
}

inline Vec3 operator*(const Vec3 &lhs, const Vec3 &rhs)/*Hadamard Product*/
{
	return Vec3(lhs.e[0] * rhs.e[0], lhs.e[1] * rhs.e[1], lhs.e[2] * rhs.e[2]);
}

inline Vec3 operator*(double t, const Vec3 &v)
{
	return Vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}
/*Multiplication by a scalar is commutative*/
inline Vec3 operator*(const Vec3 &v, double t) 
{
	return t * v;
}

inline Vec3 operator/(const Vec3 &v, double t)
{
	return (1/t) * v;
}

inline double dot(const Vec3 &lhs, const Vec3 &rhs)
{
	return lhs.e[0] * rhs.e[0] + lhs.e[1] * rhs.e[1] + lhs.e[2] * rhs.e[2];
}

inline Vec3 cross(const Vec3 &lhs, const Vec3 &rhs)
{
	return Vec3(lhs.e[1] * rhs.e[2] - lhs.e[2] * rhs.e[1],
		lhs.e[2] * rhs.e[0] - lhs.e[0] * rhs.e[2],
		lhs.e[0] * rhs.e[1] - lhs.e[1] * rhs.e[0]);
}

inline Vec3 unit_vector(Vec3 v)
{
	return v / v.length();
}
Vec3 random_in_unit_sphere()
{
	while (true)
	{
		auto p = Vec3::random(-1.0, 1.0);
		if (p.length_squared() >= 1.0) continue;
		return p;
	}
}




//Type aliases for vec3 since we are going to use it for both colors and points.
//Note that these are just aliases, We use them only clarify intent and use
using Point3 = Vec3; /*3D Point*/
using Color = Vec3; /*RGB Color*/

#endif