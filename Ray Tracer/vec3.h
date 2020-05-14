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
Vec3 random_in_unit_sphere()/*Returns a random point on the unit sphere*/
{
	auto angle = random_double(0.0, 2 * pi);
	/*Z coordinate of the point*/
	auto z = random_double(-1.0, 1.0);
	/*r is used to keep radius normalized it scales x and y coordinates of point*/
	/*(r*cosa)^2 + (r*sina)^2 + z^2 = 1 then r is:*/
	auto r = sqrt(1 - z * z );
	return Vec3(r*cos(angle), r*sin(angle), z);
}
Vec3 random_in_hemisphere(const Vec3 &normal)/*You may use it if you are not going to offset with normal*/
{
	Vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(normal, in_unit_sphere) > 0.0) /*In the hemisphere*/
	{
		return in_unit_sphere;
	}
	else
	{
		return -in_unit_sphere;
	}
}
Vec3 reflect(const Vec3 &v, const Vec3 &n)
{
	/*In our desing V will be normalized by callee*/
	/*Since v points in the dot(v,n) will be negative so we will negate it again to get the dir of normal*/
	return v - 2 * dot(v, n) * n;
}


//Type aliases for vec3 since we are going to use it for both colors and points.
//Note that these are just aliases, We use them only clarify intent and use
using Point3 = Vec3; /*3D Point*/
using Color = Vec3; /*RGB Color*/

#endif