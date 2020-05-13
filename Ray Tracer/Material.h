#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
#include "Ray.h"
#include "Hittable.h"

class Material
{
public:
	virtual bool scatter(const Ray &r_in, const hit_record &rec, Color &attenuation, Ray &scattered) const = 0;
};

#endif