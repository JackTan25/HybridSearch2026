

module;
#include "inc/mlas.h"
module mlas_matrix_multiply;
import stl;

namespace hybridsearch {

static_assert(std::is_same_v<float, f32>);

void matrixA_multiply_matrixB_output_to_C(const float *x,
                                          const float *y,
                                          const SizeT x_row_num,
                                          const SizeT y_col_num,
                                          const SizeT common_dimension,
                                          float *output) {
    MlasGemm(CblasNoTrans,
             CblasNoTrans,
             x_row_num,
             y_col_num,
             common_dimension,
             1.0f,
             x,
             common_dimension,
             y,
             y_col_num,
             0.0f,
             output,
             y_col_num,
             nullptr);
}

void transpose_matrixA_multiply_matrixB_output_to_C(const float *x,
                                                    const float *y,
                                                    const SizeT x_col_num,
                                                    const SizeT y_col_num,
                                                    const SizeT common_dimension,
                                                    float *output) {
    MlasGemm(CblasTrans, CblasNoTrans, x_col_num, y_col_num, common_dimension, 1.0f, x, x_col_num, y, y_col_num, 0.0f, output, y_col_num, nullptr);
}

void matrixA_multiply_transpose_matrixB_output_to_C(const float *x,
                                                    const float *y,
                                                    const SizeT nx,
                                                    const SizeT ny,
                                                    const SizeT dimension,
                                                    float *output) {
    MlasGemm(CblasNoTrans, CblasTrans, nx, ny, dimension, 1.0f, x, dimension, y, dimension, 0.0f, output, ny, nullptr);
}

void transpose_matrixA_multiply_transpose_matrixB_output_to_C(const float *x,
                                                              const float *y,
                                                              const SizeT x_col_num,
                                                              const SizeT y_row_num,
                                                              const SizeT common_dimension,
                                                              float *output) {
    MlasGemm(CblasTrans,
             CblasTrans,
             x_col_num,
             y_row_num,
             common_dimension,
             1.0f,
             x,
             x_col_num,
             y,
             common_dimension,
             0.0f,
             output,
             y_row_num,
             nullptr);
}

void TransposeMatrix(const float *input, float *output, const SizeT input_row_count, const SizeT input_column_count) {
    MlasTranspose(input, output, input_row_count, input_column_count);
}

} // namespace hybridsearch
