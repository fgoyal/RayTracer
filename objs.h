#ifndef OBJS_H
#define OBJS_H
#include "ray.h"
#include "vec3.h"
#include "aabb.h"

#include <vector>
#include <stdlib.h>

class material;

/**
 * Stores the important information about a ray-object intersection.
 **/
struct hit_record {
    /** the point at which the intersection occurs */
    point3 p;

    /** the surface normal at the intersection facing away from the object */
    vec3 normal;

    /** the value that generates a point on the object and ray */
    double t;

    /** the kDiffuse component for the given object */
    color kD;

    /** the material of the given given */
    material* mat;

    /**
     * Determines if the normal faces away from the object and changes it if it doesn't
     * @param r the ray cast at the object
     * @param n the current surface normal
     **/
    inline void set_normal(const ray& r, const vec3& n) {
        bool front = dot(r.direction(), n) < 0;
        normal = front ? n : -n;
    }
};

/** Abstract class designed to hold all possible objects in the image */
class objs {
    public:
        /**
         * Determines if there is any intersection between the object and the given ray.
         * @param r the ray that intersects with the object
         * @param rec if the ray intersects, this stores information about how it hit
         * @return true or false depending on if it intersects
         **/
        virtual bool ray_intersection(const ray& r, hit_record& rec, double tmin, double tmax) const = 0;
        
        /**
         * Calculates the outward surface normal at the given point on the object
         * @param position the point to find the normal at
         * @return outward facing surface normal
         **/
        virtual vec3 surface_normal(const point3 position) const = 0;

        /**
         * Getter for the object's main color
         * @return the kDiffuse component for the object in the Phong shading model
         **/
        virtual color kDiffuse() const = 0;

        /**
         * Getter for the bounding box of the object. 
         * @return the bounding box
         **/
        virtual aabb bounding_box() const = 0;

        /**
         * @return a string saying the type of object it is
         */
        virtual std::string type() const = 0;
};

#endif