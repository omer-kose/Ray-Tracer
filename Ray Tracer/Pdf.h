#pragma once
#ifndef PDF_H
#define PDF_H

#include "Utility.h"

/*PDF's have 2 basic functions*/
/*First: Generate random sampels according to some rule. (Towards light or objects for example)*/
/*Second: Return a PDF Value depending on that PDF*/



class Pdf
{
public:
	virtual ~Pdf() {}

	virtual double value(const Vec3 &direction) const = 0;
	virtual Vec3 generate() const = 0;
};


inline Vec3 random_cosine_direction()
{
	auto r1 = random_double();
	auto r2 = random_double();
	auto z = sqrt(1 - r2);

	auto phi = 2 * pi * r1;
	auto x = cos(phi) * sqrt(r2);
	auto y = sin(phi) * sqrt(r2);

	return Vec3(x, y, z);
}

class Cosine_Pdf : public Pdf {
public:
	Cosine_Pdf(const Vec3& w) { uvw.build_from_w(w); }

	virtual double value(const Vec3& direction) const override { /*Returns a pdf value*/
		auto cosine = dot(unit_vector(direction), uvw.w());
		return (cosine <= 0) ? 0 : cosine / pi;
	}

	virtual Vec3 generate() const override { /*Generates a random direction vector*/
		return uvw.local(random_cosine_direction());
	}

public:
	Onb uvw;
};

class Hittable_Pdf : public Pdf
{
public:
	Hittable_Pdf(shared_ptr<Hittable> p, const Point3& origin) : ptr(p), o(origin) {}

	virtual double value(const Vec3& direction) const override 
	{
		return ptr->pdf_value(o, direction);
	}

	virtual Vec3 generate() const override 
	{
		return ptr->random(o);
	}

public:
	Point3 o;
	shared_ptr<Hittable> ptr;
};


#endif