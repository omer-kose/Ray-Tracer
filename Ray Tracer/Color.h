#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "Utility.h"
#include <iostream>

void write_color(std::ostream &out, Color pixel_color, int samples_per_pixel)
{
	/*Input pixel color will be the sum of sampled pixel*/
	/*We will take average and clamp then write it*/
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	/*Multiply each channel to get samples' mean to get an average color of pixel*/
	auto scalar = 1.0 / samples_per_pixel;
	r *= scalar;
	g *= scalar;
	b *= scalar;


	//Write the translated [0,255] value of each color component
	out << static_cast<int>(256.0 * clamp(r, 0.0, 1.0)) << ' '
		<< static_cast<int>(256.0 * clamp(g,0.0,1.0)) << ' '
		<< static_cast<int>(255.0 * clamp(b,0.0,1.0)) << '\n';
}

#endif 