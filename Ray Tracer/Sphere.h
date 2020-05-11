#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "vec3.h"

class Sphere : public Hittable
{
public:
	Sphere() {};
	Sphere(Point3 cen, double r)
		:
		center(cen),
		radius(r)
	{};
	virtual bool hit(const Ray &ray, double t_min, double t_max, hit_record &rec) const;
public:
	Point3 center;
	double radius;
};


bool Sphere::hit(const Ray &ray, double t_min, double t_max, hit_record &rec) const
{
	Vec3 centerToOrigin = ray.getOrigin() - center;
	Vec3 rayDir = ray.getDir();
	/*a,b,c are coefficients of quadratic equation: (dir*dir) t^2 + (2*cto*dir) + (cto*cto) - r^2 = 0*/
	/*Remember that dir and cto are vectors so we use dot product to multiply them*/
	auto a = dot(rayDir, rayDir);
	auto half_b = dot(centerToOrigin, rayDir);
	auto c = dot(centerToOrigin, centerToOrigin) - (radius * radius);
	auto discriminant = half_b * half_b - a * c;

	if (discriminant > 0)
	{
		auto root = sqrt(discriminant);
		auto t = (-half_b - root) / a;
		if (t < t_max && t > t_min)
		{
			rec.t = t;
			rec.p = ray.at(t);
			Vec3 outward_normal = (rec.p - center) / radius;/*Dividing by radius normalizes normal vector 
												    since it is from center to surface, its length is exactly radius.*/
			
			rec.set_face_normal(ray, outward_normal);
			return true;
		}
		t = (-half_b + root) / a;
		if (t < t_max && t > t_min)
		{
			rec.t = t;
			rec.p = ray.at(t);
			Vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(ray, outward_normal);
			return true;
		}
	}

	return false;
}





#endif