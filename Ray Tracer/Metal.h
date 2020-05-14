#pragma once
#ifndef METAL_H
#define METAL_H

#include "Material.h"

class Metal : public Material
{
public:
	Metal(const Color &alb) : albedo(alb) {}
	virtual bool scatter(const Ray &ray_in, const hit_record &rec, Color &attenuation, Ray &scattered) const
	{
		Vec3 reflectedDir = reflect(unit_vector(ray_in.getDir()), rec.normal);
		scattered = Ray(rec.p, reflectedDir);
		attenuation = albedo;
		return (dot(scattered.getDir(),rec.normal) > 0);

	}
public:
	Color albedo;
};


#endif