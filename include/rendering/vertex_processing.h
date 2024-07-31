#ifndef VERTEX_PROCESSING_H
#define VERTEX_PROCESSING_H

#include "math/vec3.h"
#include "math/mat4.h"

void process_triangle(const mat4* mvp, const struct vec3* source, struct vec3* result);
void process_normal(const mat4* model_matrix, const struct vec3* source, struct vec3* result);

#endif // VERTEX_PROCESSING_H
