#ifndef PLANE_H
#define PLANE_H

#include "objs.h"
#include "vec3.h"
#include "ray.h"

using std::sqrt;

class plane : public objs {
    public: 
        /** 
         * Constructor for a Plane
         * @param point any point that appears on the plane
         * @param normal the surface normal for the plane
         * @param kDiffuse the kDiffuse element for the Phong shading model
         */
        plane(const point3& point, const vec3& normal, const color& kDiffuse, material* mat) : a(point), n(normal), kD(kDiffuse), m(mat) {}
        
        point3 point() const {
            return a;
        }

        color kDiffuse() const {
            return kD;
        }

        material* mat() const {
            return m;
        }

        std::string type() const {
            return "plane";
        }

        virtual vec3 surface_normal(const point3 position) const;
        virtual bool ray_intersection(const ray& r, hit_record& rec, double tmin, double tmax) const;
        virtual aabb bounding_box() const;

    public:
        point3 a;
        vec3 n;
        color kD;
        material* m;
};

vec3 plane::surface_normal(const point3 position) const {
    return unit_vector(n);
}

bool plane::ray_intersection(const ray& r, hit_record& rec, double tmin, double tmax) const {
    double t = dot((a - r.origin()), unit_vector(n)) / dot(r.direction(), unit_vector(n));

    rec.t = t;
    rec.p = r.at(t);
    rec.set_normal(r, unit_vector(n));
    rec.kD = kD;
    rec.mat = m;
    return (t >= 0.0);
}

aabb plane::bounding_box() const {
    return aabb();
}

#endif