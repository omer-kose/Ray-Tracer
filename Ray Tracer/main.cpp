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
#include "Diffuse_Light.h"
#include "Aarect.h"
#include "Box.h"


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
Hittable_List simple_light()
{
	Hittable_List objects;

	auto pertext = make_shared<Noise_Texture>(4);
	objects.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
	objects.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

	auto diffuse_light = make_shared<Diffuse_Light>(make_shared<Solid_Color>(4, 4, 4));
	objects.add(make_shared<Sphere>(Point3(0, 7, 0), 2, diffuse_light));
	objects.add(make_shared<XY_Rect>(3, 5, 1, 3, -2, diffuse_light));

	return objects;
}
Hittable_List cornell_box() {
	Hittable_List objects;

	auto red = make_shared<Lambertian>(Color(.65, .05, .05));
	auto white = make_shared<Lambertian>(Color(.73, .73, .73));
	auto green = make_shared<Lambertian>(Color(.12, .45, .15));
	auto light = make_shared<Diffuse_Light>(Color(15, 15, 15));

	objects.add(make_shared<YZ_Rect>(0, 555, 0, 555, 555, green));
	objects.add(make_shared<YZ_Rect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<XZ_Rect>(213, 343, 227, 332, 554, light));
	objects.add(make_shared<XZ_Rect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<XZ_Rect>(0, 555, 0, 555, 555, white));
	objects.add(make_shared<XY_Rect>(0, 555, 0, 555, 555, white));

	objects.add(make_shared<Box>(Point3(130, 0, 65), Point3(295, 165, 230), white));
	objects.add(make_shared<Box>(Point3(265, 0, 295), Point3(430, 330, 460), white));

	return objects;
}
Color ray_color(const Ray &ray, const Color &background,const Hittable_List &world, int depth)/*depth is the recursive depth*/
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
	if (!world.hit(ray, 0.001, infinity, rec))
	{
		return background;
	}

	Ray scattered;
	Color attenuation;
	Color emmited = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

	if (!rec.mat_ptr->scatter(ray, rec, attenuation, scattered))
	{
		return emmited;
	}

	return emmited + attenuation * ray_color(scattered, background, world, depth - 1);
}

int main()
{
	const auto aspect_ratio = 1.0 / 1.0;
	const int image_width = 600;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	Hittable_List world;

	int samples_per_pixel = 100;
	int max_depth = 50;

	Point3 lookfrom;
	Point3 lookat;
	Vec3 vup(0, 1, 0);
	auto vfov = 40.0;
	auto aperture = 0.0;
	auto dist_to_focus = 10.0;
	Color background(0, 0, 0);

	switch (6) {
	case 1:
		world = random_scene();
		lookfrom = Point3(13, 2, 3);
		lookat = Point3(0, 0, 0);
		vfov = 20.0;
		background = Color(0.70, 0.80, 1.00);
		break;

	case 2:
		world = two_spheres();
		lookfrom = Point3(13, 2, 3);
		lookat = Point3(0, 0, 0);
		vfov = 20.0;
		background = Color(0.70, 0.80, 1.00);
		break;

	case 3:
		world = two_perlin_spheres();
		lookfrom = Point3(13, 2, 3);
		lookat = Point3(0, 0, 0);
		vfov = 20.0;
		background = Color(0.70, 0.80, 1.00);
		break;

	case 4:
		world = earth();
		lookfrom = Point3(0, 0, 12);
		lookat = Point3(0, 0, 0);
		vfov = 20.0;
		background = Color(0.70, 0.80, 1.00);
		break;

	case 5:
		world = simple_light();
		lookfrom = Point3(26, 3, 6);
		lookat = Point3(0, 2, 0);
		vfov = 20.0;
		break;

	default:
	case 6:
		world = cornell_box();
		lookfrom = Point3(278, 278, -800);
		lookat = Point3(278, 278, 0);
		vfov = 40.0;
		break;

	}

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);
	
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
				pixel_color += ray_color(r, background, world, max_depth);
			}
			write_color(std::cout, pixel_color,samples_per_pixel);
		}
	}

	std::cerr << "\nDone.\n";
	return 0;
}