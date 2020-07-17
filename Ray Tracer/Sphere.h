#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "vec3.h"

class Sphere : public Hittable
{
public:
	Sphere() {};
	Sphere(Point3 cen, double r, shared_ptr<Material> m)
		:
		center(cen),
		radius(r),
		mat_ptr(m)
	{};
	virtual bool hit(const Ray &ray, double t_min, double t_max, hit_record &rec) const;
	virtual bool bounding_box(double t0, double t1, Aabb &output_box) const;
public:
	Point3 center;
	double radius;
	shared_ptr<Material> mat_ptr;
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
			/*Normals are normalized in here*/
			Vec3 outward_normal = (rec.p - center) / radius;/*Dividing by radius normalizes normal vector 
												    since it is from center to surface, its length is exactly radius.*/
			
			rec.set_face_normal(ray, outward_normal);
			rec.mat_ptr = mat_ptr;
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			return true;
		}
		t = (-half_b + root) / a;
		if (t < t_max && t > t_min)
		{
			rec.t = t;
			rec.p = ray.at(t);
			Vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(ray, outward_normal);
			rec.mat_ptr = mat_ptr;
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			return true;
		}
	}

	return false;
}

bool Sphere::bounding_box(double t0, double t1, Aabb &output_box) const
{
	output_box = Aabb(
					center - Vec3(radius,radius,radius),
					center + Vec3(radius,radius,radius));
	
	return true;
}


#endif