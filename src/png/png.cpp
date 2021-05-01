/**
 * @file PNG.c
 * @author Ian Rudnick
 * Implementation of a PNG class based on the class used in CS 225: Data 
 * Structures. Uses the lodepng PNG library. Modified to use RGBAPixels instead
 * of HSLAPixels.
 */
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "png/lodepng.h"
#include "png/png.h"
#include "png/rgba_pixel.h"

using std::cerr;
using std::endl;
using std::string;
using std::vector;


namespace rudnick_rt {

void PNG::_copy(PNG const & other) {
  delete[] image_data_;

  width_ = other.width_;
  height_ = other.height_;
  image_data_ = new RGBAPixel[width_ * height_];

  for (unsigned i = 0; i < width_ * height_; i++) {
    image_data_[i] = other.image_data_[i];
  }
}


PNG::PNG() {
  width_ = 0;
  height_ = 0;
  image_data_ = NULL;
}

PNG::PNG(unsigned int width, unsigned int height) {
  width_ = width;
  height_ = height;
  image_data_ = new RGBAPixel[width_ * height_];
}

PNG::PNG(PNG const & other) {
  image_data_ = NULL;
  _copy(other);
}

PNG::~PNG() {
  delete[] image_data_;
}

PNG const & PNG::operator=(PNG const & other) {
  if (this != &other) { _copy(other); }
  return *this;
}

bool PNG::operator== (PNG const & other) const {
  if (width_ != other.width_) { return false; }
  if (height_ != other.height_) { return false; }

  for (unsigned i = 0; i < width_ * height_; i++) {
    RGBAPixel & p1 = image_data_[i];
    RGBAPixel & p2 = other.image_data_[i];
    if (p1 != p2) { return false; }
  }

  return true;
}

bool PNG::operator!= (PNG const & other) const {
  return !(*this == other);
}

RGBAPixel & PNG::_get_pixel(unsigned int x, unsigned int y) const {
  if (width_ == 0 || height_ == 0) {
    cerr << "ERROR: Call to PNG::getPixel() made on an "
            "image with no pixels." << endl;
    assert(width_ > 0);
    assert(height_ > 0);
  }

  if (x >= width_) {
    cerr << "WARNING: Call to PNG::getPixel(" << x << "," << y << ") "
            "tries to access x out of bounds of width: " << width_ << endl;
    cerr << "       : Truncating x to " << (width_ - 1) << endl;
    x = width_ - 1;
  }
  if (y >= height_) {
    cerr << "WARNING: Call to PNG::getPixel(" << x << "," << y << ") "
            "tries to access y out of bounds of height: " << height_ << endl;
    cerr << "       : Truncating y to " << (height_ - 1) << endl;
    y = height_ - 1;
  }

  unsigned index = x + (y * width_);
  return image_data_[index];
}

RGBAPixel & PNG::getPixel(unsigned int x, unsigned int y) {
  return _get_pixel(x,y);
}

const RGBAPixel & PNG::getPixel(unsigned int x, unsigned int y) const {
  return _get_pixel(x,y);
}

void PNG::setPixel(unsigned int x, unsigned int y, double r, double g, double b) {
  RGBAPixel& pixel = this->getPixel(x, y);
  pixel.setColor(r, g, b);
}

bool PNG::writeToFile(const string & filename) {
  unsigned idx;
  unsigned error;
  unsigned char *byte_data = new unsigned char[width_ * height_ * 4];

  for (unsigned i = 0; i < width_ * height_; i++) {
    idx = i * 4;
    byte_data[idx]     = image_data_[i].r;
    byte_data[idx + 1] = image_data_[i].g;
    byte_data[idx + 2] = image_data_[i].b;
    byte_data[idx + 3] = image_data_[i].a;
  }

  error = lodepng::encode(filename, byte_data, width_, height_);
  if (error) {
    cerr << "PNG encoding error " << error << ": " << lodepng_error_text(error) << endl;
  }
  delete[] byte_data;
  return (error == 0);
}

unsigned int PNG::width() const {
  return width_;
}

unsigned int PNG::height() const {
  return height_;
}

void PNG::resize(unsigned int newWidth, unsigned int newHeight) {
  // Create a new vector to store the image data for the new (resized) image
  RGBAPixel * newImageData = new RGBAPixel[newWidth * newHeight];

  // Copy the current data to the new image data, using the existing pixel
  // for coordinates within the bounds of the old image size
  for (unsigned x = 0; x < newWidth; x++) {
    for (unsigned y = 0; y < newHeight; y++) {
      if (x < width_ && y < height_) {
        RGBAPixel & oldPixel = this->getPixel(x, y);
        RGBAPixel & newPixel = newImageData[ (x + (y * newWidth)) ];
        newPixel = oldPixel;
      }
    }
  }

  // Clear the existing image
  delete[] image_data_;

  // Update the image to reflect the new image size and data
  width_ = newWidth;
  height_ = newHeight;
  image_data_ = newImageData;
}

std::ostream & operator<<(std::ostream & out, PNG const & png) {
  std::hash<double> hashFunction;
  std::size_t hash = 0;

  for (unsigned x = 0; x < png.width(); x++) {
    for (unsigned y = 0; y < png.height(); y++) {
      const RGBAPixel &pixel = png.getPixel(x, y);
      hash ^= hashFunction(pixel.r);
      hash ^= hashFunction(pixel.g);
      hash ^= hashFunction(pixel.b);
      hash ^= hashFunction(pixel.a);
    }
  }

  out << "PNG(w=" << png.width() << ", h=" << png.height() << ", hash=" << std::hex << hash << std::dec << ")";
  return out;
}

} // namespace rudnick_rt
