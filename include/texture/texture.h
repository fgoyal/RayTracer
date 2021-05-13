/**
 * @file texture.h
 * @author Ian Rudnick
 * Classes for procedural textures.
 */
#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"

/**
 * Abstract class for textures.
 * Derived classes must implement a way to get the texture color value at a
 * specific point.
 */
class texture {
public:
	virtual color value(double u, double v, const point3& p) const = 0;
};


/**
 * Class for basic solid-color textures.
 */
class solid_color_texture : public texture {
public:
	/**
	 * Constructs a default solid color texture.
	 * Sets the texture color to white.
	 */
	solid_color_texture() : color_value_(color(1, 1, 1)) {}

	/**
	 * Constructs a solid color texture with the given color.
	 * @param c The color for the texture.
	 */
	solid_color_texture(color c) : color_value_(c) {}

	/**
	 * Constructs a solid color texture with the given color.
	 * @param red The red value, in range [0, 1].
	 * @param green The green value, in range [0, 1].
	 * @param blue The blue value, in range [0, 1].
	 */
	solid_color_texture(float red, float green, float blue)
		: solid_color_texture(color(red, green, blue)) {}

	/**
	 * Gets the color value of the texture at a specific point.
	 * @return The color value of the texture.
	 */
	virtual color value(double u, double v, const vec3& p) const override {
		return this->color_value_;
	}

private:
	color color_value_;
};

#endif
