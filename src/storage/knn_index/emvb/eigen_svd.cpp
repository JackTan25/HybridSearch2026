

module;

#include "JacobiSVD.h"
module eigen_svd;

namespace hybridsearch {

// A = U * S * V^T
// A, U, V: dimension * dimension
// S: dimension
void EMVBSVDSolve(const int dimension, float *A, float *U, float *V) {
    // TODO: now only consider square matrix
    // TODO: now only use jacobi svd
    return Eigen::JacobiSVDSolve(dimension, A, U, V);
}

} // namespace hybridsearch
