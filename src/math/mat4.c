#include "math/mat4.h"
#include "math/fixed32.h"

#include <stdint.h>

void mat4_mul(mat4* out, const mat4* a, const mat4* b) {
    for (uint_fast8_t i = 0; i < 4; i++) {
        for (uint_fast8_t j = 0; j < 4; j++) {
            (*out)[i][j] = fixed32_mul((*a)[0][j], (*b)[i][0]) +
                fixed32_mul((*a)[1][j], (*b)[i][1]) +
                fixed32_mul((*a)[2][j], (*b)[i][2]) +
                fixed32_mul((*a)[3][j], (*b)[i][3]);
        }
    }
}

struct vec4 mat4_mul_vec4(const mat4* m, const struct vec4* v) {
    return (struct vec4) {
        fixed32_mul(v->x, (*m)[0][0]) + fixed32_mul(v->y, (*m)[1][0]) + fixed32_mul(v->z, (*m)[2][0]) + fixed32_mul(v->w, (*m)[3][0]),
        fixed32_mul(v->x, (*m)[0][1]) + fixed32_mul(v->y, (*m)[1][1]) + fixed32_mul(v->z, (*m)[2][1]) + fixed32_mul(v->w, (*m)[3][1]),
        fixed32_mul(v->x, (*m)[0][2]) + fixed32_mul(v->y, (*m)[1][2]) + fixed32_mul(v->z, (*m)[2][2]) + fixed32_mul(v->w, (*m)[3][2]),
        fixed32_mul(v->x, (*m)[0][3]) + fixed32_mul(v->y, (*m)[1][3]) + fixed32_mul(v->z, (*m)[2][3]) + fixed32_mul(v->w, (*m)[3][3])
    };
}

void mat4_from_quat(mat4* out, const struct quat* q) {
    fixed32 ii = fixed32_mul(q->x, q->x);
    fixed32 jj = fixed32_mul(q->y, q->y);
    fixed32 kk = fixed32_mul(q->z, q->z);
    fixed32 ir = fixed32_mul(q->x, q->w);
    fixed32 ij = fixed32_mul(q->x, q->y);
    fixed32 ik = fixed32_mul(q->x, q->z);
    fixed32 jr = fixed32_mul(q->y, q->w);
    fixed32 jk = fixed32_mul(q->y, q->z);
    fixed32 kr = fixed32_mul(q->z, q->w);

    (*out)[0][0] = FIXED32_ONE - ((jj + kk) << 1);
    (*out)[0][1] = (ij + kr) << 1;
    (*out)[0][2] = (ik - jr) << 1;
    (*out)[0][3] = FIXED32_ZERO;

    (*out)[1][0] = (ij - kr) << 1;
    (*out)[1][1] = FIXED32_ONE - ((ii + kk) << 1);
    (*out)[1][2] = (jk + ir) << 1;
    (*out)[1][3] = FIXED32_ZERO;

    (*out)[2][0] = (ik + jr) << 1;
    (*out)[2][1] = (jk - ir) << 1;
    (*out)[2][2] = FIXED32_ONE - ((ii + jj) << 1);
    (*out)[2][3] = FIXED32_ZERO;

    (*out)[3][0] = FIXED32_ZERO;
    (*out)[3][1] = FIXED32_ZERO;
    (*out)[3][2] = FIXED32_ZERO;
    (*out)[3][3] = FIXED32_ONE;
}

void mat4_from_translation(mat4* out, const struct vec3* v) {
    (*out)[0][0] = FIXED32_ONE;
    (*out)[0][1] = FIXED32_ZERO;
    (*out)[0][2] = FIXED32_ZERO;
    (*out)[0][3] = FIXED32_ZERO;

    (*out)[1][0] = FIXED32_ZERO;
    (*out)[1][1] = FIXED32_ONE;
    (*out)[1][2] = FIXED32_ZERO;
    (*out)[1][3] = FIXED32_ZERO;

    (*out)[2][0] = FIXED32_ZERO;
    (*out)[2][1] = FIXED32_ZERO;
    (*out)[2][2] = FIXED32_ONE;
    (*out)[2][3] = FIXED32_ZERO;

    (*out)[3][0] = v->x;
    (*out)[3][1] = v->y;
    (*out)[3][2] = v->z;
    (*out)[3][3] = FIXED32_ONE;
}
