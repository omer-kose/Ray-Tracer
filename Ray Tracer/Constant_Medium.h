#pragma once

#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "Utility.h"

#include "Hittable.h"
#include "Material.h"
#include "Texture.h"
#include "Isotropic.h"

class Constant_Medium : public Hittable
{
public:
	Constant_Medium () {}
	Constant_Medium(shared_ptr<Hittable> b, double d, shared_ptr<Texture> a)
		:
		boundary(b),
		neg_inv_density(-1/d),
		phase_function(make_shared<Isotropic>(a))
	{}

	Constant_Medium(shared_ptr<Hittable> b, double d, Color c)
		: boundary(b),
		neg_inv_density(-1 / d),
		phase_function(make_shared<Isotropic>(c))
	{}

	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
	
	virtual bool bounding_box(double t0, double t1, Aabb& output_box) const override 
	{
		return boundary->bounding_box(t0, t1, output_box);
	}


private:
	shared_ptr<Hittable> boundary;
	shared_ptr<Material> phase_function;
	double neg_inv_density;
};

bool Constant_Medium::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
	// Print occasional samples when debugging. To enable, set enableDebug true.
	const bool enableDebug = false;
	const bool debugging = enableDebug && random_double() < 0.00001;

	/*rec1 is closer boundary, rec2 is far boundary*/
	hit_record rec1, rec2;

	if (!boundary->hit(r, -infinity, infinity, rec1))
		return false;

	if (!boundary->hit(r, rec1.t + 0.0001, infinity, rec2))
		return false;

	if (debugging) std::cerr << "\nt0=" << rec1.t << ", t1=" << rec2.t << '\n';

	if (rec1.t < t_min) rec1.t = t_min;
	if (rec2.t > t_max) rec2.t = t_max;

	if (rec1.t >= rec2.t)
		return false;

	if (rec1.t < 0)
		rec1.t = 0;

	const auto ray_length = r.getDir().length();
	const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
	const auto hit_distance = neg_inv_density * log(random_double());

	if (hit_distance > distance_inside_boundary)
		return false;

	/*div hit_dist by ray_length since we use rec.t * (dir.length()) to find hitpoint*/
	rec.t = rec1.t + hit_distance / ray_length;
	rec.p = r.at(rec.t);

	if (debugging) {
		std::cerr << "hit_distance = " << hit_distance << '\n'
			<< "rec.t = " << rec.t << '\n'
			<< "rec.p = " << rec.p << '\n';
	}

	rec.normal = Vec3(1, 0, 0);  // arbitrary
	rec.front_face = true;     // also arbitrary
	rec.mat_ptr = phase_function;

	return true;
}


#endif