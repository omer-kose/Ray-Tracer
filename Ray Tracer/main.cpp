#include <iostream>
#include "Utility.h"


#include "Hittable_List.h"
#include "Sphere.h"
#include "Camera.h"


Color ray_color(const Ray &ray, const Hittable_List &world, int depth)/*depth is the recursive depth*/
{
	hit_record rec;
	/*If ray exceeds the bounce limit, no more light is gathered (no contribution)*/
	if (depth <= 0)
	{
		return Color(0.0, 0.0, 0.0);
	}
	/*Objects*/
	if (world.hit(ray, 0.0, infinity, rec))
	{
		Point3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth - 1);
	}
	Vec3 unit_direction = unit_vector(ray.getDir());
	/*Y goes from -1 to 1 approximately(on image plane we do our calculation wrt image plane)*/
	/*Background*/
	auto t = 0.5 * (unit_direction.y() + 1.0); /*T is an interpolator so we map y between 0 and 1*/
	return (1 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);/*Linear interpolation*/
}

int main()
{
	constexpr auto aspect_ratio = 16.0 / 9.0;
	constexpr int image_width = 384;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	constexpr int samples_per_pixel = 100;
	constexpr int max_depth = 50;

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	/*Using Right Handed Coordinate System*/
	Point3 rayOrigin(0.0, 0.0, 0.0);
	/*Dimensions of image plane (Alligned with aspect_ratio)*/
	Vec3 horizontal(4.0, 0.0, 0.0);
	Vec3 vertical(0.0, 2.0, 0.0);
	/*We are casting our rays relative to lower_left_corner*/
	Point3 lower_left_corner(-2.0,-1.0,-1.0);/*Image plane has an offset of 1 along -z dir*/
	//IMAGE PLANE DEF ENDS
	/*Camera*/
	Camera cam;

	Hittable_List world;
	world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5));
	world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0));

	
	for (int j = image_height - 1; j >= 0; j--)//From top to bottom
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++)//From left to right
		{
			/*Pixel's color*/
			Color pixel_color(0.0,0.0,0.0);
			/*For every pixel we will cast multiple pixels*/
			for (int s = 0; s < samples_per_pixel; s++)
			{
				auto u = (i + random_double()) / (image_width - 1);/*Ranges from 0 to 1 along x axis (left to right)*/
				auto v = (j + random_double()) / (image_height - 1);/*Ranges from 0 to 1 along y axis (Top to bottom)*/
				Ray r = cam.getRay(u,v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(std::cout, pixel_color,samples_per_pixel);
		}
	}

	std::cerr << "\nDone.\n";
	return 0;
}