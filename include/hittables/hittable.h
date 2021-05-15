#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include <stdlib.h>
#include <vector>

#include "aabb.h"
#include "ray.h"
#include "vec3.h"

using std::shared_ptr;
using std::make_shared;

class material;


/**
 * Structure for storing data about ray-object intersections.
 */
struct hit_record {
    point3 point;
    vec3 normal;
    double t;
    double u, v;
    // color kD;
    shared_ptr<material> mat;

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


/**
 * Abstract class for hittable objects in the scene.
 */
class hittable {
    public:
        virtual ~hittable() = default;

        /**
         * Determines if there is any intersection between the object and the given ray.
         * @param r the ray that intersects with the object
         * @param rec if the ray intersects, this stores information about how it hit
         * @return true or false depending on if it intersects
         **/
        virtual bool hit(const ray& r, hit_record& rec, double tmin, double tmax) const = 0;
        
        /**
         * Calculates the outward surface normal at the given point on the object
         * @param position the point to find the normal at
         * @return outward facing surface normal
         **/
        virtual vec3 surface_normal(const point3 position) const = 0;

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