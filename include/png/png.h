/**
 * @file png.h
 * @author Ian Rudnick
 * PNG class based on the class used in CS 225: Data Structures.
 * Uses RGBAPixels and the lodepng PNG library.
 */

#ifndef RUDNICKRT_PNG_H
#define RUDNICKRT_PNG_H

#include <string>
#include "rgba_pixel.h"

namespace rudnick_rt {
class PNG {
public:

    /**
     * Constructs an empty PNG image object.
     */
    PNG();

    /**
     * Constructs a PNG of the specified dimensions.
     * @param width Image width.
     * @param height Image height.
     */
    PNG(unsigned int width, unsigned int height);

    /**
     * Copy constructor: constructs a new PNG as a copy of another.
     * @param other PNG to copy.
     */
    PNG(PNG const & other);

    /**
     * Destructor: frees all memory used by the PNG.
     */
    ~PNG();

    /**
     * Assignment operator: sets one PNG equal to another.
     * @param other PNG to copy.
     */
    PNG const & operator=(PNG const & other);

    /**
     * Equality operator: checks if two PNGs are the same.
     * @param other Image to compare to this one.
     * @return True if the images are equal.
     */
    bool operator==(PNG const & other) const;

    /**
     * Inquality operator: checks if two PNGs are different.
     * @param other Image to compare to this one.
     * @return True if the images are different.
     */
    bool operator!=(PNG const & other) const;

    /**
     * Writes a PNG image to a file.
     * @param filename Name of the file to be written.
     * @return True if the write was successful.
     */
    bool writeToFile(std::string const & filename);

    /**
     * Gets a reference to the pixel at the given coordinates in the image.
     * (0,0) is the upper-left corner.
     * @param x Pixel x-coordinate.
     * @param y Pixel y-coordinate.
     * @return A reference to the pixel at the given coordinates.
     */
    RGBAPixel & getPixel(unsigned int x, unsigned int y);

    /**
     * Gets a reference to the pixel at the given coordinates in the image.
     * (0,0) is the upper-left corner.
     * Const version
     * @param x Pixel x-coordinate.
     * @param y Pixel y-coordinate.
     * @return A reference to the pixel at the given coordinates.
     */
    const RGBAPixel & getPixel(unsigned int x, unsigned int y) const;

    /**
     * Sets the color values of a pixel in the image.
     * @param x Pixel x-coordinate
     * @param y Pixel y-coordinate
     * @param r Red value to give the pixel
     * @param g Green value to give the pixel
     * @param b Blue value to give the pixel
     */
    void setPixel(unsigned int x, unsigned int y, double r, double g, double b);

    /**
     * Gets the width of this image.
     * @return Width of this image.
     */
    unsigned int width() const;

    /**
     * Gets the height of this image.
     * @return Height of the image.
     */
    unsigned int height() const;

    /**
     * Resizes a PNG to the given dimensions.
     * @param newWidth Resized image width.
     * @param newHeight Resized image height.
     */
    void resize(unsigned int newWidth, unsigned int newHeight);

private:
    unsigned int width_;
    unsigned int height_;
    RGBAPixel *image_data_;

    /**
     * Copies the contents of other to self
     */
    void _copy(PNG const & other);

    /**
     * Gets a reference to the pixel at the given coordinates
     */
    RGBAPixel & _get_pixel(unsigned int x, unsigned int y) const;

}; // class PNG

std::ostream & operator<<(std::ostream & out, PNG const & pixel);
std::stringstream & operator<<(std::stringstream & out, PNG const & pixel);

} // namespace rudnick_rt

#endif // RUDNICKRT_PNG_H
