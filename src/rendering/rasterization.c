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

static bool is_within_the_half_plane(fixed32 x, fixed32 y, const struct vec3* coeffs) {
    return fixed32_mul(coeffs->x, x) + fixed32_mul(coeffs->y, y) <= -coeffs->z;
}

static bool is_inside_triangle(fixed32 x, fixed32 y, const struct vec3* coeffs) {
    return is_within_the_half_plane(x, y, &coeffs[0]) &&
        is_within_the_half_plane(x, y, &coeffs[1]) &&
        is_within_the_half_plane(x, y, &coeffs[2]);
}

static void rasterize_within_rectangle(
    uint8_t* buffer, const struct vec2* top_left, const struct vec2* bottom_right,
    const struct vec3* coeffs, fixed32 color
) {
    uint32_t starting_x = fixed32_round_to_uint32(top_left->x);
    uint32_t ending_x = fixed32_round_to_uint32(bottom_right->x - FIXED32_ONE);
    uint32_t starting_y = fixed32_round_to_uint32(top_left->y);
    uint32_t ending_y = fixed32_round_to_uint32(bottom_right->y - FIXED32_ONE);

    for (uint32_t scr_y = starting_y; scr_y <= ending_y; scr_y++) {
        for(uint32_t scr_x = starting_x; scr_x <= ending_x; scr_x++) {
            fixed32 x = fixed32_from_uint32(scr_x) + FIXED32_CONST(0, 5, 0);
            fixed32 y = fixed32_from_uint32(scr_y) + FIXED32_CONST(0, 5, 0);

            if (!is_inside_triangle(x, y, coeffs)) {
                continue;
            }

            bool pixel_color = dither_color_to_monochrome(scr_x, scr_y, color);

            if (!pixel_color) {
                continue;
            }

            uint32_t buffer_index = (scr_y * BUFFERS_WIDTH + scr_x) / 8;
            uint8_t mask = 1 << (7 - scr_x % 8);
            buffer[buffer_index] |= mask;
        }
    }
}

static struct vec3 compute_coeffs_for_edge(const struct vec2* vertex_1, const struct vec2* vertex_2) {
    fixed32 a = vertex_1->y - vertex_2->y;
    fixed32 b = vertex_2->x - vertex_1->x;
    fixed32 c = fixed32_mul(vertex_1->x, vertex_2->y) - fixed32_mul(vertex_2->x, vertex_1->y);

    return (struct vec3) {a, b, c};
}

static fixed32 min_of_2(fixed32 a, fixed32 b) {
    return a < b ? a : b;
}

static fixed32 max_of_2(fixed32 a, fixed32 b) {
    return a > b ? a : b;
}

static fixed32 min_of_3(fixed32 a, fixed32 b, fixed32 c) {
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

static fixed32 max_of_3(fixed32 a, fixed32 b, fixed32 c) {
    return a > b ? (a > c ? a : c) : (b > c ? b : c);
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

static bool is_counter_clockwise(const struct vec2* vertices) {
    fixed32 cross_product = vec2_cross(
        vec2_sub(vertices[1], vertices[0]),
        vec2_sub(vertices[2], vertices[0])
    );

    return cross_product > FIXED32_ZERO;
}

void rasterize_triangle(uint8_t* buffer, const struct vec3* vertices, const struct vec3* normal) {
    struct vec2 scr_space_vertices[3];
    scr_space_vertices[0] = to_screen_space((struct vec2) {vertices[0].x, vertices[0].y});
    scr_space_vertices[1] = to_screen_space((struct vec2) {vertices[1].x, vertices[1].y});
    scr_space_vertices[2] = to_screen_space((struct vec2) {vertices[2].x, vertices[2].y});

    // We are rendering only counter-clockwise triangles.
    // But scr_space_vertices are flipped in y-axis, so the clockwiseness is reversed.
    if (is_counter_clockwise(scr_space_vertices)) {
        return;
    }

    struct vec3 coeffs[3];
    coeffs[0] = compute_coeffs_for_edge(&scr_space_vertices[0], &scr_space_vertices[1]);
    coeffs[1] = compute_coeffs_for_edge(&scr_space_vertices[1], &scr_space_vertices[2]);
    coeffs[2] = compute_coeffs_for_edge(&scr_space_vertices[2], &scr_space_vertices[0]);

    // As triangle color depends only on the normal, we can compute it once.
    fixed32 triangle_color = compute_fragment_color(normal);

    fixed32 min_x = min_of_3(scr_space_vertices[0].x, scr_space_vertices[1].x, scr_space_vertices[2].x);
    fixed32 max_x = max_of_3(scr_space_vertices[0].x, scr_space_vertices[1].x, scr_space_vertices[2].x);
    fixed32 min_y = min_of_3(scr_space_vertices[0].y, scr_space_vertices[1].y, scr_space_vertices[2].y);
    fixed32 max_y = max_of_3(scr_space_vertices[0].y, scr_space_vertices[1].y, scr_space_vertices[2].y);

    fixed32 left_x = max_of_2(min_x, FIXED32_ZERO);
    fixed32 right_x = min_of_2(max_x, FIXED32_CONST(BUFFERS_WIDTH, 0, 0));
    fixed32 top_y = max_of_2(min_y, FIXED32_ZERO);
    fixed32 bottom_y = min_of_2(max_y, FIXED32_CONST(BUFFERS_HEIGHT, 0, 0));

    struct vec2 top_left = {left_x, top_y};
    struct vec2 bottom_right = {right_x, bottom_y};

    rasterize_within_rectangle(buffer, &top_left, &bottom_right, coeffs, triangle_color);
}
