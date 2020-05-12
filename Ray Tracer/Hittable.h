#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

struct hit_record
{
	Point3 p;
	Vec3 normal;
	double t;
	bool front_face;

	inline void set_face_normal(const Ray &ray, const Vec3 &outward_normal)
	{
		/*Normal directions are determined with respect to ray direction*/
		/*Front face normals should look to us, whereas back face normals should look inside*/
		/*We defined outward_normals to look always outside of the surface*/
		/*So, with this function we correct them*/
		front_face = dot(ray.getDir(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}

};

class Hittable
{
public:
	virtual bool hit(const Ray &ray, double t_min, double t_max, hit_record &rec) const = 0;/*Pure virtual function*/
};


#endif