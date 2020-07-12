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
			auto t0 = fmin((_min[i]-r.getOrigin()[i])/r.getDir()[i],
							(_max[i]-r.getOrigin()[i])/r.getDir()[i]);

			auto t1 = fmax((_min[i] - r.getOrigin()[i]) / r.getDir()[i],
						(_max[i] - r.getOrigin()[i]) / r.getDir()[i]);

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