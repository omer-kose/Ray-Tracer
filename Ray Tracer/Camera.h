#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Utility.h"

class Camera
{
public:
	Camera()
	{
		/*We are casting our rays relative to lower_left_corner*/
		lower_left_corner = Point3(-2.0, -1.0, -1.0);
		/*Image plane dimensions*/
		horizontal = Vec3(4.0, 0.0, 0.0);
		vertical = Vec3(0.0, 2.0, 0.0);
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