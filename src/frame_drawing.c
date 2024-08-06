#include "constants.h"
#include "math/quat.h"
#include "math/vec3.h"
#include "rendering/camera.h"
#include "rendering/frametime_text.h"
#include "rendering/model.h"
#include "rendering/rasterization.h"
#include "rendering/vertex_processing.h"
#include "mesh_data.h"

#include <stdint.h>
#include <stdbool.h>

static const struct camera CAMERA = {
    {FIXED32_CONST(0, 9689124, 0), FIXED32_CONST(0, -247404, 0), FIXED32_ZERO, FIXED32_ZERO},
    {FIXED32_ZERO, FIXED32_ONE, FIXED32_ZERO}
};

static const struct quat ROTATE_RIGHT_BY_10_DEG = {
    FIXED32_CONST(0, 9961947, 0), FIXED32_ZERO, FIXED32_CONST(0, 871557, 1), FIXED32_ZERO
};

static struct model model = {
    {FIXED32_ONE, FIXED32_ZERO, FIXED32_ZERO, FIXED32_ZERO},
    {FIXED32_ZERO, FIXED32_ZERO, FIXED32_CONST(-2, 0, 0)},
    VERTICES,
    NORMALS,
    TRIANGLES_COUNT
};

static void clear_frame(uint8_t* color_buffer) {
    for (uint32_t i = 0; i < BUFFERS_WIDTH * BUFFERS_HEIGHT / 8; i++) {
        color_buffer[i] = 0x00;
    }
}

static void draw_model(uint8_t* color_buffer, const struct camera* camera, const struct model* model) {
    mat4 m;
    model_matrix(model, &m);
    mat4 vp;
    camera_vp_matrix(camera, &vp);
    mat4 mvp;
    mat4_mul(&mvp, &vp, &m);

    struct vec3 processed_vertices[3];
    struct vec3 processed_normal;
    for (uint32_t i = 0; i < model->triangles_count; i++) {
        process_triangle(&mvp, model->vertices + i * 3, processed_vertices);
        process_normal(&m, model->normals + i, &processed_normal);
        rasterize_triangle(color_buffer, processed_vertices, &processed_normal);
    }
}

void draw_frame(uint8_t* color_buffer) {
    quat_normalize(&model.rotation);

    clear_frame(color_buffer);
    draw_model(color_buffer, &CAMERA, &model);
    draw_frametime_text(color_buffer);

    model.rotation = quat_mul(&model.rotation, &ROTATE_RIGHT_BY_10_DEG);
}
