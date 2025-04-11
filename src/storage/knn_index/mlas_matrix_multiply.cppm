

module;
export module mlas_matrix_multiply;
import stl;

namespace hybridsearch {

export void
matrixA_multiply_matrixB_output_to_C(const float *x, const float *y, SizeT x_row_num, SizeT y_col_num, SizeT common_dimension, float *output);

export void transpose_matrixA_multiply_matrixB_output_to_C(const float *x,
                                                           const float *y,
                                                           SizeT x_col_num,
                                                           SizeT y_col_num,
                                                           SizeT common_dimension,
                                                           float *output);

export void matrixA_multiply_transpose_matrixB_output_to_C(const float *x, const float *y, SizeT nx, SizeT ny, SizeT dimension, float *output);

export void transpose_matrixA_multiply_transpose_matrixB_output_to_C(const float *x,
                                                                     const float *y,
                                                                     SizeT x_col_num,
                                                                     SizeT y_row_num,
                                                                     SizeT common_dimension,
                                                                     float *output);

export void TransposeMatrix(const float *input, float *output, SizeT input_row_count, SizeT input_column_count);

} // namespace hybridsearch
