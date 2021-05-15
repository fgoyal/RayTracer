/**
 * @file main.cpp
 * @authors Fiza Goyal, Jake Elling, Ian Rudnick
 *
 * Main rendering routine for the raytracer.
 *
 * Final project for CS 419, Production Computer Graphics, Spring 2021 at the
 * University of Illinois at Urbana Champaign.
 */
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <limits>

#include "aabb.h"

#include "camera.h"
#include "color.h"
#include "jitter.h"
#include "material.h"
#include "mesh.h"
#include "ray.h"
#include "scene_presets.h"
#include "utils.h"
#include "vec3.h"

#include "hittables/hittable.h"
#include "hittables/bvh_node.h"
#include "hittables/plane.h"
#include "hittables/rectangle.h"
#include "hittables/sphere.h"
#include "hittables/triangle.h"
#include "hittables/moving_sphere.h"

#include "png/png.h"
#include "png/rgba_pixel.h"

using std::cout;
using std::cerr;
using std::string;
using std::vector;
using std::numeric_limits;
using std::shared_ptr;
using std::make_shared;

using rudnick_rt::PNG;


// --------------------------------------- VARIABLES --------------------------------------- //
static bool perspective = true;
static bool multisampling = true;
static const int fine_grid = 128;
static int coarse_grid = (int) std::sqrt(fine_grid);
const int max_depth = 50;
double infinity = numeric_limits<double>::infinity();

// Image
const static double aspect_ratio = 16.0 / 9.0;
const static int image_width = 200;
const static int image_height = static_cast<int>(image_width / aspect_ratio);

// Camera
const float viewport_width = 4.0;
const float s = viewport_width / image_width;
const vec3 direction = vec3(0, 0, -1);

const point3 eye_point = point3(0, 0, 0);
//const point3 eye_point(10, 3, 5);
const point3 look_at_point(0, 0, -1);
const vec3 up = vec3(0, 1, 0);
double dir = 3.5;

const camera cam = camera(eye_point, look_at_point, up, dir, image_width, image_height, s, 0.0, 1.0);

static color background(0.8, 0.9, 0.99);

// Objects
const int NUM_OBJECTS = 10;
const double sphere_radius = 0.5;
vector<shared_ptr<hittable>> objects;
bvh_node scene;

// Phong shading parameters
const vec3 lightPosition = vec3(0.75, 0.75, 0.5);
const vec3 kAmbient = vec3(1, 1, 1);
const vec3 iAmbient = vec3(0,0,0);
const vec3 iDiffuse = vec3(1,1,1);
const vec3 kSpecular = vec3(1,1,1);
const vec3 iSpecular = vec3(1,1,1);
const float shininess = 20;

// --------------------------------------- FUNCTIONS --------------------------------------- //

/**
 * Use gamma correction to fix the intensities of the color
 * @param c: the color to gamma correct
 * @return the corrected color
 */
color gamma_correction(color& c) {
    double ratio = 1.0 / fine_grid;
    color output = c * ratio;
    output = vec_sqrt(output);
    output = vec_clamp(output, 0.0, 1.0);
    return output;
}

/**
 * The color at the position as determined by the Phong reflection model's diffuse shading
 * @param N surface normal
 * @param position the point we are trying to shade
 * @param kDiffuse base color for the object
 * @return shaded color
 */
color phong_reflection(vec3 N, point3 position, vec3 kDiffuse) {
    vec3 L = unit_vector(lightPosition - position); // light vector
    vec3 V = unit_vector(eye_point - position);
    vec3 R = unit_vector(reflect(L, N));
    double diffuseLight = fmax(dot(L, N), 0.0);
    double specularLight = fmax(pow(dot(R, V), shininess), 0.0);

    vec3 ambient = kAmbient * iAmbient;
    vec3 diffuse = kDiffuse * diffuseLight * iDiffuse;
    vec3 specular = kSpecular * specularLight * iSpecular;
    color c = ambient + diffuse + specular;
    return vec_clamp(c, 0.0, 1.0);
}

/**
 * Generates a shadow ray for all hit points and determines if it will be in shadow or not.
 * The more objects covering it, the darker the shadow will be.
 * @param original the base color for the pixel
 * @param rec stores the hit point and its normal
 * @return the color based on if its in shadow
 */
color apply_shadows(color original, hit_record rec) {
    double epsilon = 0.0001;
    ray shadow_ray_before = ray(rec.point, lightPosition - rec.point);
    vec3 new_origin = shadow_ray_before.origin() + epsilon * shadow_ray_before.direction();
    ray shadow_ray = ray(new_origin, lightPosition - rec.point);
    hit_record tmp;
    color shadow = original;
    int i = 0;
    bool hit = scene.hit(shadow_ray, tmp, 0.001, infinity);
    if (hit) {
        shadow = shade(shadow, 0.4);
    }
    return shadow;
}

/**
 * For each ray, determine what object is the closest and return the shaded color accordingly
 * @param r: the ray to shoot at all objects
 * @return the final color at the point after shading and shadows
 */
