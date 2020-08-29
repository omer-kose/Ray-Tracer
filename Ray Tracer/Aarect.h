#pragma once

#include "Hittable.h"
#include "Material.h"
#include "Aabb.h"

class XY_Rect : public Hittable
{
public:
	XY_Rect() {}
	XY_Rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<Material> mat)
		: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

	virtual bool hit(const Ray &ray, double t_min, double t_max, hit_record &rec) const override;

	virtual bool bounding_box(double t0, double t1, Aabb &output_box) const override
	{
		// The bounding box must have non-zero width in each dimension, so pad the Z
		// dimension a small amount.
		output_box = Aabb(Point3(x0, y0, k - 0.0001), Point3(x1, y1, k + 0.0001));
		return true;
	}

private:
	shared_ptr<Material> mp;
	double x0, x1, y0, y1,k;
};


bool XY_Rect::hit(const Ray &ray, double t_min, double t_max, hit_record &rec) const
{
	auto t = (k - ray.getOrigin().z()) / ray.getDir().z();
	if (t < t_min || t>t_max) return false;

	auto x = ray.getOrigin().x() + t * ray.getDir().x();
	auto y = ray.getOrigin().y() + t * ray.getDir().y();

	if (x < x0 || x > x1 || y < y0 || y > y1) return false;

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	auto outward_normal = Vec3(0.0, 0.0, 1.0);
	rec.set_face_normal(ray, outward_normal);
	rec.mat_ptr = mp;
	rec.p = ray.at(t);
	return true;	
}


class XZ_Rect : public Hittable {
public:
	XZ_Rect() {}

	XZ_Rect(double _x0, double _x1, double _z0, double _z1, double _k,
		shared_ptr<Material> mat)
		: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

	virtual bool hit(const Ray& r, double t0, double t1, hit_record& rec) const override;

	virtual bool bounding_box(double t0, double t1, Aabb& output_box) const override {
		// The bounding box must have non-zero width in each dimension, so pad the Y
		// dimension a small amount.
		output_box = Aabb(Point3(x0, k - 0.0001, z0), Point3(x1, k + 0.0001, z1));
		return true;
	}

	virtual double pdf_value(const Point3& origin, const Vec3& v) const override 
	{
		hit_record rec;
		if (!this->hit(Ray(origin, v), 0.001, infinity, rec))
			return 0;

		auto area = (x1 - x0)*(z1 - z0);
		auto distance_squared = rec.t * rec.t * v.length_squared();
		auto cosine = fabs(dot(v, rec.normal) / v.length());

		return distance_squared / (cosine * area);
	}

	virtual Vec3 random(const Point3& origin) const override 
	{
		auto random_point = Point3(random_double(x0, x1), k, random_double(z0, z1));
		return random_point - origin;
	}

public:
	shared_ptr<Material> mp;
	double x0, x1, z0, z1, k;
};

class YZ_Rect : public Hittable {
public:
	YZ_Rect() {}

	YZ_Rect(double _y0, double _y1, double _z0, double _z1, double _k,
		shared_ptr<Material> mat)
		: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

	virtual bool hit(const Ray& r, double t0, double t1, hit_record& rec) const override;

	virtual bool bounding_box(double t0, double t1, Aabb& output_box) const override {
		// The bounding box must have non-zero width in each dimension, so pad the X
		// dimension a small amount.
		output_box = Aabb(Point3(k - 0.0001, y0, z0), Point3(k + 0.0001, y1, z1));
		return true;
	}

public:
	shared_ptr<Material> mp;
	double y0, y1, z0, z1, k;
};

bool XZ_Rect::hit(const Ray& r, double t0, double t1, hit_record& rec) const {
	auto t = (k - r.getOrigin().y()) / r.getDir().y();
	if (t < t0 || t > t1)
		return false;
	auto x = r.getOrigin().x() + t * r.getDir().x();
	auto z = r.getOrigin().z() + t * r.getDir().z();
	if (x < x0 || x > x1 || z < z0 || z > z1)
		return false;
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	auto outward_normal = Vec3(0, 1, 0);
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mp;
	rec.p = r.at(t);
	return true;
}

bool YZ_Rect::hit(const Ray& r, double t0, double t1, hit_record& rec) const {
	auto t = (k - r.getOrigin().x()) / r.getDir().x();
	if (t < t0 || t > t1)
		return false;
	auto y = r.getOrigin().y() + t * r.getDir().y();
	auto z = r.getOrigin().z() + t * r.getDir().z();
	if (y < y0 || y > y1 || z < z0 || z > z1)
		return false;
	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	auto outward_normal = Vec3(1, 0, 0);
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mp;
	rec.p = r.at(t);
	return true;
}