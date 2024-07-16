#ifndef VEC4_H
#define VEC4_H

#include "fixed32.h"
#include "vec3.h"

struct vec4 {
    fixed32 x;
    fixed32 y;
    fixed32 z;
    fixed32 w;
};

static inline struct vec4 vec4_from_vec3(const struct vec3* v, fixed32 w) {
    return (struct vec4) {
        v->x,
        v->y,
        v->z,
        w
    };
}

#endif // VEC4_H
