#include <iostream>
#include "Utility.h"
#include "Material.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Camera.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"
#include "Bvh.h"
#include "Texture.h"



Hittable_List random_scene()
{
	Hittable_List world;

	auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	world.add(make_shared<Sphere>(Point3(0.0, -1000.0, 0.0), 1000, ground_material));

	for (int i = -11; i < 11; i++)
	{
		for (int j = -11; j < 11; j++)
		{
			/*Create a random material*/
			auto choose_mat = random_double();
			Point3 center(i + 0.9 * random_double(), 0.2, j + 0.9 * random_double());

			if((center - Vec3(4.0,0.2,0.0)).length() > 0.9)
			{
				shared_ptr<Material> sphere_material;
				if (choose_mat < 0.8)
				{
					//Diffuse
					auto albedo = make_shared<Solid_Color>(Color::random() * Color::random());
					sphere_material = make_shared<Lambertian>(albedo);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));

				}
				else if (choose_mat < 0.95)
				{
					//Metal
					auto albedo = Color::random(0.5, 1.0);
					auto fuzz = random_double(0.0, 0.5);
					sphere_material = make_shared<Metal>(albedo, fuzz);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else
				{
					//Glass
					sphere_material = make_shared<Dielectric>(1.5);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}

			}


		}
	}


	auto material1 = make_shared<Dielectric>(1.5);
	world.add(make_shared<Sphere>(Point3(0.0, 1.0, 0.0), 1.0, material1));

	auto material2 = make_shared<Lambertian>(make_shared<Solid_Color>(0.5,0.5,0.5));
	world.add(make_shared<Sphere>(Point3(-4.0, 1.0, 0.0), 1.0, material2));

	auto material3 = make_shared<Metal>(Color(0.7,0.6,0.5),0.0);
	world.add(make_shared<Sphere>(Point3(4.0, 1.0, 0.0), 1.0, material3));

	return world;
}
Hittable_List two_spheres()
{
	Hittable_List objects;
	
	auto checker = make_shared<Checker_Texture>(
		make_shared<Solid_Color>(0.2, 0.3, 0.2),
		make_shared<Solid_Color>(0.9, 0.9, 0.9));

	objects.add(make_shared<Sphere>(Point3(0, -10, 0), 10, make_shared<Lambertian>(checker)));
	objects.add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Lambertian>(checker)));


	return objects;
}
Hittable_List two_perlin_spheres()
{
	Hittable_List objects;

	auto pertext = make_shared<Noise_Texture>(12.0);
	objects.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
	objects.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

	return objects;
}
Hittable_List earth()
{
	auto earth_texture = make_shared<Image_Texture>("earthmap.jpg");
	auto earth_surface = make_shared<Lambertian>(earth_texture);
	auto globe = make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);

	return Hittable_List(globe);
}
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
	Point3 lookFrom(13.0, 2.0, 3.0);
	Point3 lookAt(0.0, 0.0, 0.0);
	Vec3 vUp(0.0, 1.0, 0.0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.0;
	Camera cam(lookFrom, lookAt, vUp, 20, aspect_ratio, aperture, dist_to_focus);

	Hittable_List world = earth();

	
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