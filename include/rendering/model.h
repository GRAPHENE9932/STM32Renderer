#ifndef MODEL_H
#define MODEL_H

#include "math/mat4.h"
#include "math/quat.h"
#include "math/vec3.h"

struct model {
    struct quat rotation;
    struct vec3 position;
    const struct vec3* vertices;
    const struct vec3* normals;
    uint32_t triangles_count;
};

void model_matrix(const struct model* model, mat4* out);

#endif // MODEL_H
