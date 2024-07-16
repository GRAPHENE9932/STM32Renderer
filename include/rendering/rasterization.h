#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "math/vec3.h"

#include <stdint.h>

void rasterize_triangle(uint8_t* buffer, const struct vec3* vertices);

#endif // RASTERIZER_H
