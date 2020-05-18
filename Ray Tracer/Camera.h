#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Utility.h"

class Camera
{
public:
	Camera(double vfov, double aspect_ratio)//vfov: vertical field-of-view in degrees
	{
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);/*directly tan(theta/2) since we let rays head to z = -1 plane*/
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		/*Offset of the image plane from the origin*/
		auto focal_length = 1.0;

		origin = Point3(0.0, 0.0, 0.0);
		/*Image plane dimensions*/
		horizontal = Vec3(viewport_width, 0.0, 0.0);
		vertical = Vec3(0.0, viewport_height, 0.0);
		/*We are casting our rays relative to lower_left_corner*/
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0.0, 0.0, focal_length);
	}
	/*U and V are parameter which range from 0.0 to 1.0 depending on (pixel/dimension)*/
	/*u = colIndex/width*/
	/*v = rowIndex/height*/
	Ray getRay(double u, double v) const
	{
		return Ray(origin, lower_left_corner + u * horizontal + v * vertical);
	}

private:
	Point3 origin;
	Point3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
};

#endif