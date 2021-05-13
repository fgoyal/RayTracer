/**
 * @file scene_presets.h
 * @author Ian Rudnick
 * Functions to build preset scenes for the raytracer.
 * 
 * For CS 419 at the University of Illinois at Urbana-Champaign.
 */
#ifndef SCENE_PRESETS_H
#define SCENE_PRESETS_H

#include <cmath>
#include <iostream>
#include <memory>

#include "material.h"
#include "mesh.h"
#include "utils.h"
#include "vec3.h"

#include "hittables/hittable.h"
#include "hittables/hittable_list.h"
#include "hittables/bvh_node.h"
#include "hittables/rectangle.h"
#include "hittables/sphere.h"
#include "hittables/triangle.h"

using std::shared_ptr;
using std::make_shared;

//-----------------------------------------------------------------------------
/**
 * Creates the default scene created by Fiza.
 */
bvh_node default_scene() {
	hittable_list objects;

	// Color pallete
	// color purple      = color( 91,  75, 122) / 255.0;
	color orange      = color(219, 121,  59) / 255.0;
	color pink        = color(201,  81,  81) / 255.0;
	color blue        = color( 12, 173, 173) / 255.0;
	// color yellow      = color(255, 247, 163) / 255.0;
	// color sky         = color(173, 225, 255) / 255.0;
	// color black       = color(0.0, 0.0, 0.0);
	color dark_gray   = color(0.2, 0.2, 0.2);
	color light_gray  = color(0.9, 0.9, 0.9);
	color white       = color(1.0, 1.0, 1.0);

	// Add a triangle and three spheres to the scene.
	vec3 a_1 = vec3(-0.3, -0.6, -0.5); // front
    vec3 b_1 = vec3(-0.8, -0.6, -1); // back
    vec3 c_1 = vec3(-0.4, 0.2, -0.7); // top
	auto triangle_mat = make_shared<lambertian>(blue);
	objects.add(make_shared<triangle>(a_1, b_1, c_1, triangle_mat));

	auto s1_material = make_shared<mirror>(light_gray, 0.05);
	point3 s1_center(-0.2, -0.3, -1);
	auto s1_radius = 0.3;
	objects.add(make_shared<sphere>(s1_center, s1_radius, s1_material));

	auto s2_material = make_shared<glass>(white, 1.5);
	point3 s2_center(0.4, -0.3, -1);
	auto s2_radius = 0.2;
	objects.add(make_shared<sphere>(s2_center, s2_radius, s2_material));

	auto s3_material = make_shared<lambertian>(orange);
	point3 s3_center(0.8, -0.3, -1.5);
	auto s3_radius = 0.1;
	objects.add(make_shared<sphere>(s3_center, s3_radius, s3_material));

	auto s4_material = make_shared<lambertian>(pink);
	point3 s4_center(0.3, -0.43, -0.7);
	auto s4_radius = 0.07;
	objects.add(make_shared<sphere>(s4_center, s4_radius, s4_material));

	// Generate a triangle-checkerboard floor.
	hittable_list checkerboard;
	auto checker_mat_1 = make_shared<lambertian>(dark_gray);
	auto checker_mat_2 = make_shared<lambertian>(white);
	auto y = -0.5;
	point3 a, b, c, d;
    double width = 0.5;
    double length = 0.5;

    for (double z = 0; z > -10; z -= length) {
        for (double x = -10; x < 10; x += width) {
            a = point3(x, y, z - length);
            b = point3(x, y, z);
            c = point3(x + width, y, z);
            d = point3(x + width, y, z - length);
			checkerboard.add(make_shared<triangle>(a, b, c, checker_mat_2));
			checkerboard.add(make_shared<triangle>(a, c, d, checker_mat_1));
        }
    }

	objects.add(make_shared<bvh_node>(checkerboard));

	// Add area lights to the scene.
	auto light_mat = make_shared<area_light>(white);
	double light_x[3] = {-0.8, 0.2, 0.8};
    for (int i = 0; i < 3; i++) {
        a = point3(light_x[i], -0.35, -1.4);
        b = point3(light_x[i], -0.35, -0.6);
        c = point3(light_x[i], -0.6, -0.6);
        d = point3(light_x[i], -0.6, -1.4);
		objects.add(make_shared<rectangle>(a, b, c, d, light_mat));
    }

	point3 s_light_center(0, 0.5, -1);
	double s_light_radius = 0.25;
	objects.add(make_shared<sphere>(s_light_center, s_light_radius, light_mat));

	// Create a BVH tree of all the objects.
	return bvh_node(objects);
}


//-----------------------------------------------------------------------------
/**
 * Creates a scene of just the cow mesh.
 */
bvh_node cow_mesh() {
    color cow_color = color(1,0,0);
	auto cow_mat = make_shared<lambertian>(cow_color);
    mesh cow_mesh = mesh("objs/cow.obj", cow_mat);
    vector<shared_ptr<hittable>> cow_triangles = cow_mesh.get_faces();
    return bvh_node(cow_triangles);
}

#endif
