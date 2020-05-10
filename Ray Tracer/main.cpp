#include <iostream>
#include "vec3.h"
#include "Color.h"
#include "Ray.h"


bool hit_sphere(const Point3 &center, double radius, const Ray &ray)
{
	Vec3 centerToOrigin = ray.getOrigin() - center;
	Vec3 rayDir = ray.getDir();
	/*a,b,c are coefficients of quadratic equation: (dir*dir) t^2 + (2*cto*dir) + (cto*cto) - r^2 = 0*/
	/*Remember that dir and cto are vectors so we use dot product to multiply them*/
	auto a = dot(rayDir, rayDir);
	auto b = 2.0 * dot(centerToOrigin, rayDir);
	auto c = dot(centerToOrigin, centerToOrigin) - (radius * radius);
	auto discriminant = b * b - 4.0 * a * c;
	return (discriminant > 0);
}
Color ray_color(const Ray &r)
{
	if (hit_sphere(Point3(0, 0, -1), 0.5, r))
	{
		return Color(1.0, 0.0, 0.0);
	}
	Vec3 unit_direction = unit_vector(r.getDir());
	/*Y goes from -1 to 1 approximately(on image plane we do our calculation wrt image plane)*/
	auto t = 0.5 * (unit_direction.y() + 1.0); /*T is an interpolator so we map y between 0 to 1*/
	return (1 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);/*Linear interpolation*/
}

int main()
{
	constexpr auto aspect_ratio = 16.0 / 9.0;
	constexpr int image_width = 384;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	/*Using Right Handed Coordinate System*/
	Point3 rayOrigin(0.0, 0.0, 0.0);
	/*Dimensions of image plane (Alligned with aspect_ratio)*/
	Vec3 horizontal(4.0, 0.0, 0.0);
	Vec3 vertical(0.0, 2.25, 0.0);
	/*We are casting our rays relative to lower_left_corner*/
	Point3 lower_left_corner = rayOrigin - (horizontal / 2) - (vertical / 2) - Vec3(0,0,1);/*Image plane has an offset of 1 along -z dir*/
	for (int j = image_height - 1; j >= 0; j--)//From top to bottom
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++)//From left to right
		{
			auto u = double(i) / (image_width - 1);/*Ranges from 0 to 1 along x axis (left to right)*/
			auto v = double(j) / (image_height - 1);/*Ranges from 0 to 1 along y axis (Top to bottom)*/
			Ray r(rayOrigin, lower_left_corner + u * horizontal + v * vertical);
			Color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}

	std::cerr << "\nDone.\n";
	return 0;
}