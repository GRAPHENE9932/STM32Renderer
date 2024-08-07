#include "rendering/rasterization.h"
#include "constants.h"
#include "math/vec2.h"
#include "math/fixed32.h"

#include <stdbool.h>
#include <stdint.h>

static const struct vec3 LIGHT_DIR = {
    FIXED32_CONST(0, 577350269189, 0),
    FIXED32_CONST(0, -577350269189, 0),
    FIXED32_CONST(0, -577350269189, 0)
};

static const uint8_t BAYER_MATRIX[4][4] = {
    {0, 12, 3, 15},
    {8, 4, 11, 7},
    {2, 14, 1, 13},
    {10, 6, 9, 5}
};

static fixed32 compute_fragment_color(const struct vec3* normal) {
    struct vec3 neg_light_dir = vec3_neg(LIGHT_DIR);
    fixed32 result = vec3_dot(normal, &neg_light_dir);
    if (result < FIXED32_ZERO) {
        result = FIXED32_ZERO;
    }

    return result;
}

static bool dither_color_to_monochrome(uint32_t x, uint32_t y, fixed32 grayscale_color) {
    fixed32 threshold = BAYER_MATRIX[x & 0b11][y & 0b11] << 12;
    return grayscale_color > threshold;
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

static bool line_intersection_test(const struct vec2* vertices, fixed32 line_y) {
    return !((line_y < vertices[0].y && line_y < vertices[1].y && line_y < vertices[2].y) ||
        (line_y > vertices[0].y && line_y > vertices[1].y && line_y > vertices[2].y));
}

static void extract_intersecting_sides(const struct vec2* vertices, fixed32 y, const struct vec2 *(*out)[2]) {
    if ((vertices[0].y >= y && vertices[1].y < y && vertices[2].y < y) ||
        (vertices[0].y <= y && vertices[1].y > y && vertices[2].y > y)) {
        out[0][0] = &vertices[0];
        out[0][1] = &vertices[1];
        out[1][0] = &vertices[0];
        out[1][1] = &vertices[2];
    }
    else if ((vertices[1].y >= y && vertices[0].y < y && vertices[2].y < y) ||
             (vertices[1].y <= y && vertices[0].y > y && vertices[2].y > y)) {
        out[0][0] = &vertices[1];
        out[0][1] = &vertices[0];
        out[1][0] = &vertices[1];
        out[1][1] = &vertices[2];
    }
    else {
        out[0][0] = &vertices[2];
        out[0][1] = &vertices[0];
        out[1][0] = &vertices[2];
        out[1][1] = &vertices[1];
    }
}

static void draw_line(uint8_t* buffer, const struct vec2* vertices, const struct vec3* normal, uint32_t y_int) {
    const fixed32 y = fixed32_from_uint32(y_int) + FIXED32_CONST(0, 5, 0);

    // Check if the line is intersecting the triangle. If it isn't, do nothing.
    if (!line_intersection_test(vertices, y)) {
        return;
    }

    // Find the two sides of this triangle which intersect the line.
    const struct vec2* intersecting_sides[2][2];
    extract_intersecting_sides(vertices, y, intersecting_sides);

    fixed32 intersection_x_1 = get_intersection_point_x(y, intersecting_sides[0]);
    fixed32 intersection_x_2 = get_intersection_point_x(y, intersecting_sides[1]);
    if (intersection_x_1 > intersection_x_2) {
        fixed32 tmp = intersection_x_2;
        intersection_x_2 = intersection_x_1;
        intersection_x_1 = tmp;
    }

    fixed32 x = FIXED32_CONST(0, 5, 0);
    for (uint_fast8_t i = 0; i < BUFFERS_WIDTH; i++) {
        bool pixel_in_triangle = intersection_x_1 <= x && x < intersection_x_2;
        x += FIXED32_ONE;

        if (!pixel_in_triangle) {
            continue;
        }

        fixed32 color = compute_fragment_color(normal);
        uint8_t pixel_enabled = dither_color_to_monochrome(i, y_int, color);
        pixel_enabled <<= 7 - i % 8;
        buffer[i / 8] |= pixel_enabled;
    }
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

void rasterize_triangle(uint8_t* buffer, const struct vec3* vertices, const struct vec3* normal) {
    struct vec2 scr_space_vertices[3];
    scr_space_vertices[0] = to_screen_space((struct vec2) {vertices[0].x, vertices[0].y});
    scr_space_vertices[1] = to_screen_space((struct vec2) {vertices[1].x, vertices[1].y});
    scr_space_vertices[2] = to_screen_space((struct vec2) {vertices[2].x, vertices[2].y});

    for (uint32_t y = 0; y < BUFFERS_HEIGHT; y++) {
        draw_line(buffer + y * (BUFFERS_WIDTH >> 3), scr_space_vertices, normal, y);
    }
}
