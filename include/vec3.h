#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
// #include "utils.h"


using std::sqrt;
using std::ostream;

class vec3 {
    public: 
        vec3() : e{0, 0, 0} {}
        vec3(float e0, float e1, float e2) : e {e0, e1, e2} {}

        float x() const {
            return e[0];
        }

        float y() const {
            return e[1];
        }

        float z() const {
            return e[2];
        }

        vec3 operator-() const {
            return vec3(-e[0], -e[1], -e[2]);
        }

        float operator[](int i) const {
            return e[i];
        }

        float& operator[](int i) {
            return e[i];
        }

        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*=(const float t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(const float t) {
            return *this *= 1/t;
        }

        float length() const {
            return sqrt(length_squared());
        }

        float length_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

        bool near_zero() const {
            const auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        }

    public:
        float e[3];

};

// Type aliases for vec3
using point3 = vec3;
using color = vec3;


// vec3 Utility Functions

inline ostream& operator<<(ostream &out, const vec3 &v) {
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(float t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, float t) {
    return t * v;
}

inline vec3 operator/(vec3 v, float t) {
    return (1/t) * v;
}

inline float dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
} 

inline vec3 reflect(vec3 V, vec3 N) {
    return V - 2 * dot(V, N) * N;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

inline double clip(double n, double lower, double upper) {
    return std::max(lower, std::min(n, upper));
}

inline double clip_min(double n, double lower) {
    return std::max(lower, n);
}

inline vec3 vec_clamp(vec3 v, double min, double max) {
    return vec3(clip(v.e[0], min, max),
                clip(v.e[1], min, max),
                clip(v.e[2], min, max));
}

inline vec3 vec_clamp_min(vec3 v, double min) {
    return vec3(clip_min(v.e[0], min),
                clip_min(v.e[1], min),
                clip_min(v.e[2], min));
}

inline vec3 vec_sqrt(vec3 v) {
    return vec3(sqrt(v.e[0]), sqrt(v.e[1]), sqrt(v.e[2]));
}

#endif