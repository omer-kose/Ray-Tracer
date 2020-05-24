#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Utility.h"

class Camera
{
public:
	Camera
	(
		Point3 lookFrom,/*Camera origin in world coordinates*/
		Point3 lookAt,/*In world coordinates*/
		Vec3 vup,//This will be used to define an orthonormal basis for camera.(This is also in world coord)
		double vfov, //vfov: vertical field-of-view in degrees
		double aspect_ratio,
		double aperture,
		double focus_dist
	)
	{
		/*Viewport Definition*/
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);/*directly tan(theta/2) since we let rays head to z = -1 plane*/
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		/*Orthonormal Basis Definition*/
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(vup,w));
		v = cross(w,u);


		/*Image Plane Definiton (Oriented with respect to Camera basis)*/
		origin = lookFrom;
		/*Horizontal and Vertical are both used as dimensions and vectors to traverse image plane with scalars range from 0 to 1 (getRay function)*/
		/*By adjusting focus distance we are now able to replace image plane in z direction*/
		/*While distancing we also have to readjust horizontal and vertical dimensions wrt viewport dimensions*/
		horizontal = focus_dist * viewport_width * u;/*Points right from left to right along image plane*/
		vertical = focus_dist * viewport_height * v;/*Points up from bottom to up along image plane*/
		lower_left_corner = origin - (horizontal / 2) - (vertical / 2) - focus_dist * w;

		lens_radius = aperture / 2;
	} 
	/*S and T are parameter which range from 0.0 to 1.0 depending on (pixel/dimension)*/
	/*s = colIndex/width*/
	/*t = rowIndex/height*/
	Ray getRay(double s, double t) const
	{
		Vec3 randomDirection = random_in_unit_disk() * lens_radius;
		Vec3 offset = u * randomDirection.x() + v * randomDirection.y();

		/*Since origin is not (0,0,0) anymore to get true direction we have to subtract origin(cameraOrigin)*/
		/*Like any other direction determination calculations*/
		return Ray(origin + offset, 
			lower_left_corner + s * horizontal + t * vertical - origin - offset);
	}

private:
	Point3 origin;
	Point3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	double lens_radius;
};

#endif