#include "math/vec3.h"
#include "math/fixed32.h"

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

void vec3_normalize(struct vec3* v) {
    fixed32 denominator = fixed32_inv_sqrt(
        fixed32_mul(v->x, v->x) +
        fixed32_mul(v->y, v->y) +
        fixed32_mul(v->z, v->z)
    );

    v->x = fixed32_mul(v->x, denominator);
    v->y = fixed32_mul(v->y, denominator);
    v->z = fixed32_mul(v->z, denominator);
}
