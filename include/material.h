#ifndef MATERIAL_H
#define MATERIAL_H

struct hit_record; 

/**
 * Abstract class for material
 */
class material {
    public:
        /**
         * Produces a scattered ray
         * @param r: the ray being shot at the material
         * @param rec: hit record storing the intersection results
         * @param scattered: holds the calculated scattered ray
         * @return true if ray is scattered, false otherwise
         */
        virtual bool scatter(
            const ray& r, const hit_record& rec, ray& scattered
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
        virtual bool scatter(const ray& r, const hit_record& rec, ray& scattered) const override {
            vec3 scatter_direction = rec.normal + random_unit_vector();
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }
            scattered = ray(rec.p, scatter_direction);
            return true;
        }
};

/**
 * Class for objects that are reflective or mirror-like
 */
class mirror : public material {
    public: 
        mirror(double f) : fuzz(f < 1 ? f : 1) {}
        virtual bool scatter(const ray& r, const hit_record& rec, ray& scattered) const override{
            vec3 reflected = reflect(r.direction(), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            return (dot(scattered.direction(), rec.normal) > 0);
        }
    public:
        double fuzz;
};

/**
 * Class for objects that are transparent or glass-like
 */
class glass : public material {
    public: 
        glass(double index) : ior(index) {}
        virtual bool scatter(const ray& r, const hit_record& rec, ray& scattered) const override{
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
            scattered = ray(rec.p, direction);
            return true;
        }

    public:
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
        virtual bool scatter(const ray& r, const hit_record& rec, ray& scattered) const override{
            return false;
        }
        virtual color emitted() const override {
            return c;
        }
    public:
        color c;
};




#endif