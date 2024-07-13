#include "math/vec3.h"

fixed32 vec3_dot(const struct vec3* a, const struct vec3* b) {
    return fixed32_mul(a->x, b->x) +
        fixed32_mul(a->y, b->y) +
        fixed32_mul(a->z, b->z);
}

struct vec3 vec3_cross(const struct vec3* a, const struct vec3* b) {
    return (struct vec3) {
        fixed32_mul(a->y, b->z) - fixed32_mul(a->z, b->y),
        fixed32_mul(a->z, b->x) - fixed32_mul(a->x, b->z),
        fixed32_mul(a->x, b->y) - fixed32_mul(a->y, b->x)
    };
}
