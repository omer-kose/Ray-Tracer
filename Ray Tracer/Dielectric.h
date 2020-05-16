#pragma once
#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "Material.h"

class Dielectric : public Material
{
public:
	Dielectric(double ri) : ref_idx(ri) {}
	double schlick(double cosine, double ref_idx) const/*Shlick Approximation*/
	{
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
	virtual bool scatter(const Ray& ray_in, const hit_record &rec, Color &attenuation, Ray &scattered) const
	{
		attenuation = Color(1.0, 1.0, 1.0);/*No contribution from object it takes light and just passes inside*/
											/*No absorbtion*/
		double etai_over_etat;/*Object ref over environment*/
		/*The name eta is the name of the symbol of index in Greek Alphabet*/
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
		double cosI = dot(-unit_direction, rec.normal);
		double sinI = sqrt(1.0 - cosI*cosI);
		if (etai_over_etat * sinI > 1.0)
		{
			//Must reflect 
			Vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = Ray(rec.p, reflected);
			return true;
		}
		
		//Can refract
		double reflect_probability = schlick(cosI, etai_over_etat);
		if (random_double() < reflect_probability)
		{
			Vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = Ray(rec.p, reflected);
			return true;
		}
		Vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
		scattered = Ray(rec.p, refracted);
		return true;
		
	
	}
public:
	double ref_idx;/*Refractive Index of our material*/
};

#endif