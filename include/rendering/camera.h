#ifndef CAMERA_H
#define CAMERA_H

#include "math/quat.h"
#include "math/vec3.h"
#include "math/mat4.h"

struct camera {
    struct quat rotation;
    struct vec3 position;
};

void camera_vp_matrix(const struct camera* camera, mat4* out);

#endif // CAMERA_H
