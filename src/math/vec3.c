#include "math/vec3.h"

fixed16_16 vec3_dot(const struct vec3* a, const struct vec3* b) {
    return fixed16_16_mul(a->x, b->x) +
        fixed16_16_mul(a->y, b->y) +
        fixed16_16_mul(a->z, b->z);
}

struct vec3 vec3_cross(const struct vec3* a, const struct vec3* b) {
    return (struct vec3) {
        fixed16_16_mul(a->y, b->z) - fixed16_16_mul(a->z, b->y),
        fixed16_16_mul(a->z, b->x) - fixed16_16_mul(a->x, b->z),
        fixed16_16_mul(a->x, b->y) - fixed16_16_mul(a->y, b->x)
    };
}
