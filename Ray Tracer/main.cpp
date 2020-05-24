#include <iostream>
#include "Utility.h"
#include "Material.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Camera.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"




Color ray_color(const Ray &ray, const Hittable_List &world, int depth)/*depth is the recursive depth*/
{
	hit_record rec;
	/*If ray exceeds the bounce limit, no more light is gathered (no contribution)*/
	if (depth <= 0)
	{
		return Color(0.0, 0.0, 0.0);
	}
	
	/*Objects*/
	/*0.001 because of floating point approximation errors. */
	/*Computer might not distinguish whether ray hit to the object or not since at intersection point
	t is very small. Therefore, it might interpret it as a not hit point so, we compensate that by
	adding using an epsilon 0.001*/
	if (world.hit(ray, 0.001, infinity, rec))
	{
		Ray scattered;
		Color attenuation;
		if (rec.mat_ptr->scatter(ray, rec, attenuation, scattered))
		{
			return attenuation * ray_color(scattered, world, depth-1);
		}
		return Color(0.0, 0.0, 0.0);
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
	Point3 lookFrom(3.0, 3.0, 2.0);
	Point3 lookAt(0.0, 0.0, -1.0);
	Vec3 vUp(0.0, 1.0, 0.0);
	auto dist_to_focus = (lookFrom - lookAt).length();
	auto aperture = 2.0;
	Camera cam(lookFrom, lookAt, vUp, 20, aspect_ratio, aperture, dist_to_focus);

	Hittable_List world;
	world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5,make_shared<Lambertian>(Color(0.1, 0.2, 0.5))));
	world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100, make_shared<Lambertian>(Color(0.8, 0.8, 0.0))));


	
	world.add(make_shared<Sphere>(Point3(1, 0, -1), 0.5, make_shared<Metal>(Color(.8, .6, .2), 0.0)));
	/*When you combine these you will get a bubble*/
	world.add(make_shared<Sphere>(Point3(-1, 0,-1), 0.5, make_shared<Dielectric>(1.5)));
	/*When radius is negated normals are reversed*/
	world.add(make_shared<Sphere>(Point3(-1, 0, -1), -0.45, make_shared<Dielectric>(1.5)));

	
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