#pragma once
#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "Material.h"

class Lambertian : public Material
{
public:
	Lambertian(const Color &alb) : albedo(alb) {}

	virtual bool scatter(const Ray &ray_in, const hit_record &rec, Color &attenuation, Ray &scattered) const
	{
		/*Lambertian scatter direction*/
		/*Offsetting with normal so that we create a unit tangent sphere at the intersection point*/
		/*Therefore, scattered rays wont go inside of the sphere*/
		Vec3 scattered_direction = rec.normal + random_on_unit_sphere();
		/*Creating a new ray with origin at interseciton point*/
		scattered = Ray(rec.p, scattered_direction);
		attenuation = albedo;
		return true;
	}

public:
	Color albedo;
};

#endif