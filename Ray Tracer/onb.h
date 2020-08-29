#pragma once
#ifndef ONB_H
#define ONB_H

#include "Utility.h"

class Onb
{
public:
	Onb() {}
	inline Vec3 operator[](int i) const { return axis[i]; }

	Vec3 u() const { return axis[0]; }
	Vec3 v() const { return axis[1]; }
	Vec3 w() const { return axis[2]; }

	Vec3 local(double a, double b, double c)
	{
		return a * u() + b * v() + c * w();
	}

	Vec3 local(const Vec3 &a) const
	{
		return a.x()*u() + a.y() * v() + a.z() * w();
	}

	void build_from_w(const Vec3&);

private:
	Vec3 axis[3];
};

void Onb::build_from_w(const Vec3 &n)
{
	axis[2] = unit_vector(n);
	Vec3 a = (w().x() > 0.9) ? Vec3(0.0, 1.0, 0.0) : Vec3(1.0, 0.0, 0.0);
	axis[1] = unit_vector(cross(w(), a));
	axis[0] = cross(w(), v());
}



#endif