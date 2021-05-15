/**
 * @file stb_image_include.h
 * @author Ian Rudnick
 * Sets up the stb_image library for inclusion in the project.
 */
#ifndef STB_IMAGE_INCLUDE_H
#define STB_IMAGE_INCLUDE_H

// Disable pedantic warnings for this external library.
// #ifdef _MSC_VER
// 	pragma warning (push, 0)
// #endif

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wsign-compare"
// #pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#pragma GCC diagnostic pop

#endif
