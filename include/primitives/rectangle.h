#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "objs.h"
#include "vec3.h"
#include "ray.h"
#include "aabb.h"
#include "material.h"
#include "triangle.h"

class rectangle : public objs {
    public: 
        /** 
         * Constructor for a Triangle
         * @param a_t, b_t, c_t: the three edge points of the triangle
         * @param kDiffuse the kDiffuse element for the Phong shading model
         */
        rectangle(const vec3& a, const vec3& b, const vec3& c, const vec3& d, const color& kDiffuse, material* mat) : kD(kDiffuse), m(mat) {
            t1 = new triangle(a, b, c, kDiffuse, mat);
            t2 = new triangle(a, c, d, kDiffuse, mat);
            bbox = create_aabb();
        }
        
        color kDiffuse() const {
            return kD;
        }

        material* mat() const {
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
        bool ray_intersection(const ray& r, hit_record& rec, double tmin, double tmax) const;
        aabb create_aabb() const;

    public:
        vec3 a,b,c,d;
        triangle* t1;
        triangle* t2;
        color kD;
        aabb bbox;
        material* m;
};

vec3 rectangle::surface_normal(const point3 position) const {
    return t1->surface_normal(position);
}

bool rectangle::ray_intersection(const ray& r, hit_record& rec, double tmin, double tmax) const {
    bool t1_intersect = t1->ray_intersection(r, rec, tmin, tmax);
    bool t2_intersect = t2->ray_intersection(r, rec, tmin, tmax);
    return t1_intersect || t2_intersect;
}

aabb rectangle::create_aabb() const {
    return surrounding_box(t1->bounding_box(), t2->bounding_box());
}

inline ostream& operator<<(ostream &out, const rectangle& t) {
    return out << t.type() << ": " << t.kDiffuse();
}

#endif