#ifndef VERTEX_PROCESSING_H
#define VERTEX_PROCESSING_H

#include "model.h"
#include "camera.h"
#include "math/vec3.h"

void process_vertices(const struct model* model, const struct camera* camera, struct vec3* result);

#endif // VERTEX_PROCESSING_H
