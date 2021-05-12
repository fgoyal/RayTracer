/**
 * @file RGBAPixel.h
 * @author Ian Rudnick
 * Class for pixel data using the RGBA colorspace.
 */
#ifndef RUDNICKRT_RGBAPIXEL_H
#define RUDNICKRT_RGBAPIXEL_H

#include <iostream>
#include <sstream>
#include "vec3.h"

namespace rudnick_rt {
class RGBAPixel {
public:
    double r;   // Double for the red channel of the pixel, [0, 255].
    double g;   // Double for the green channel of the pixel, [0, 255].
    double b;   // Double for the blue channel of the pixel, [0, 255].
    double a;   // Double for the alpha of the pixel, [0, 255].

    /**
     * Constructs a default RGBAPixel.
     * A default pixel is completely opaque and white. The alpha component is
     * set to 255.0, and the color channels are all 255.
     */
    RGBAPixel();

    /**
     * Constructs an opaque RGBAPixel with the given RGB values.
     * The alpha component is set to 255.0.
     * @param red Red value [0, 255]
     * @param green Green value [0, 255]
     * @param blue Blue value [0, 255]
     */
    RGBAPixel(double red, double green, double blue);

    /**
     * Constructs an RGBAPixel with the given RGBA values.
     * @param red Red value [0, 255]
     * @param green Green value [0, 255]
     * @param blue Blue value [0, 255]
     * @param alpha Alpha value [0, 255]
     */
    RGBAPixel(double red, double green, double blue, double alpha);

    /**
     * Sets the color of a pixel in the PNG.
     */
    void setColor(double red, double green, double blue);

    bool operator== (RGBAPixel const & other) const;
    bool operator!= (RGBAPixel const & other) const;

}; // class RGBAPixel

/**
 * Stream operator to allow pixels to be written to standard streams.
 * @param out Stream to write to.
 * @param pixel Pixel to write to the stream.
 */
std::ostream & operator<<(std::ostream & out, RGBAPixel const & pixel);
std::stringstream & operator<<(std::stringstream & out, RGBAPixel const & pixel);

} // namespace rudnick_rt

#endif
