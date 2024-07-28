#include "constants.h"
#include "math/vec3.h"
#include "rendering/camera.h"
#include "rendering/frametime_text.h"
#include "rendering/model.h"
#include "rendering/rasterization.h"
#include "rendering/vertex_processing.h"

#include <stdint.h>
#include <stdbool.h>

static const struct vec3 VERTICES[3] = {
    {FIXED32_CONST(0, -5, 0), FIXED32_CONST(0, -5, 0), FIXED32_ZERO},
    {FIXED32_ZERO, FIXED32_CONST(0, 5, 0), FIXED32_ZERO},
    {FIXED32_CONST(0, 5, 0), FIXED32_CONST(0, -5, 0), FIXED32_ZERO}
};
struct vec3 processed_vertices[3];

static const struct camera CAMERA = {
    {FIXED32_ONE, FIXED32_ZERO, FIXED32_ZERO, FIXED32_ZERO},
    {FIXED32_ZERO, FIXED32_ZERO, FIXED32_ZERO}
};

static const struct quat ROTATE_RIGHT_BY_10_DEG = {
    FIXED32_CONST(0, 9961947, 0), FIXED32_ZERO, FIXED32_CONST(0, 871557, 1), FIXED32_ZERO
};

static struct model model = {
    {FIXED32_ONE, FIXED32_ZERO, FIXED32_ZERO, FIXED32_ZERO},
    {FIXED32_ZERO, FIXED32_ZERO, FIXED32_CONST(-1, 0, 0)},
    VERTICES,
    3
};

static void clear_frame(uint8_t* color_buffer) {
    for (uint32_t i = 0; i < BUFFERS_WIDTH * BUFFERS_HEIGHT / 8; i++) {
        color_buffer[i] = 0x00;
    }
}

void draw_frame(uint8_t* color_buffer) {
    clear_frame(color_buffer);
    process_vertices(&model, &CAMERA, processed_vertices);
    rasterize_triangle(color_buffer, processed_vertices);
    draw_frametime_text(color_buffer);

    model.rotation = quat_mul(&model.rotation, &ROTATE_RIGHT_BY_10_DEG);
}
