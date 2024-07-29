#include "constants.h"
#include "math/quat.h"
#include "math/vec3.h"
#include "rendering/camera.h"
#include "rendering/frametime_text.h"
#include "rendering/model.h"
#include "rendering/rasterization.h"
#include "rendering/vertex_processing.h"

#include <stdint.h>
#include <stdbool.h>

static const struct vec3 VERTICES[60] = {
    {0x00000000, 0xffff0000, 0x00000000},
    {0x0000b93d, 0xffff8d84, 0x00008695},
    {0xffffb93f, 0xffff8d84, 0x0000d9c3},
    {0x0000b93d, 0xffff8d84, 0x00008695},
    {0x00000000, 0xffff0000, 0x00000000},
    {0x0000b93d, 0xffff8d84, 0xffff796b},
    {0x00000000, 0xffff0000, 0x00000000},
    {0xffffb93f, 0xffff8d84, 0x0000d9c3},
    {0xffff1b07, 0xffff8d84, 0x00000000},
    {0x00000000, 0xffff0000, 0x00000000},
    {0xffff1b07, 0xffff8d84, 0x00000000},
    {0xffffb93f, 0xffff8d84, 0xffff263d},
    {0x00000000, 0xffff0000, 0x00000000},
    {0xffffb93f, 0xffff8d84, 0xffff263d},
    {0x0000b93d, 0xffff8d84, 0xffff796b},
    {0x0000b93d, 0xffff8d84, 0x00008695},
    {0x0000b93d, 0xffff8d84, 0xffff796b},
    {0x0000e4f9, 0x0000727c, 0x00000000},
    {0xffffb93f, 0xffff8d84, 0x0000d9c3},
    {0x0000b93d, 0xffff8d84, 0x00008695},
    {0x000046c1, 0x0000727c, 0x0000d9c3},
    {0xffff1b07, 0xffff8d84, 0x00000000},
    {0xffffb93f, 0xffff8d84, 0x0000d9c3},
    {0xffff46c3, 0x0000727c, 0x00008695},
    {0xffffb93f, 0xffff8d84, 0xffff263d},
    {0xffff1b07, 0xffff8d84, 0x00000000},
    {0xffff46c3, 0x0000727c, 0xffff796b},
    {0x0000b93d, 0xffff8d84, 0xffff796b},
    {0xffffb93f, 0xffff8d84, 0xffff263d},
    {0x000046c1, 0x0000727c, 0xffff263d},
    {0x0000b93d, 0xffff8d84, 0x00008695},
    {0x0000e4f9, 0x0000727c, 0x00000000},
    {0x000046c1, 0x0000727c, 0x0000d9c3},
    {0xffffb93f, 0xffff8d84, 0x0000d9c3},
    {0x000046c1, 0x0000727c, 0x0000d9c3},
    {0xffff46c3, 0x0000727c, 0x00008695},
    {0xffff1b07, 0xffff8d84, 0x00000000},
    {0xffff46c3, 0x0000727c, 0x00008695},
    {0xffff46c3, 0x0000727c, 0xffff796b},
    {0xffffb93f, 0xffff8d84, 0xffff263d},
    {0xffff46c3, 0x0000727c, 0xffff796b},
    {0x000046c1, 0x0000727c, 0xffff263d},
    {0x0000b93d, 0xffff8d84, 0xffff796b},
    {0x000046c1, 0x0000727c, 0xffff263d},
    {0x0000e4f9, 0x0000727c, 0x00000000},
    {0x000046c1, 0x0000727c, 0x0000d9c3},
    {0x0000e4f9, 0x0000727c, 0x00000000},
    {0x00000000, 0x00010000, 0x00000000},
    {0xffff46c3, 0x0000727c, 0x00008695},
    {0x000046c1, 0x0000727c, 0x0000d9c3},
    {0x00000000, 0x00010000, 0x00000000},
    {0xffff46c3, 0x0000727c, 0xffff796b},
    {0xffff46c3, 0x0000727c, 0x00008695},
    {0x00000000, 0x00010000, 0x00000000},
    {0x000046c1, 0x0000727c, 0xffff263d},
    {0xffff46c3, 0x0000727c, 0xffff796b},
    {0x00000000, 0x00010000, 0x00000000},
    {0x0000e4f9, 0x0000727c, 0x00000000},
    {0x000046c1, 0x0000727c, 0xffff263d},
    {0x00000000, 0x00010000, 0x00000000}
};

static const struct camera CAMERA = {
    {FIXED32_ONE, FIXED32_ZERO, FIXED32_ZERO, FIXED32_ZERO},
    {FIXED32_ZERO, FIXED32_ZERO, FIXED32_ZERO}
};

static const struct quat ROTATE_RIGHT_BY_10_DEG = {
    FIXED32_CONST(0, 9961947, 0), FIXED32_ZERO, FIXED32_CONST(0, 871557, 1), FIXED32_ZERO
};

static struct model model = {
    {FIXED32_ONE, FIXED32_ZERO, FIXED32_ZERO, FIXED32_ZERO},
    {FIXED32_ZERO, FIXED32_ZERO, FIXED32_CONST(-2, 0, 0)},
    VERTICES,
    60
};

static void clear_frame(uint8_t* color_buffer) {
    for (uint32_t i = 0; i < BUFFERS_WIDTH * BUFFERS_HEIGHT / 8; i++) {
        color_buffer[i] = 0x00;
    }
}

static void draw_model(uint8_t* color_buffer, const struct camera* camera, const struct model* model) {
    mat4 mvp;
    produce_mvp_matrix(model, camera, &mvp);
    struct vec3 processed_vertices[3];
    for (uint32_t i = 0; i < model->vertices_count; i += 3) {
        process_triangle(&mvp, model->vertices + i, processed_vertices);
        rasterize_triangle(color_buffer, processed_vertices);
    }
}

void draw_frame(uint8_t* color_buffer) {
    quat_normalize(&model.rotation);
    clear_frame(color_buffer);

    draw_model(color_buffer, &CAMERA, &model);

    draw_frametime_text(color_buffer);

    model.rotation = quat_mul(&model.rotation, &ROTATE_RIGHT_BY_10_DEG);
}
