#ifndef QUAT_H
#define QUAT_H

#include "fixed32.h"
#include "math/vec3.h"

struct quat {
    fixed32 w;
    fixed32 x;
    fixed32 y;
    fixed32 z;
};

struct quat quat_mul(const struct quat* a, const struct quat* b);
struct vec3 quat_mul_by_vec3(const struct quat* q, const struct vec3* v);
void quat_normalize(struct quat* q);

#endif // QUAT_H
