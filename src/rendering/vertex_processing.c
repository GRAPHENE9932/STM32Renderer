#include "rendering/vertex_processing.h"
#include "math/fixed32.h"
#include "math/mat4.h"
#include "math/vec3.h"
#include "math/vec4.h"

#include <stdint.h>

void process_triangle(const mat4* mvp, const struct vec3* source, struct vec3* result) {
    for (uint_fast8_t i = 0; i < 3; i++) {
        struct vec4 v_src = vec4_from_vec3(&source[i], FIXED32_ONE);
        struct vec4 v_out = mat4_mul_vec4(mvp, &v_src);
        
        const fixed32 w_inverse = fixed32_div(FIXED32_ONE, v_out.w);
        result[i].x = fixed32_mul(v_out.x, w_inverse);
        result[i].y = fixed32_mul(v_out.y, w_inverse);
        result[i].z = fixed32_mul(v_out.z, w_inverse);
    }
}

void process_normal(const mat4* model_matrix, const struct vec3* source, struct vec3* result) {
    struct vec4 v_src = vec4_from_vec3(source, FIXED32_ZERO);
    struct vec4 v_out = mat4_mul_vec4(model_matrix, &v_src);
    *result = vec3_from_vec4(&v_out);
    vec3_normalize(result);
}
