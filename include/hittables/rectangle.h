#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "aabb.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"
#include "hittables/hittable.h"
#include "hittables/triangle.h"

class rectangle : public hittable {
    public: 
        /** 
         * Constructor for a Triangle
         * @param a_t, b_t, c_t: the three edge points of the triangle
         * @param kDiffuse the kDiffuse element for the Phong shading model
         */
        rectangle(const vec3& a, const vec3& b, const vec3& c, const vec3& d, shared_ptr<material> mat) : m(mat) {
            t1 = make_shared<triangle>(a, b, c, mat);
            t2 = make_shared<triangle>(a, c, d, mat);
            bbox = create_aabb();
        }

        shared_ptr<material> mat() const {
            return m;
        }

        aabb bounding_box() const {
            return bbox;
        }

        std::string type() const {
            return "rectangle";
        }

        // virtual color kDiffuse() const;
        vec3 surface_normal(const point3 position) const;
        bool hit(const ray& r, hit_record& rec, double tmin, double tmax) const;
        aabb create_aabb() const;

    public:
        vec3 a, b, c, d;
        shared_ptr<triangle> t1, t2;
        aabb bbox;
        shared_ptr<material> m;
};

vec3 rectangle::surface_normal(const point3 position) const {
    return t1->surface_normal(position);
}

bool rectangle::hit(const ray& r, hit_record& rec, double tmin, double tmax) const {
    bool t1_intersect = t1->hit(r, rec, tmin, tmax);
    bool t2_intersect = t2->hit(r, rec, tmin, tmax);
    return t1_intersect || t2_intersect;
}

aabb rectangle::create_aabb() const {
    return surrounding_box(t1->bounding_box(), t2->bounding_box());
}

inline ostream& operator<<(ostream &out, const rectangle& t) {
    return out << t.type();
}

#endif