color ray_color(const ray& r, int depth) {
    if (depth <= 0) {
        return color(0.0, 0.0, 0.0);
    }

    hit_record rec;
    bool hit = scene.hit(r, rec, 0.001, infinity);

    color output;
    if (hit) {
        ray scattered;
        color attenuation;
        color emitted = rec.mat->emitted();

        if (rec.mat->scatter(r, rec, scattered, attenuation)) {
            output = emitted + attenuation * ray_color(scattered, depth - 1);
        }
        else {
            output = emitted;
        }

        return output;
    }

    return background;
}

/**
 * Calculates the center coordinate for the given pixel
 * @param i, j: the pixel coordinates in the image
 * @return the pixel center in the view plane
 */
vec3 get_pixel_center(int i, int j) {
    double x = s * (i - image_width / 2 + 0.5);
    double y = s * (j - image_height / 2 + 0.5);
    return vec3(x, y, 0);
}

/**
 * Calculates the sample coordinate within a single pixel
 * @param (i, j): the pixel coordinates in the image
 * @param (k, l): the sample position within the grid
 * @return a coordinate within the pixel in the view plane
 */
vec3 get_grid_pixel_center(int i, int j, int k, int l) {
    double delta_x = (k + 0.5) / fine_grid;
    double delta_y = (l + 0.5) / fine_grid;
    double x = s * (i - image_width / 2 + delta_x);
    double y = s * (j - image_height / 2 + delta_y);
    return vec3(x, y, 0);
}

/**
 * Shoots a single ray at the given point based on either perspective or orthographic projections
 * @param pixel_center the point of the pixel we are shooting through
 * @return the ray color based on the objects it hits
 */
color shoot_one_ray(vec3& pixel_center) {
    ray r;
    if (perspective) {
        r = cam.get_ray(pixel_center);
    } else {
        r = ray(pixel_center, direction);
    }
    return ray_color(r, max_depth);
}

/**
 * Shoots multiple rays per pixel, using multi-jittered sampling
 * @param i, j: the pixel coordinates in the image
 * @return the average color for the pixel based of the different rays
 */
color shoot_multiple_rays(int i, int j) {
    bool** multi_jitter_mask = get_multi_jitter_mask(fine_grid);
    vector<color> colors;

    for (int k = 0; k < fine_grid; k++) {
        for (int l = 0; l < fine_grid; l++) {
            if (multi_jitter_mask[k][l]) {
                vec3 grid_center = get_grid_pixel_center(i, j, k, l);
                color pixel_color = shoot_one_ray(grid_center);
                colors.push_back(pixel_color);
            }
        }
    }

    return get_average_color(colors);
}


/**
 * Checks command line arguments for "p" and "j" to set perspective projection and multisampling respectively
 */
void set_command_line_args(int argc, char* argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (!string(argv[i]).compare("p")) {
                perspective = true;
            }

            if (!string(argv[i]).compare("j")) {
                multisampling = true;
            }
        }
    }
}

/**
 * The main rendering program.
 * Takes in optional command line arguments.
 */
int main(int argc, char* argv[]) {
    // Get a name for the output image file.
    std::string image_name;
    std::cout << "Enter a name for the new image: ";
    std::cin >> image_name;
    std::cout << std::endl;

    // Start a timer to time the rendering process.
    std::clock_t start;
    double duration;
    start = std::clock();

    // Initialize RNG
    srand(time(NULL));
    set_command_line_args(argc, argv);

    // Set up the scene.
    scene = three_spheres();

    // Print performance info
    cout << "Image dimensions: " << image_width << "x" << image_height << "\n";
    cout << "Number of primitives: " << objects.size() << "\n";

    // create_mesh();
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    cout << "Time to construct BVH tree: " << duration << " seconds\n";

    // Simple data structure to store pixel data for the PNG output
    PNG* image = new PNG(image_width, image_height);

    // Main rendering loop
    for (int j = 0; j < image_height; ++j) {
        cout << "\rScanlines remaining: " << image_height-j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0.0, 0.0, 0.0);
            if (multisampling) {
                pixel_color = shoot_multiple_rays(i, j);
            }
            else {
                vec3 pixel_center = get_pixel_center(i, j);
                pixel_color = shoot_one_ray(pixel_center);
            }

            // PNG image format is upside-down, so (0,0) is the top-left corner
            // so we have to give image_height-1-j as the y-coordinate.
            image->setPixel(i, image_height-1-j,
                            pixel_color.x(),
                            pixel_color.y(),
                            pixel_color.z());
        }
    }
    cout << "\n\n";

    // Encode the PNG data into the final image file.
    image->writeToFile("renders/" + image_name + ".png");
    delete image;
    cout << "Image saved as renders/" << image_name << ".png\n";

    // Display the total rendering time.
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    cout << "Total rendering time: " << duration << "\n";

    cout << "\nDone!\n";
    return 0;
}
