

module;

export module eigen_svd;

namespace hybridsearch {

// A = U * S * V^T
// A, U, V: dimension * dimension
// S: dimension
export void EMVBSVDSolve(int dimension, float *A, float *U, float *V);

} // namespace hybridsearch
