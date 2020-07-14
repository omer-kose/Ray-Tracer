#pragma once

#include "Hittable.h"
#include "Hittable_List.h"
#include "Utility.h"
#include <algorithm>



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
	/*If there is no model at leaf both left and right will return false resulting in no hit*/
	bool left_hit = left->hit(r, tmin, tmax, rec);
	bool right_hit = right->hit(r, tmin, tmax, rec);

	return left_hit || right_hit;

}

Bvh_Node::Bvh_Node(
	std::vector<std::shared_ptr<Hittable>> &objects,
	size_t start, size_t end)
{
	
	int axis = random_int(0, 2);

	auto comparator = (axis == 0) ? box_x_compare :
					  (axis == 1) ? box_y_compare :
					                box_z_compare;

	size_t object_span = end - start;

	if (object_span == 1)
	{
		left = right = objects[start];
	}
	else if (object_span == 2)
	{
		if (comparator(objects[start], objects[start + 1]))
		{
			left = objects[start];
			right = objects[start + 1];
		}
		else
		{
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else
	{
		std::sort(objects.begin() + start, objects.begin() + end, comparator);

		auto mid = (start + end) / 2;
		left = make_shared<Bvh_Node>(objects, start, mid);
		right = make_shared<Bvh_Node>(objects, mid, end);
	}

	Aabb box_left, box_right;

	if (!left->bounding_box(0, 0, box_left) || !right->bounding_box(0, 0, box_right))
	{
		std::cerr << "No bounding box in BVH constructor\n";
	}
	
	box = surrounding_box(box_left, box_right);

}

inline bool box_compare(const shared_ptr<Hittable> &a, const shared_ptr<Hittable> &b, int axis)
{
	Aabb box_a;
	Aabb box_b;

	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
		std::cerr << "No bounding box in bvh_node constructor.\n";


	return box_a.min().e[axis] < box_b.min().e[axis];
}

bool box_x_compare(const shared_ptr<Hittable> &a, const shared_ptr<Hittable> &b)
{
	return box_compare(a, b, 0);
}
bool box_y_compare(const shared_ptr<Hittable> &a, const shared_ptr<Hittable> &b)
{
	return box_compare(a, b, 1);
}
bool box_z_compare(const shared_ptr<Hittable> &a, const shared_ptr<Hittable> &b)
{
	return box_compare(a, b, 2);
}