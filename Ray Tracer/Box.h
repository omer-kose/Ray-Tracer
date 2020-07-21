#pragma once
#ifndef BOX_H
#define BOX_H

#include "Utility.h"
#include "Aarect.h"
#include "Hittable_List.h"

class Box : public Hittable
{
public:
	Box() {}
	Box(const Point3 &p0, const Point3 &p1, shared_ptr<Material> ptr);

	virtual bool hit(const Ray& ray, double t0, double t1, hit_record& rec) const override;

	virtual bool bounding_box(double t0, double t1, Aabb& output_box) const override {
		output_box = Aabb(box_min, box_max);
		return true;
	}
private:
	Point3 box_min;
	Point3 box_max;
	Hittable_List sides;
};

Box::Box(const Point3& p0, const Point3& p1, shared_ptr<Material> ptr)
{
	box_min = p0;
	box_max = p1;

	sides.add(make_shared<XY_Rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
	sides.add(make_shared<XY_Rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

	sides.add(make_shared<XZ_Rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
	sides.add(make_shared<XZ_Rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

	sides.add(make_shared<YZ_Rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
	sides.add(make_shared<YZ_Rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}
bool Box::hit(const Ray& r, double t0, double t1, hit_record& rec) const {
	return sides.hit(r, t0, t1, rec);
}

#endif