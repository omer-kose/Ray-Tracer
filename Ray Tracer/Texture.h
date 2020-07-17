#pragma once

#include "Utility.h"
#include "Perlin.h"

class Texture
{
public:
	virtual Color value(double u, double v, const Point3 &p) const = 0;
};

class Solid_Color : public Texture
{
public:
	Solid_Color() {};
	Solid_Color(Color c_in) : color_value(c_in) {}

	Solid_Color(double red, double green, double blue)
		:
		Solid_Color(Color(red,green,blue))
	{}

	virtual Color value(double u, double v, const Point3 &p) const
	{
		return color_value;
	}

private:
	Color color_value;
};

class Checker_Texture : public Texture
{
public:
	Checker_Texture() {}
	Checker_Texture(shared_ptr<Texture> t0, shared_ptr<Texture> t1) : even(t0), odd(t1) {}
	Checker_Texture(Color c1, Color c2)
		: even(make_shared<Solid_Color>(c1)), odd(make_shared<Solid_Color>(c2)) {}

	virtual Color value(double u, double v, const Point3 &p) const
	{
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0)
		{
			return odd->value(u, v, p);
		}
		else
		{
			return even->value(u, v, p);
		}
	}

private:
	shared_ptr<Texture> odd;
	shared_ptr<Texture> even;
};

class Noise_Texture : public Texture
{
public:
	Noise_Texture() {}
	Noise_Texture(double freq_in) : freq(freq_in) {}
	
	virtual Color value(double u, double v, const Point3 &p) const
	{
		return Color(1, 1, 1) * noise.noise(p*freq);
	}
private:
	Perlin noise;
	double freq;
};