#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <vector>
#include <iostream>

/**
 * Write the translated [0, 255] value of each color component.
 * @param out the stream to output the color information to
 * @param pixel_color the vector containing the color information. Each channel ranges from [0, 1]
 **/
void write_color(std::ostream &out, color pixel_color) {
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

/**
 * Darkens the given color by the factor. The smaller the factor, the darker the result
 * @param pixel_color vec3 containing color information, with each channel in [0,1]
 * @param factor double between [0.0, 1.0]
 * @return the new color after it's been shaded
 **/
color shade(color pixel_color, double factor) {
    return pixel_color * factor;
}

/**
 * Finds the average color out of a list of colors
 * @param vect the list of colors
 * @return the average color
 **/
color get_average_color(std::vector<color> &vect) {
    color avg = color(0, 0, 0);
    for (auto c : vect) {
        avg += c;
    }
    return avg / vect.size();
}

#endif