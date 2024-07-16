#include "rendering/camera.h"
#include "math/fixed32.h"
#include "math/vec3.h"

/*
 * Precomputed perspective matrix
 * 1 / (aspect * tan(fpv / 2))  | 0                     | 0                         | 0
 * 0                            | 1 / (tan(fov / 2))    | 0                         | 0
 * 0                            | 0                     | z_far / (z_far - z_near)  | -(z_far * z_near) / (z_far - z_near)
 * 0                            | 0                     | 1                         | 0
 * Where aspect = 2 (128/64)
 * fov = PI/2
 * z_far = 50
 * z_near = 0.1
 */
static const mat4 PROJECTION_MATRIX = {
    {FIXED32_CONST(0, 5, 0), FIXED32_ZERO,    FIXED32_ZERO,                        FIXED32_ZERO},
    {FIXED32_ZERO,           FIXED32_ONE,     FIXED32_ZERO,                        FIXED32_ZERO},
    {FIXED32_ZERO,           FIXED32_ZERO,    FIXED32_CONST(1, 2004008016, 2),     FIXED32_ONE},
    {FIXED32_ZERO,           FIXED32_ZERO,    FIXED32_CONST(0, -100200400801, 0),  FIXED32_ZERO},
};

static const struct vec3 FORWARD_VECTOR = {FIXED32_ZERO, FIXED32_ZERO, FIXED32_MINUS_ONE};
static const struct vec3 UP_VECTOR = {FIXED32_ZERO, FIXED32_ONE, FIXED32_ZERO};

static void compute_view_matrix(const struct camera* camera, mat4* out) {
    struct vec3 up = quat_mul_by_vec3(&camera->rotation, &UP_VECTOR);
    struct vec3 forward = quat_mul_by_vec3(&camera->rotation, &FORWARD_VECTOR);
    // TODO: It might be more efficient to just multiply camera rotation by a constant "right" vector.
    struct vec3 right = vec3_cross(&up, &forward);
    vec3_normalize(&right);

    (*out)[0][0] = right.x;
    (*out)[0][1] = right.y;
    (*out)[0][2] = right.z;
    (*out)[0][3] = FIXED32_ZERO;
    (*out)[1][0] = up.x;
    (*out)[1][1] = up.y;
    (*out)[1][2] = up.z;
    (*out)[1][3] = FIXED32_ZERO;
    (*out)[2][0] = forward.x;
    (*out)[2][1] = forward.y;
    (*out)[2][2] = forward.z;
    (*out)[2][3] = FIXED32_ZERO;
    (*out)[3][0] = -vec3_dot(&right, &camera->position);
    (*out)[3][1] = -vec3_dot(&up, &camera->position);
    (*out)[3][2] = -vec3_dot(&forward, &camera->position);
    (*out)[3][3] = FIXED32_ONE;
}

void camera_vp_matrix(const struct camera* camera, mat4* out) {
    mat4 view_matrix;
    compute_view_matrix(camera, &view_matrix);
    mat4_mul(out, &PROJECTION_MATRIX, &view_matrix);
}
