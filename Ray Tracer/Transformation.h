#pragma once

#include "Utility.h"
#include "Hittable.h"

class Translate : public Hittable
{
public:
	Translate() {}
	Translate(shared_ptr<Hittable> p, const Vec3 &displacement) : ptr(p) , offset(displacement) {}
	
	virtual bool hit(const Ray &ray, double t_min, double t_max, hit_record &rec) const override;

	virtual bool bounding_box(double t0, double t1, Aabb& output_box) const override;
	

private:
	shared_ptr<Hittable> ptr;
	Vec3 offset;
};

bool Translate::hit(const Ray &ray, double t_min, double t_max, hit_record &rec) const
{
	/*To create the impression that object is offsetted we offset the incident ray*/
	Ray moved_r(ray.getOrigin() - offset, ray.getDir()); 
	if (!ptr->hit(moved_r, t_min, t_max, rec))
		return false;

	rec.p += offset;/*Offset hitpoints to be able to be able to make lighting, texturing calculations properly*/
	rec.set_face_normal(moved_r, rec.normal);

	return true;
}

bool Translate::bounding_box(double t0, double t1, Aabb& output_box) const 
{
	if (!ptr->bounding_box(t0, t1, output_box))
		return false;

	output_box = Aabb(
		output_box.min() + offset,
		output_box.max() + offset);

	return true;
}



class Rotate_Y : public Hittable
{
public:
	Rotate_Y() {}
	Rotate_Y(shared_ptr<Hittable> p, double angle);

	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

	virtual bool bounding_box(double t0, double t1, Aabb& output_box) const override 
	{
		output_box = bbox;
		return hasbox;
	}

private:
	shared_ptr<Hittable> ptr;
	double sin_theta;
	double cos_theta;
	bool hasbox;
	Aabb bbox;
};

Rotate_Y::Rotate_Y(shared_ptr<Hittable> p, double angle) : ptr(p)
{
	auto radians = degrees_to_radians(angle);
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->bounding_box(0, 1, bbox);

	Point3 min(infinity, infinity, infinity);
	Point3 max(-infinity, -infinity, -infinity);

	/*Determining the boundaries for the bounding box*/
	for (int i = 0; i < 2; i++) 
	{
		for (int j = 0; j < 2; j++) 
		{
			for (int k = 0; k < 2; k++) 
			{
				/*Get current boundary points*/
				auto x = i * bbox.max().x() + (1 - i)*bbox.min().x();
				auto y = j * bbox.max().y() + (1 - j)*bbox.min().y();
				auto z = k * bbox.max().z() + (1 - k)*bbox.min().z();

				auto newx = cos_theta * x + sin_theta * z;
				auto newz = -sin_theta * x + cos_theta * z;

				
				Vec3 tester(newx, y, newz);

				/*Determine the boundaries*/
				for (int c = 0; c < 3; c++) {
					min[c] = fmin(min[c], tester[c]);
					max[c] = fmax(max[c], tester[c]);
				}
			}
		}
	}

	bbox = Aabb(min, max);

}

bool Rotate_Y::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
	/*Rotating ray source (only for the rotated object) to create the impression of rotation*/
	/*Negative rotation*/
	auto origin = r.getOrigin();
	auto direction = r.getDir();

	origin[0] = cos_theta * r.getOrigin()[0] - sin_theta * r.getOrigin()[2];
	origin[2] = sin_theta * r.getOrigin()[0] + cos_theta * r.getOrigin()[2];

	direction[0] = cos_theta * r.getDir()[0] - sin_theta * r.getDir()[2];
	direction[2] = sin_theta * r.getDir()[0] + cos_theta * r.getDir()[2];

	Ray rotated_r(origin, direction);

	if (!ptr->hit(rotated_r, t_min, t_max, rec))
		return false;

	/*To make calculations properly we should rotate the hit point and normals*/
	auto p = rec.p;
	auto normal = rec.normal;

	p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
	p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

	normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
	normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

	rec.p = p;
	rec.set_face_normal(rotated_r, normal);

	return true;
}