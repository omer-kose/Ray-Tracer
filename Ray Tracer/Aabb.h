#pragma once
#include "Utility.h"

class Aabb
{
public:
	Aabb() {}
	Aabb(const Point3 &a, const Point3 &b) { _min = a, _max = b; }

	Point3 min() const { return _min; }
	Point3 max() const { return _max; }

	bool hit(const Ray& r, double tmin, double tmax) const
	{
		for (int i = 0; i < 3; i++) /*Traversing all axes*/
		{
			auto invDir = 1.0f / r.getDir()[i];
			auto t0 = (_min[i] - r.getOrigin()[i]) * invDir;
			auto t1 = (_max[i] - r.getOrigin()[i]) * invDir;
			
			if (invDir < 0.0f)
			{
				std::swap(t0, t1);
			}

			tmin = fmax(t0,tmin);
			tmax = fmin(t1, tmax);

			if (tmax <= tmin)
				return false;

		}

		return true;
	}
private:
	Point3 _min;
	Point3 _max;
};


Aabb surrounding_box(const Aabb &box0, const Aabb &box1)
{
	Point3 small(fmin(box0.min().x(),box1.min().x()),
				fmin(box0.min().y(), box1.min().y()),
				fmin(box0.min().z(), box1.min().z()));
	
	
	Point3 big(fmin(box0.max().x(), box1.max().x()),
		fmin(box0.max().y(), box1.max().y()),
		fmin(box0.max().z(), box1.max().z()));

	return Aabb(small, big);

}