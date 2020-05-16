#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "Utility.h"
#include "Ray.h"

/*Hittables and materials need to know each other so there is some circularity of the references
So we forward declare material class to be able to create references to it, without needing to know
layout of the class*/
/*By forward declaring: We avoid including Material.h , which slightly speeds up compilation time*/
class Material;


struct hit_record
{
	Point3 p;
	Vec3 normal;
	shared_ptr<Material> mat_ptr;
	double t;
	bool front_face;

	inline void set_face_normal(const Ray &ray, const Vec3 &outward_normal)
	{
		/*Normal directions are determined with respect to ray direction*/
		/*Front face normals should look to us, whereas back face normals should look inside*/
		/*We defined outward_normals to look always outside of the surface*/
		/*So, with this function we correct them*/
		front_face = dot(ray.getDir(), outward_normal) < 0;
		/*Reverse backface normals to correctly calculate light calculations*/
		normal = front_face ? outward_normal : -outward_normal;
	}

};

class Hittable
{
public:
	virtual bool hit(const Ray &ray, double t_min, double t_max, hit_record &rec) const = 0;/*Pure virtual function*/
};


#endif
