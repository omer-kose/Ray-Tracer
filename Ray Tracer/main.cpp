#include <iostream>
#include "vec3.h"
#include "Color.h"

int main()
{
	constexpr int image_width = 256;
	constexpr int image_height = 256;

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--)//From top to bottom
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++)//From left to right
		{
			auto r = double(i) / (image_width - 1);//From left to right, red channel goes from black to full brightness.
			auto g = double(j) / (image_height - 1);//From top to bottom, green channel goes from black to full brightness
			auto b = 0.25;
			Color pixel_color(r, g, b);
			write_color(std::cout, pixel_color);
		}
	}

	std::cerr << "\nDone.\n";
	return 0;
}