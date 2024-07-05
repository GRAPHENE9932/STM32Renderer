#ifndef VEC3_H
#define VEC3_H

#include "fixed16_16.h"

struct vec3 {
    fixed16_16 x;
    fixed16_16 y;
    fixed16_16 z;
};

fixed16_16 vec3_dot(const struct vec3* a, const struct vec3* b);
struct vec3 vec3_cross(const struct vec3* a, const struct vec3* b);

#endif // VEC3_H
