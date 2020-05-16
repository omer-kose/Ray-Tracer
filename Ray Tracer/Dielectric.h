#pragma once
#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "Material.h"

class Dielectric : public Material
{
public:
	Dielectric(double ri) : ref_idx(ri) {}
	virtual bool scatter(const Ray& ray_in, const hit_record &rec, Color &attenuation, Ray &scattered) const
	{
		attenuation = Color(1.0, 1.0, 1.0);/*No contribution from object it takes light and just passes inside*/
											/*No absorbtion*/
		double etai_over_etat;/*Object ref over environment*/
		/*We are assuming environment consists of pure air (ref_idx = 1.0)=*/
		if (rec.front_face)
		{
			etai_over_etat = 1.0 / ref_idx;/*env / mat_ref_idx*/
		}
		else
		{
			etai_over_etat = ref_idx;/*mat_ref_idx / env*/
		}
		Vec3 unit_direction = unit_vector(ray_in.getDir());/*We need incoming dir to refract*/
		Vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
		scattered = Ray(rec.p, refracted);
		return true;
	}
public:
	double ref_idx;/*Refractive Index of our material*/
};

#endif