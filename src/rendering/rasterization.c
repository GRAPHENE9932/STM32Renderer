#include "rendering/rasterization.h"
#include "constants.h"
#include "math/vec2.h"
#include "math/fixed32.h"

#include <stdbool.h>

static bool is_inside_triangle(struct vec2 point, struct vec2* vertices) {
    fixed32 y2_minus_y3 = vertices[1].y - vertices[2].y;
    fixed32 x3_minus_x2 = vertices[2].x - vertices[1].x;
    fixed32 x_minus_x3 = point.x - vertices[2].x;
    fixed32 y_minus_y3 = point.y - vertices[2].y;
    fixed32 x1_minus_x3 = vertices[0].x - vertices[2].x;
    fixed32 y1_minus_y3 = vertices[0].y - vertices[2].y;
    fixed32 y3_minus_y1 = vertices[2].y - vertices[0].y;

    fixed32 denominator = fixed32_mul(y2_minus_y3, x1_minus_x3) + fixed32_mul(x3_minus_x2, y1_minus_y3);

    fixed32 t1 = fixed32_div(
        fixed32_mul(y2_minus_y3, x_minus_x3) +
        fixed32_mul(x3_minus_x2, y_minus_y3),
        denominator
    );

    fixed32 t2 = fixed32_div(
        fixed32_mul(y3_minus_y1, x_minus_x3) +
        fixed32_mul(x1_minus_x3, y_minus_y3),
        denominator
    );

    fixed32 t3 = FIXED32_ONE - t1 - t2;

    return FIXED32_ZERO <= t1 && t1 <= FIXED32_ONE &&
        FIXED32_ZERO <= t2 && t2 <= FIXED32_ONE &&
        FIXED32_ZERO <= t3 && t3 <= FIXED32_ONE;
}

static struct vec2 to_screen_space(struct vec2 point) {
    vec2_add_and_assign(&point, (struct vec2) {FIXED32_ONE, FIXED32_ONE});
    vec2_mul_and_assign(&point, (struct vec2) {FIXED32_CONST(BUFFERS_WIDTH / 2, 0, 0), FIXED32_CONST(BUFFERS_HEIGHT / 2, 0, 0)});
    point = vec2_sub(
        (struct vec2) {FIXED32_CONST(BUFFERS_WIDTH, 0, 0), FIXED32_CONST(BUFFERS_HEIGHT, 0, 0)},
        point
    );
    return point;
}

static uint8_t draw_byte(uint32_t x_div_8, uint32_t y, struct vec2* vertices) {
    uint8_t result = 0;

    for (uint32_t x_bit = 0; x_bit < 8; x_bit++) {
        result |= is_inside_triangle(
            (struct vec2) {fixed32_from_uint32(x_div_8 * 8 + x_bit), fixed32_from_uint32(y)},
            vertices
        ) << (7 - x_bit);
    }

    return result;
}

void rasterize_triangle(uint8_t* buffer, const struct vec3* vertices) {
    struct vec2 scr_space_vertices[3];
    scr_space_vertices[0] = to_screen_space((struct vec2) {vertices[0].x, vertices[0].y});
    scr_space_vertices[1] = to_screen_space((struct vec2) {vertices[1].x, vertices[1].y});
    scr_space_vertices[2] = to_screen_space((struct vec2) {vertices[2].x, vertices[2].y});

    for (uint32_t y = 0; y < BUFFERS_HEIGHT; y++) {
        for (uint32_t x_byte = 0; x_byte < BUFFERS_WIDTH / 8; x_byte++) {
            buffer[y * BUFFERS_WIDTH / 8 + x_byte] = draw_byte(x_byte, y, scr_space_vertices);
        }
    }
}
