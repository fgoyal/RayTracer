#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "utils.h"
#include "hittables/hittable.h"
#include "vec3.h"
#include "ray.h"
#include "aabb.h"
#include "material.h"

class moving_sphere : public hittable {
public:
    moving_sphere() {}
    moving_sphere(
        point3 cen0, point3 cen1, double _time0, double _time1, double radius, shared_ptr<material> mat)
        : center0(cen0), center1(cen1), time0(_time0), time1(_time1), rad(radius), m(mat) {
            bbox = create_aabb();
        }

    point3 center(double time) const;

    double radius() const {
        return rad;
    }

    shared_ptr<material> mat() const {
        return m;
    }

    aabb bounding_box() const override {
        return bbox;
    }

    std::string type() const override {
        return "moving sphere";
    }

    virtual vec3 surface_normal(const point3 position) const override;
    virtual bool hit(const ray& r, hit_record& rec, double tmin, double tmax) const override;
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
    point3 center0;
    point3 center1;
    double time0;
    double time1;
    double rad;
    aabb bbox;
    shared_ptr<material> m;
};

vec3 moving_sphere::surface_normal(const point3 position) const {
    return unit_vector(position - center0);
}
point3 moving_sphere::center(double time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, hit_record& rec, double tmin, double tmax) const {
    vec3 oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - rad*rad;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    if (root < tmin || tmax < root) {
        root = (-half_b + sqrtd) / a;
        if (root < tmin || tmax < root) {
            return false;
        }
    }

    rec.t = root;
    rec.point = r.at(root);
    auto outward_normal = (rec.point - center(r.time())) / rad;
    rec.set_normal(r, outward_normal);
    this->compute_uv(rec.normal, rec.u, rec.v);
    rec.mat = m;

    return true;
}

aabb moving_sphere::create_aabb() const {
    aabb box0(
        center(time0) - vec3(rad, rad, rad),
        center(time0) + vec3(rad, rad, rad));
    aabb box1(
        center(time1) - vec3(rad, rad, rad),
        center(time1) + vec3(rad, rad, rad));

    return surrounding_box(box0, box1);
}

inline ostream& operator<<(ostream &out, const moving_sphere& s) {
    return out << s.type() << ": " << s.center0;
}

#endif
