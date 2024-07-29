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
struct vec3 processed_vertices[60];

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
    20
};

static void clear_frame(uint8_t* color_buffer) {
    for (uint32_t i = 0; i < BUFFERS_WIDTH * BUFFERS_HEIGHT / 8; i++) {
        color_buffer[i] = 0x00;
    }
}

void draw_frame(uint8_t* color_buffer) {
    quat_normalize(&model.rotation);
    clear_frame(color_buffer);
    process_vertices(&model, &CAMERA, processed_vertices);

    for (uint32_t i = 0; i < 60; i += 3) {
        rasterize_triangle(color_buffer, processed_vertices + i);
    }

    draw_frametime_text(color_buffer);

    model.rotation = quat_mul(&model.rotation, &ROTATE_RIGHT_BY_10_DEG);
}
