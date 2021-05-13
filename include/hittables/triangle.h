#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "aabb.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"
#include "hittables/hittable.h"


class triangle : public hittable {
    public: 
        /** 
         * Constructor for a Triangle
         * @param a_t, b_t, c_t: the three edge points of the triangle
         * @param kDiffuse the kDiffuse element for the Phong shading model
         */
        triangle(const vec3& a_t, const vec3& b_t, const vec3& c_t, shared_ptr<material> mat) : a(a_t), b(b_t), c(c_t), m(mat) {
            bbox = create_aabb();
        }
        
        vec3 a_t() const {
            return a;
        }

        vec3 b_t() const {
            return b;
        }

        vec3 c_t() const {
            return c;
        }

        shared_ptr<material> mat() const {
            return m;
        }

        aabb bounding_box() const {
            return bbox;
        }

        std::string type() const {
            return "triangle";
        }

        // virtual color kDiffuse() const;
        vec3 surface_normal(const point3 position) const;
        vec3 interpolated_normal(const point3 position) const;
        bool hit(const ray& r, hit_record& rec, double tmin, double tmax) const;
        aabb create_aabb() const;
        void set_vertex_normals(const vec3& a, const vec3& b, const vec3& c);
        vec3 barycentric_coordinates(const point3 position) const;

    public:
        point3 a;
        point3 b;
        point3 c;
        aabb bbox;
        vec3 normal_a;
        vec3 normal_b;
        vec3 normal_c;
        shared_ptr<material> m;
};

/**
 * Calculates the area of the triangle given three points
 * @return the area as a double
 */
inline double area(const point3 x, const point3 y, const point3 z) {
    vec3 e1 = y - x;
    vec3 e2 = z - x;
    return 0.5 * cross(e1, e2).length();
}

/** the position is not used, only there to match the function structure **/
vec3 triangle::surface_normal(const point3 position) const {
    vec3 e1 = b - a;
    vec3 e2 = c - a;
    return unit_vector(cross(e1, e2));
}

/**
 * Calculates the interpolated normal based on the barycentric coordinates of the point
 * @param position: the point to calculate barycentric coordinates for
 * @return a vec3 containing the interpolated normal
 */
vec3 triangle::interpolated_normal(const point3 position) const {
    vec3 bc = barycentric_coordinates(position);
    return normal_a * bc[0] + normal_b * bc[1] + normal_c * bc[2];
}

bool triangle::hit(const ray& r, hit_record& rec, double tmin, double tmax) const {
    vec3 e1 = b - a;
    vec3 e2 = c - a;
    vec3 q = cross(r.direction(), e2);

    double p = dot(e1, q);
    if (abs(p) < 0.000001) {
        return false;
    }

    double f = 1/p;
    vec3 s = r.origin() - a;
    double u = f * dot(s, q);

    if (u < 0.0) {
        return false;
    }

    vec3 x = cross(s, e1);
    double v = f * dot(r.direction(), x);
    if (v < 0.0 || (u + v) > 1.0) {
        return false;
    }
    double t = f * dot(e2, x);
    if (t < tmin || t > tmax) {
        return false;
    }
    rec.t = t;
    rec.point = r.at(t);
    // rec.set_normal(r, interpolated_normal(rec.p));
    rec.set_normal(r, surface_normal(rec.point));
    // rec.kD = kD;
    rec.mat = m;
    return true;
}

aabb triangle::create_aabb() const {
    double minx = fmin(fmin(a[0], b[0]), c[0]);
    double maxx = fmax(fmax(a[0], b[0]), c[0]);
    double miny = fmin(fmin(a[1], b[1]), c[1]);
    double maxy = fmax(fmax(a[1], b[1]), c[1]);
    double minz = fmin(fmin(a[2], b[2]), c[2]);
    double maxz = fmax(fmax(a[2], b[2]), c[2]);

    double epsilon = 0.0000001;
    if (minx == maxx) {
        minx -= epsilon;
        maxx += epsilon;
    }

    if (miny == maxy) {
        miny -= epsilon;
        maxy += epsilon;
    }

    if (minz == maxz) {
        minz -= epsilon;
        maxz += epsilon;
    }
    return aabb(vec3(minx, miny, minz), vec3(maxx, maxy, maxz));
}

/**
 * Setter for vertex normals
 */
void triangle::set_vertex_normals(const vec3& x, const vec3& y, const vec3& z) {
    normal_a = x;
    normal_b = y;
    normal_c = z;
}

/**
 * Calculate the barycentric coordinates at given point
 * @param position: the point to find barycentric coordinates for
 */
vec3 triangle::barycentric_coordinates(const point3 position) const {
    double T = area(a, b, c);
    double b1 = area(position, b, c) / T;
    double b2 = area(a, position, c) / T;
    double b3 = area(a, b, position) / T;
    return vec3(b1, b2, b3);
}

inline ostream& operator<<(ostream &out, const triangle& t) {
    return out << "triangle";
}

#endif