#pragma once

#include "Utility.h"
#include "Perlin.h"
#include "rtw_stb_image.h"

#include <iostream>

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
		return Color(1, 1, 1) * 0.5 * (1 + sin(freq*p.z() + 10 * noise.turb(p))); /*-1:1 -> 0:1*/
	}
private:
	Perlin noise;
	double freq;
};

class Image_Texture : public Texture
{
public:
	const static int bytes_per_pixel = 3;
	Image_Texture()
		:
		data(nullptr),
		width(0),
		height(0),
		bytes_per_scanline(0)
	{}
	Image_Texture(const char *filename)
	{
		auto components_per_pixel = bytes_per_pixel;
		data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);
		if (!data)
		{
			std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
			width = height = 0;
		}
		
		bytes_per_scanline = width * bytes_per_pixel;
	}
	~Image_Texture()
	{
		delete data;
	}
	virtual Color value(double u, double v, const Point3 &p) const
	{
		if (data == nullptr) /*If no data return cyan*/
		{
			return Color(0, 1, 1);
		}

		// Clamp input texture coordinates to [0,1] x [1,0]
		u = clamp(u, 0.0, 1.0);
		v = clamp(v, 0.0, 1.0); 

		/*i and j are pixel coordinates*/
		auto i = static_cast<int>(u * width);
		auto j = static_cast<int>(v * height);
		// Clamp integer mapping, since actual coordinates should be less than 1.0
		if (i >= width)  i = width - 1;
		if (j >= height) j = height - 1;

		const auto color_scale = 1.0 / 255.0;/*to map colors between 0:1*/
		auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

		return Color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
	}
private:
	unsigned char *data;
	int width, height;
	int bytes_per_scanline;
};