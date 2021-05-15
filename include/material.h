#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>

#include "ray.h"
#include "texture.h"
#include "utils.h"
#include "vec3.h"
#include "hittables/hittable.h"

using std::shared_ptr;
using std::make_shared;

/**
 * Abstract class for materials.
 */
class material {
    public:
        virtual ~material() = default;

        /**
         * Produces a scattered ray
         * @param r: the ray being shot at the material
         * @param rec: hit record storing the intersection results
         * @param scattered: holds the calculated scattered ray
         * @return true if ray is scattered, false otherwise
         */
        virtual bool scatter(
            const ray& r, const hit_record& rec, ray& scattered, color& attenuation
        ) const = 0;

        /**
         * determines what color light the material emits
         * @return the color emitted
         */
        virtual color emitted() const {
            return color(0, 0, 0);
        }
};

/**
 * Class for diffuse/lambertian objects
 */
class lambertian : public material {
    public:
        lambertian(const color& c)
        : texture_(make_shared<solid_color_texture>(c)) {}

        lambertian(shared_ptr<texture> t)
        : texture_(t) {}

        virtual bool scatter(const ray& r, const hit_record& rec, ray& scattered, color& attenuation) const override {
            vec3 scatter_direction = rec.normal + random_unit_vector();
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }
            scattered = ray(rec.point, scatter_direction, r.time());
            attenuation = this->texture_->value(rec.u, rec.v, rec.point);
            return true;
        }

    public:
        shared_ptr<texture> texture_;
};

/**
 * Class for reflective materials.
 */
class mirror : public material {
    public:
        mirror(const color& c, double f)
        : texture_(make_shared<solid_color_texture>(c)), fuzz_(f<1 ? f : 1) {}

        mirror(shared_ptr<texture> t, double f)
        : texture_(t), fuzz_(f<1 ? f : 1) {}

        virtual bool scatter(const ray& r, const hit_record& rec, ray& scattered, color& attenuation) const override{
            vec3 reflected = reflect(r.direction(), rec.normal);
            scattered = ray(rec.point, reflected + fuzz_ * random_in_unit_sphere(), r.time());
            attenuation = this->texture_->value(rec.u, rec.v, rec.point);
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        shared_ptr<texture> texture_;
        double fuzz_;
};

/**
 * Class for transparent materials.
 */
class dielectric : public material {
    public: 
        dielectric(const color& mat_color, double index) : c(mat_color), ior(index) {}

        virtual bool scatter(const ray& r, const hit_record& rec, ray& scattered, color& attenuation) const override{
            double refraction_ratio;
            vec3 n = rec.normal;
            if (dot(r.direction(), rec.normal) < 0) {
                refraction_ratio = 1.0 / ior;
            } else {
                refraction_ratio = ior;
            }

            vec3 unit_direction = r.direction();
            double cos_theta = fmin(dot(-unit_direction, n), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cant_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;
            if (cant_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
                direction = reflect(unit_direction, n);
            } else {
                direction = refract(unit_direction, n, refraction_ratio);
            }
            scattered = ray(rec.point, direction, r.time());
            attenuation = this->c;
            return true;
        }

    public:
        color c;
        double ior;

    private:
        /**
         * Schlick's approximation for reflectance
         */
        static double reflectance(double cosine, double ref_idx) {
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
};

/**
 * Class for area lights aka objects that emit their own light
 */
class area_light : public material {
    public:
        area_light(const color& emit) : c(emit) {}
        virtual bool scatter(const ray& r, const hit_record& rec, ray& scattered, color& attenuation) const override{
            return false;
        }
        virtual color emitted() const override {
            return this->c;
        }

    public:
        color c;
};




#endif
