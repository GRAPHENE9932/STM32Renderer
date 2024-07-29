#include "math/quat.h"

struct quat quat_mul(const struct quat* a, const struct quat* b) {
    struct quat result;
    result.w = fixed32_mul(a->w, b->w) - fixed32_mul(a->x, b->x) - fixed32_mul(a->y, b->y) - fixed32_mul(a->z, b->z);
    result.x = fixed32_mul(a->w, b->x) + fixed32_mul(a->x, b->w) + fixed32_mul(a->y, b->z) - fixed32_mul(a->z, b->y);
    result.y = fixed32_mul(a->w, b->y) + fixed32_mul(a->y, b->w) + fixed32_mul(a->z, b->x) - fixed32_mul(a->x, b->z);
    result.z = fixed32_mul(a->w, b->z) + fixed32_mul(a->z, b->w) + fixed32_mul(a->x, b->y) - fixed32_mul(a->y, b->x);

    return result;
}

static void conjugate(struct quat* q) {
    q->x = -q->x;
    q->y = -q->y;
    q->z = -q->z;
}

static struct quat pure_quat_from_vec3(const struct vec3* v) {
    struct quat result;
    result.w = 0;
    result.x = v->x;
    result.y = v->y;
    result.z = v->z;

    return result;
}

static struct vec3 vec3_from_pure_quat(const struct quat* q) {
    struct vec3 result;
    result.x = q->x;
    result.y = q->y;
    result.z = q->z;

    return result;
}

struct vec3 quat_mul_by_vec3(const struct quat* q, const struct vec3* v) {
    struct quat q_conjugate = *q;
    conjugate(&q_conjugate);

    struct quat quat_v;
    quat_v = pure_quat_from_vec3(v);

    struct quat result_quat;
    result_quat = quat_mul(q, &quat_v);
    result_quat = quat_mul(&result_quat, &q_conjugate);

    return vec3_from_pure_quat(&result_quat);
}

void quat_normalize(struct quat* q) {
    const fixed32 length = fixed32_sqrt(
        fixed32_mul(q->w, q->w) +
        fixed32_mul(q->x, q->x) +
        fixed32_mul(q->y, q->y) +
        fixed32_mul(q->z, q->z)
    );

    q->w = fixed32_div(q->w, length);
    q->x = fixed32_div(q->x, length);
    q->y = fixed32_div(q->y, length);
    q->z = fixed32_div(q->z, length);
}
