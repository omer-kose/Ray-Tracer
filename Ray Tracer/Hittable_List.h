#pragma once
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"
#include <vector>
#include <memory>

/*We'll use shared pointers in our code, 
because it allows multiple geometries to share a common instance
(for example, a bunch of spheres that all use the same texture map material), 
and because it makes memory management automatic and easier to reason about.*/

using std::shared_ptr;
using std::make_shared;

class Hittable_List : public Hittable
{
public:
	Hittable_List() {}
	Hittable_List(shared_ptr<Hittable> object) { add(object); }
	
	void clear() { objects.clear(); }/*Clear all the elements*/
	void add(shared_ptr<Hittable> object) { objects.push_back(object); }

	virtual bool hit(const Ray &ray, double tmin, double tmax, hit_record &rec) const;

public:
	std::vector<shared_ptr<Hittable>> objects;
};


bool Hittable_List::hit(const Ray &ray, double t_min, double t_max, hit_record &rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto &object : objects)
	{
		if (object->hit(ray, t_min, t_max, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;

	
}



#endif
