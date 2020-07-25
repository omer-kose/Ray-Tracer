#pragma once

#include "Texture.h"
#include "Material.h"


class Isotropic : public Material
{
public:
	Isotropic(Color c) : albedo(make_shared<Solid_Color>(c)) {}
	Isotropic(shared_ptr<Texture> a) : albedo(a) {}

	virtual bool scatter(const Ray &r_in, const hit_record& rec, Color& attenuation, Ray &scattered) const override 
	{
		scattered = Ray(rec.p, random_on_unit_sphere());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

private:
	shared_ptr<Texture> albedo;
};