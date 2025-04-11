

module;

export module dist_func_sparse_ip;

import stl;
import sparse_vec_store;
import sparse_vector_distance;
import sparse_util;
import hnsw_common;

namespace hybridsearch {

export template <typename DataType, typename IdxType>
class SparseIPDist {
public:
    using VecStoreMeta = SparseVecStoreMeta<DataType, IdxType>;
    using DistanceType = typename VecStoreMeta::DistanceType;
    using StoreType = typename VecStoreMeta::StoreType;

public:
    SparseIPDist() = default;
    SparseIPDist(SizeT dim) {}

    template <typename DataStore>
    DistanceType operator()(VertexType v1_i, VertexType v2_i, const DataStore &data_store) const {
        return Inner(data_store.GetVec(v1_i), data_store.GetVec(v2_i));
    }

    template <typename DataStore>
    DistanceType operator()(const StoreType &v1, VertexType v2_i, const DataStore &data_store, VertexType v1_i = kInvalidVertex) const {
        return Inner(v1, data_store.GetVec(v2_i));
    }

private:
    DistanceType Inner(const StoreType &v1, const StoreType &v2) const {
        return -SparseIPDistance(v1.data_, v1.indices_, v1.nnz_, v2.data_, v2.indices_, v2.nnz_);
    }
};

} // namespace hybridsearch