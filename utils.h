#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <random>
#include "vec3.h"

/**
 * Generates a random integer between min and max
 * @return the random integer
 **/
inline int random_int(int min, int max) {
    return rand() % max + min;
}

/**
 * Generates a random double between 0 and 1
 * @return the random double
 **/
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

/**
 * Generates a random double between min and max
 * @return the random double
 **/
inline double random_double(double min, double max) {
    return min + (max-min) * random_double();
}

/**
 * Generates a random vec3 where all numbers are between max and min
 * @return the random vec3
 **/
inline vec3 random_vec3(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

/**
 * Generates a random sphere position, where x is [-1.9, 1.9], y is [-1.9, 1.9], and z is [-1, -0.1]
 * @return the random position
 **/
inline vec3 random_sphere() {
    return vec3(random_double(-1.9, 1.9), random_double(-1.9, 1.9), random_double(-1, -0.1));
}

/**
 * Generates a random point within the unit sphere
 */
inline vec3 random_in_unit_sphere() {
    while (true) {
        vec3 p = random_vec3(-1,1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

/**
 * Generates the unit vetor of a random point within the unit sphere
 */
inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

#endif