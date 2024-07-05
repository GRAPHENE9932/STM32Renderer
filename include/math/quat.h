#ifndef QUAT_H
#define QUAT_H

#include "fixed16_16.h"
#include "math/vec3.h"

struct quat {
    fixed16_16 w;
    fixed16_16 x;
    fixed16_16 y;
    fixed16_16 z;
};

struct quat quat_mul(const struct quat* a, const struct quat* b);
struct vec3 quat_mul_by_vec3(const struct quat* q, const struct vec3* v);

#endif // QUAT_H
