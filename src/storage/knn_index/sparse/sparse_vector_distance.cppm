

module;

export module sparse_vector_distance;

import stl;

namespace hybridsearch {

export template <typename DataType, typename IndexType, typename ResultType = DataType>
ResultType SparseIPDistance(const DataType *data1, const IndexType *index1, SizeT nnz1, const DataType *data2, const IndexType *index2, SizeT nnz2) {
    ResultType distance{};
    SizeT i = 0, j = 0;
    while (i < nnz1 && j < nnz2) {
        if (index1[i] == index2[j]) {
            distance += data1[i] * data2[j];
            ++i;
            ++j;
        } else if (index1[i] < index2[j]) {
            ++i;
        } else {
            ++j;
        }
    }
    return distance;
}

export template <typename IndexType, typename ResultType = IndexType>
ResultType SparseBitIPDistance(const IndexType *idx1, SizeT nnz1, const IndexType *idx2, SizeT nnz2) {
    ResultType distance{};
    SizeT i = 0, j = 0;
    while (i < nnz1 && j < nnz2) {
        if (idx1[i] == idx2[j]) {
            ++distance;
            ++i;
            ++j;
        } else if (idx1[i] < idx2[j]) {
            ++i;
        } else {
            ++j;
        }
    }
    return distance;
}

} // namespace hybridsearch
