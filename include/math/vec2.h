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

#endif // VEC2_H
