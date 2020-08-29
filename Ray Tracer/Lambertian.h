#pragma once
#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "Texture.h"
#include "Material.h"
#include "onb.h"

class Lambertian : public Material
{
public:
	Lambertian() {}
	Lambertian(const Color& a) : albedo(make_shared<Solid_Color>(a)) {}
	Lambertian(shared_ptr<Texture> alb) : albedo(alb) {}

	virtual bool scatter(const Ray &ray_in, const hit_record &rec, Color &alb, Ray &scattered, double &pdf) const override
	{
		Onb onb;
		onb.build_from_w(rec.normal);
		auto direction = onb.local(random_cosine_direction());
		scattered = Ray(rec.p, unit_vector(direction));
		alb = albedo->value(rec.u, rec.v, rec.p);
		pdf = dot(onb.w(), scattered.getDir()) / pi;
		return true;
	}

	double scattering_pdf(const Ray &r_in, const hit_record &rec, const Ray &scattered) const override
	{
		auto cosine = dot(rec.normal, unit_vector(scattered.getDir()));
		return cosine < 0 ? 0 : cosine / pi;
	}


public:
	shared_ptr<Texture> albedo;
};

#endif