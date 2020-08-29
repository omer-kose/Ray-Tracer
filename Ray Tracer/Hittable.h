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
	/*Texture Coordinates*/
	double u;
	double v;
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
	virtual bool bounding_box(double t0, double t1, Aabb &output_box) const = 0;
	virtual double pdf_value(const Point3& o, const Vec3& v) const
	{
		return 0.0;
	}

	virtual Vec3 random(const Vec3& o) const 
	{
		return Vec3(1, 0, 0);
	}
};

void get_sphere_uv(const Vec3 &p, double &u, double &v) /*assuming a unit sphere*/
{
	auto phi = acos(p.y());
	auto theta = atan2(p.x(), p.z());
	if (theta >= 0) theta /= 2 * pi;
	else theta = (theta / (2 * pi)) + 1;
	u = theta;
	v = phi / pi;
}

class Flip_Face : public Hittable
{
public:
	Flip_Face(shared_ptr<Hittable> p) : ptr(p) {}
	virtual bool hit(const Ray &ray, double t_min, double t_max, hit_record &rec) const override
	{
		if (!ptr->hit(ray, t_min, t_max,rec))
		{
			return false;
		}

		rec.front_face = !rec.front_face;
		return true;
	}
	virtual bool bounding_box(double t0, double t1, Aabb &output_box) const override
	{
		return ptr->bounding_box(t0, t1, output_box);
	}
private:
	shared_ptr<Hittable> ptr;
};



#endif
