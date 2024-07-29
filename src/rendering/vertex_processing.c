#include "rendering/vertex_processing.h"
#include "math/fixed32.h"
#include "math/mat4.h"
#include "math/vec4.h"
#include "rendering/camera.h"
#include "rendering/model.h"

#include <stdint.h>

void produce_mvp_matrix(const struct model* model, const struct camera* camera, mat4* out) {
    mat4 m;
    model_matrix(model, &m);
    mat4 vp;
    camera_vp_matrix(camera, &vp);
    mat4_mul(out, &vp, &m);
}

void process_triangle(const mat4* mvp, const struct vec3* source, struct vec3* result) {
    for (uint_fast8_t i = 0; i < 3; i++) {
        struct vec4 v_src = vec4_from_vec3(&source[i], FIXED32_ONE);
        struct vec4 v_out = mat4_mul_vec4(mvp, &v_src);
        result[i].x = fixed32_div(v_out.x, v_out.w);
        result[i].y = fixed32_div(v_out.y, v_out.w);
        result[i].z = fixed32_div(v_out.z, v_out.w);
    }
}
