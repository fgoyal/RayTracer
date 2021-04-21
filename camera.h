#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"
#include <iostream>

class camera {
    public: 
        camera(point3 eye, vec3 view, vec3 up, double d, int image_width, int image_height, double s) {
            eyepoint = eye;
            dir = d;

            // calculate orthonormal basis
            w = unit_vector(eyepoint - view);
            u = unit_vector(cross(up, w));
            v = cross(w, u);
        }

        ray get_ray(vec3 coordinate) const;

    private:
        point3 eyepoint;
        double dir;
        vec3 w;
        vec3 u;
        vec3 v;
};

/**
 * Calculates a ray through the coordinate in world space coordinates
 * @param coordinate coordinate in local camera space
 * @return a ray that can intersect objects in the world space system
 */
ray camera::get_ray(vec3 coordinate) const {
    vec3 pv = coordinate - eyepoint - vec3(0.0, 0.0, dir);
    vec3 pw = u * pv.x() + v * pv.y() + w * pv.z();
    return ray(eyepoint, pw);
}

#endif