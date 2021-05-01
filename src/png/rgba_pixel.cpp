/**
 * @file RGBAPixel.cpp
 * @author Ian Rudnick
 * Implementation of the RGBAPixel class for pixel data using the RGBA
 * colorspace. For use in my PNG class with the lodepng library.
 */

#include "png/rgba_pixel.h"

#include <cmath>
#include <iostream>
#include "utils.h"

namespace rudnick_rt {

RGBAPixel::RGBAPixel() {
    r = 255.0;
    g = 255.0;
    b = 255.0;
    a = 255.0;
}

RGBAPixel::RGBAPixel(double red, double green, double blue) {
    r = red;
    g = green;
    b = blue;
    a = 255.0;
}

RGBAPixel::RGBAPixel(double red, double green, double blue, double alpha) {
    r = red;
    g = green;
    b = blue;
    a = alpha;
}

void RGBAPixel::setColor(double red, double green, double blue) {
    // this->r = 255.0 * clamp(std::sqrt(red), 0.0, 1.0);
    // this->g = 255.0 * clamp(std::sqrt(green), 0.0, 1.0);
    // this->b = 255.0 * clamp(std::sqrt(blue), 0.0, 1.0);
    this->r = 255.0 * clamp(red, 0.0, 1.0);
    this->g = 255.0 * clamp(green, 0.0, 1.0);
    this->b = 255.0 * clamp(blue, 0.0, 1.0);
    this->a = 255.0;
}


bool RGBAPixel::operator== (RGBAPixel const & other) const {
    if (fabs(a - other.a) > 0.0000001) return false;
    if (a == 0) return true;

    if (fabs(r - other.r) > 0.0000001) return false;
    if (fabs(g - other.g) > 0.0000001) return false;
    if (fabs(b - other.b) > 0.0000001) return false;
    return true;
}

bool RGBAPixel::operator!= (RGBAPixel const & other) const {
    return !(*this == other);
}

std::ostream & operator<<(std::ostream & out, RGBAPixel const & pixel) {
    out << "(" << pixel.r << ", " 
               << pixel.g << ", " 
               << pixel.b 
               << (pixel.a != 255.0 ? ", " + std::to_string(pixel.a) : "") << ")";
    
    return out;
}

} // namespace rudnick_rt
