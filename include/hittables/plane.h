#ifndef PLANE_H
#define PLANE_H

#include "ray.h"
#include "vec3.h"
#include "hittables/hittable.h"

using std::sqrt;


class plane : public hittable {
    public: 
        /** 
         * Constructor for a Plane
         * @param point any point that appears on the plane
         * @param normal the surface normal for the plane
         * @param kDiffuse the kDiffuse element for the Phong shading model
         */
        plane(const point3& point, const vec3& normal, shared_ptr<material> mat)
            : a(point), n(normal), m(mat) {}
        
        point3 point() const {
            return a;
        }

        shared_ptr<material> mat() const {
            return m;
        }

        std::string type() const {
            return "plane";
        }

        virtual vec3 surface_normal(const point3 position) const;
        virtual bool hit(const ray& r, hit_record& rec, double tmin, double tmax) const;
        virtual aabb bounding_box() const;

    public:
        point3 a;
        vec3 n;
        shared_ptr<material> m;
};

vec3 plane::surface_normal(const point3 position) const {
    return unit_vector(n);
}

bool plane::hit(const ray& r, hit_record& rec, double tmin, double tmax) const {
    double t = dot((a - r.origin()), unit_vector(n)) / dot(r.direction(), unit_vector(n));

    rec.t = t;
    rec.point = r.at(t);
    rec.set_normal(r, unit_vector(n));
    rec.mat = m;
    return (t >= 0.0);
}

aabb plane::bounding_box() const {
    return aabb();
}

#endif