#ifndef VERTEX_PROCESSING_H
#define VERTEX_PROCESSING_H

#include "model.h"
#include "camera.h"
#include "math/vec3.h"

void produce_mvp_matrix(const struct model* model, const struct camera* camera, mat4* out);
void process_triangle(const mat4* mvp, const struct vec3* source, struct vec3* result);

#endif // VERTEX_PROCESSING_H
