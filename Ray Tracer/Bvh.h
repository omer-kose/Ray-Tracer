#pragma once

#include "Hittable.h"
#include "Hittable_List.h"
#include "Utility.h"

class Bvh_Node : public Hittable
{
public:
	Bvh_Node();
	
	Bvh_Node(Hittable_List &list) : Bvh_Node(list.objects, 0 , list.objects.size()) {}

	Bvh_Node(
		std::vector<std::shared_ptr<Hittable>> &objects,
		size_t start, size_t end);

	virtual bool hit(const Ray &r, double tmin, double tmax, hit_record &rec) const;
	virtual bool bounding_box(double t0, double t1, Aabb &output_box) const;


public:
	shared_ptr<Hittable> left;
	shared_ptr<Hittable> right;
	Aabb box;

};

bool Bvh_Node::bounding_box(double t0, double t1, Aabb &output_box) const
{
	output_box = box;
	return true;
}

bool Bvh_Node::hit(const Ray &r, double tmin, double tmax, hit_record &rec) const
{
	if (!box.hit(r, tmin, tmin))
		return false;

	/*Bounding Boxes does not carry any hit record info*/
	/*Goes until the model and if there is hit it calls model's hit function*/
	/*If there is no model in that node both left and right will return false resulting in no hit*/
	bool left_hit = left->hit(r, tmin, tmax, rec);
	bool right_hit = right->hit(r, tmin, tmax, rec);

	return left_hit || right_hit;

}