#include "rendering/model.h"
#include "math/mat4.h"

void model_matrix(const struct model* model, mat4* out) {
    mat4 rotation_matrix;
    mat4_from_quat(&rotation_matrix, &model->rotation);

    mat4 translation_matrix;
    mat4_from_translation(&translation_matrix, &model->position);

    mat4_mul(out, &translation_matrix, &rotation_matrix);
}
