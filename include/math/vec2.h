#ifndef VEC2_H
#define VEC2_H

#include "fixed32.h"

struct vec2 {
    fixed32 x;
    fixed32 y;
};

static inline void vec2_add_and_assign(struct vec2* a, struct vec2 b) {
    a->x += b.x;
    a->y += b.y;
}

static inline void vec2_mul_and_assign(struct vec2* a, struct vec2 b) {
    a->x = fixed32_mul(a->x, b.x);
    a->y = fixed32_mul(a->y, b.y);
}

static inline struct vec2 vec2_sub(struct vec2 a, struct vec2 b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

static inline fixed32 vec2_cross(struct vec2 a, struct vec2 b) {
    return fixed32_mul(a.x, b.y) - fixed32_mul(a.y, b.x);
}

#endif // VEC2_H
