#ifndef SPHERE_H
#define SPHERE_H

#include "hittables/hittable.h"
#include "vec3.h"
#include "ray.h"
#include "aabb.h"
#include "material.h"

#include <cmath>
#include <math.h>


class sphere : public hittable {
    public: 
        /** 
         * Constructor for a Sphere
         * @param center the center point of the sphere
         * @param radius the radius for the sphere
         * @param kDiffuse the kDiffuse element for the Phong shading model
         */
        sphere(const point3& center, const double radius, shared_ptr<material> mat)
        : c(center), rad(radius), m(mat) {
            bbox = create_aabb();
        }
        point3 center() const {
            return c;
        }

        double radius() const {
            return rad;
        }

        shared_ptr<material> mat() const {
            return m;
        }

        aabb bounding_box() const {
            return bbox;
        }

        std::string type() const {
            return "sphere";
        }

        virtual vec3 surface_normal(const point3 position) const;
        virtual bool hit(const ray& r, hit_record& rec, double tmin, double tmax) const;
        aabb create_aabb() const;

    private:
        /**
         * Computes uv coordinates at a point on the sphere.
         * @param p The point to compute the uv coordinates for.
         * @param u Variable to store the u-coordinate.
         * @param v Variable to store the v-coordinate.
         */
        static void compute_uv(const point3& p, double& u, double& v) {
            auto theta = acos(-p.y());
            auto phi = atan2(-p.z(), p.x()) + M_PI;

            u = phi / (2*M_PI);
            v = theta / M_PI;
        }

    public:
        point3 c;
        double rad;
        aabb bbox;
        shared_ptr<material> m;
};

vec3 sphere::surface_normal(const point3 position) const {
    return unit_vector(position - c);
}

bool sphere::hit(const ray& r, hit_record& rec, double tmin, double tmax) const {
    vec3 oc = r.origin() - c;
    double a = r.direction().length_squared();
    double half_b = dot(oc, r.direction());
    double c = oc.length_squared() - rad * rad;
    double discriminant = half_b * half_b - a * c;
    double root;
    if (discriminant < 0) {
        return false; // no intersection
    } 
    
    root = (-half_b - sqrt(discriminant)) / (a);
    if (root < tmin || root > tmax) {
        root = (-half_b + sqrt(discriminant)) / (a);
        if (root < tmin || root > tmax) {
            return false;
        }
    }

    rec.t = root;
    rec.point = r.at(root);
    rec.set_normal(r, surface_normal(rec.point));
    this->compute_uv(rec.normal, rec.u, rec.v);
    // rec.kD = kD;
    rec.mat = m;

    return true;
}

aabb sphere::create_aabb() const {
    return aabb(
        c - vec3(rad, rad, rad),
        c + vec3(rad, rad, rad)
    );
}

inline ostream& operator<<(ostream &out, const sphere& s) {
    return out << s.type() << ": " << s.center();
}

#endif