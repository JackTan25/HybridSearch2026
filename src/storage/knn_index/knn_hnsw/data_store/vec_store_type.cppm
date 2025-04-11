

module;

export module vec_store_type;

import stl;
import plain_vec_store;
import sparse_vec_store;
import lvq_vec_store;
import dist_func_cos;
import dist_func_l2;
import dist_func_ip;
import dist_func_sparse_ip;
import sparse_util;
import dist_func_lsg_wrapper;

namespace hybridsearch {

export template <typename DataT, typename CompressT>
class LVQCosVecStoreType;

export template <typename DataT, typename CompressT>
class LVQL2VecStoreType;

export template <typename DataT, typename CompressT>
class LVQIPVecStoreType;

export template <typename DataT, bool LSG = false>
class PlainCosVecStoreType {
public:
    using DataType = DataT;
    using CompressType = void;
    template <bool OwnMem>
    using Meta = PlainVecStoreMeta<DataType>;
    template <bool OwnMem>
    using Inner = PlainVecStoreInner<DataType, OwnMem>;
    using QueryVecType = const DataType *;
    using StoreType = typename Meta<true>::StoreType;
    using QueryType = typename Meta<true>::QueryType;
    using Distance = std::conditional_t<LSG, PlainCosLSGDist<DataType>, PlainCosDist<DataType>>;

    static constexpr bool HasOptimize = false;

    template <typename CompressType>
    static constexpr LVQCosVecStoreType<DataType, CompressType> ToLVQ() {
        return {};
    }
};

export template <typename DataT, bool LSG = false>
class PlainL2VecStoreType {
public:
    using DataType = DataT;
    using CompressType = void;
    template <bool OwnMem>
    using Meta = PlainVecStoreMeta<DataType>;
    template <bool OwnMem>
    using Inner = PlainVecStoreInner<DataType, OwnMem>;
    using QueryVecType = const DataType *;
    using StoreType = typename Meta<true>::StoreType;
    using QueryType = typename Meta<true>::QueryType;
    using Distance = std::conditional_t<LSG, PlainL2LSGDist<DataType>, PlainL2Dist<DataType>>;

    static constexpr bool HasOptimize = false;

    template <typename CompressType>
    static constexpr LVQL2VecStoreType<DataType, CompressType> ToLVQ() {
        return {};
    }
};

export template <typename DataT, bool LSG = false>
class PlainIPVecStoreType {
public:
    using DataType = DataT;
    using CompressType = void;
    template <bool OwnMem>
    using Meta = PlainVecStoreMeta<DataType>;
    template <bool OwnMem>
    using Inner = PlainVecStoreInner<DataType, OwnMem>;
    using QueryVecType = const DataType *;
    using StoreType = typename Meta<true>::StoreType;
    using QueryType = typename Meta<true>::QueryType;
    using Distance = std::conditional_t<LSG, PlainIPLSGDist<DataType>, PlainIPDist<DataType>>;

    static constexpr bool HasOptimize = false;

    template <typename CompressType>
    static constexpr LVQIPVecStoreType<DataType, CompressType> ToLVQ() {
        return {};
    }
};

export template <typename DataT, typename IndexT>
class SparseIPVecStoreType {
public:
    using DataType = DataT;
    using CompressType = void;
    template <bool OwnMem>
    using Meta = SparseVecStoreMeta<DataT, IndexT>;
    template <bool OwnMem>
    using Inner = SparseVecStoreInner<DataT, IndexT>;
    using QueryVecType = SparseVecRef<DataT, IndexT>;
    using StoreType = typename Meta<true>::StoreType;
    using QueryType = typename Meta<true>::QueryType;
    using Distance = SparseIPDist<DataT, IndexT>;

    static constexpr bool HasOptimize = false;

    template <typename CompressType>
    static constexpr SparseIPVecStoreType<DataType, IndexT> ToLVQ() {
        return {};
    }
};

export template <typename DataT, typename CompressT>
class LVQCosVecStoreType {
public:
    using DataType = DataT;
    using CompressType = CompressT;
    template <bool OwnMem>
    using Meta = LVQVecStoreMeta<DataType, CompressType, LVQCosCache<DataType, CompressType>, OwnMem>;
    template <bool OwnMem>
    using Inner = LVQVecStoreInner<DataType, CompressType, LVQCosCache<DataType, CompressType>, OwnMem>;
    using QueryVecType = const DataType *;
    using MetaType = LVQVecStoreMetaType<DataType, CompressType, LVQCosCache<DataType, CompressType>>;
    using StoreType = typename MetaType::StoreType;
    using QueryType = typename MetaType::QueryType;
    using Distance = LVQCosDist<DataType, CompressType>;

    static constexpr bool HasOptimize = true;

    template <typename CompressType>
    static constexpr LVQCosVecStoreType<DataType, CompressType> ToLVQ() {
        return {};
    }
};

export template <typename DataT, typename CompressT>
class LVQL2VecStoreType {
public:
    using DataType = DataT;
    using CompressType = CompressT;
    template <bool OwnMem>
    using Meta = LVQVecStoreMeta<DataType, CompressType, LVQL2Cache<DataType, CompressType>, OwnMem>;
    template <bool OwnMem>
    using Inner = LVQVecStoreInner<DataType, CompressType, LVQL2Cache<DataType, CompressType>, OwnMem>;
    using QueryVecType = const DataType *;
    using MetaType = LVQVecStoreMetaType<DataType, CompressType, LVQL2Cache<DataType, CompressType>>;
    using StoreType = typename MetaType::StoreType;
    using QueryType = typename MetaType::QueryType;
    using Distance = LVQL2Dist<DataType, CompressType>;

    static constexpr bool HasOptimize = true;

    template <typename CompressType>
    static constexpr LVQL2VecStoreType<DataType, CompressType> ToLVQ() {
        return {};
    }
};

export template <typename DataT, typename CompressT>
class LVQIPVecStoreType {
public:
    using DataType = DataT;
    using CompressType = CompressT;
    template <bool OwnMem>
    using Meta = LVQVecStoreMeta<DataType, CompressType, LVQIPCache<DataType, CompressType>, OwnMem>;
    template <bool OwnMem>
    using Inner = LVQVecStoreInner<DataType, CompressType, LVQIPCache<DataType, CompressType>, OwnMem>;
    using QueryVecType = const DataType *;
    using MetaType = LVQVecStoreMetaType<DataType, CompressType, LVQIPCache<DataType, CompressType>>;
    using StoreType = typename MetaType::StoreType;
    using QueryType = typename MetaType::QueryType;
    using Distance = LVQIPDist<DataType, CompressType>;

    static constexpr bool HasOptimize = true;

    template <typename CompressType>
    static constexpr LVQIPVecStoreType<DataType, CompressType> ToLVQ() {
        return {};
    }
};

} // namespace hybridsearch
