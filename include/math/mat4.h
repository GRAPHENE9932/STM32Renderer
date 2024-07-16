#ifndef MAT4_H
#define MAT4_H

#include "fixed32.h"
#include "quat.h"
#include "vec4.h"

typedef fixed32 mat4[4][4];

void mat4_mul(mat4* out, const mat4* a, const mat4* b);
struct vec4 mat4_mul_vec4(const mat4* m, const struct vec4* v);
void mat4_from_quat(mat4* out, const struct quat* q);
void mat4_from_translation(mat4* out, const struct vec3* v);

#endif // MAT4_H
