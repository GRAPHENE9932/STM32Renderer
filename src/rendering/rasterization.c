#include "rendering/rasterization.h"
#include "constants.h"
#include "math/vec2.h"
#include "math/fixed32.h"

#include <stdbool.h>
#include <stdint.h>

static struct vec2 to_screen_space(struct vec2 point) {
    vec2_add_and_assign(&point, (struct vec2) {FIXED32_ONE, FIXED32_ONE});
    vec2_mul_and_assign(&point, (struct vec2) {FIXED32_CONST(BUFFERS_WIDTH / 2, 0, 0), FIXED32_CONST(BUFFERS_HEIGHT / 2, 0, 0)});
    point = vec2_sub(
        (struct vec2) {FIXED32_CONST(BUFFERS_WIDTH, 0, 0), FIXED32_CONST(BUFFERS_HEIGHT, 0, 0)},
        point
    );
    return point;
}

static fixed32 get_intersection_point_x(fixed32 y_of_line_1, const struct vec2* points_of_line_1[2]) {
    const fixed32 x1y2 = fixed32_mul(points_of_line_1[0]->x, points_of_line_1[1]->y);
    const fixed32 x2y1 = fixed32_mul(points_of_line_1[1]->x, points_of_line_1[0]->y);
    const fixed32 x2_minus_x1 = points_of_line_1[1]->x - points_of_line_1[0]->x;
    const fixed32 y2_minus_y1 = points_of_line_1[1]->y - points_of_line_1[0]->y;

    return fixed32_div(
        x1y2 - x2y1 + fixed32_mul(x2_minus_x1, y_of_line_1),
        y2_minus_y1
    );
}

static void draw_line(uint8_t* buffer, const struct vec2* vertices, uint32_t y_int) {
    const fixed32 y = fixed32_from_uint32(y_int) + FIXED32_CONST(0, 5, 0);

    // Check if the line is intersecting the triangle. If it isn't, then just fill the whole line with zeroes.
    if ((y < vertices[0].y && y < vertices[1].y && y < vertices[2].y) ||
        (y > vertices[0].y && y > vertices[1].y && y > vertices[2].y)) {
        for (uint_fast8_t i = 0; i < (BUFFERS_WIDTH >> 3); i++) {
            buffer[i] = 0;
        }
        return;
    }

    // Find the two sides of this triangle which intersect the line.
    const struct vec2* intersecting_lines[2][2];
    if ((vertices[0].y >= y && vertices[1].y < y && vertices[2].y < y) ||
        (vertices[0].y <= y && vertices[1].y > y && vertices[2].y > y)) {
        intersecting_lines[0][0] = &vertices[0];
        intersecting_lines[0][1] = &vertices[1];
        intersecting_lines[1][0] = &vertices[0];
        intersecting_lines[1][1] = &vertices[2];
    }
    else if ((vertices[1].y >= y && vertices[0].y < y && vertices[2].y < y) ||
             (vertices[1].y <= y && vertices[0].y > y && vertices[2].y > y)) {
        intersecting_lines[0][0] = &vertices[1];
        intersecting_lines[0][1] = &vertices[0];
        intersecting_lines[1][0] = &vertices[1];
        intersecting_lines[1][1] = &vertices[2];
    }
    else {
        intersecting_lines[0][0] = &vertices[2];
        intersecting_lines[0][1] = &vertices[0];
        intersecting_lines[1][0] = &vertices[2];
        intersecting_lines[1][1] = &vertices[1];
    }

    fixed32 intersection_x_1 = get_intersection_point_x(y, intersecting_lines[0]);
    fixed32 intersection_x_2 = get_intersection_point_x(y, intersecting_lines[1]);
    if (intersection_x_1 > intersection_x_2) {
        fixed32 tmp = intersection_x_2;
        intersection_x_2 = intersection_x_1;
        intersection_x_1 = tmp;
    }

    fixed32 x = FIXED32_CONST(0, 5, 0);
    for (uint_fast8_t i = 0; i < BUFFERS_WIDTH; i++) {
        uint8_t pixel_enabled = intersection_x_1 <= x && x < intersection_x_2;
        pixel_enabled <<= 7 - i % 8;
        buffer[i / 8] |= pixel_enabled;

        x += FIXED32_ONE;
    }
}

void rasterize_triangle(uint8_t* buffer, const struct vec3* vertices) {
    struct vec2 scr_space_vertices[3];
    scr_space_vertices[0] = to_screen_space((struct vec2) {vertices[0].x, vertices[0].y});
    scr_space_vertices[1] = to_screen_space((struct vec2) {vertices[1].x, vertices[1].y});
    scr_space_vertices[2] = to_screen_space((struct vec2) {vertices[2].x, vertices[2].y});

    for (uint32_t y = 0; y < BUFFERS_HEIGHT; y++) {
        draw_line(buffer + y * (BUFFERS_WIDTH >> 3), scr_space_vertices, y);
    }
}
