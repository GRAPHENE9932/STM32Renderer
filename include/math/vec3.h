#ifndef VEC3_H
#define VEC3_H

#include "fixed32.h"

struct vec3 {
    fixed32 x;
    fixed32 y;
    fixed32 z;
};

fixed32 vec3_dot(const struct vec3* a, const struct vec3* b);
struct vec3 vec3_cross(const struct vec3* a, const struct vec3* b);
void vec3_normalize(struct vec3* v);

static inline struct vec3 vec3_neg(const struct vec3 v) {
    return (struct vec3) {
        -v.x, -v.y, -v.z
    };
}

#endif // VEC3_H
