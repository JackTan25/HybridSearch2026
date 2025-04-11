

module;

export module merge_knn_data;

import stl;
import table_function;

import base_table_ref;
import merge_knn;
import knn_expr;
import internal_types;

namespace hybridsearch {

export enum class MergeKnnHeapType {
    kMaxHeap,
    kMinHeap,
    kInvalid,
};

export class MergeKnnFunctionData : public TableFunctionData {

public:
    explicit MergeKnnFunctionData(i64 query_count,
                                  i64 topk,
                                  EmbeddingDataType elem_type,
                                  KnnDistanceType knn_distance_type,
                                  SharedPtr<BaseTableRef> table_ref);

private:
    template <typename DatType, typename DistType>
    void InitMergeKnn(KnnDistanceType knn_distance_type);

public:
    i64 query_count_{};
    i64 topk_{};
    EmbeddingDataType elem_type_{EmbeddingDataType::kElemInvalid};
    MergeKnnHeapType heap_type_{MergeKnnHeapType::kInvalid};
    SharedPtr<BaseTableRef> table_ref_{};

    SharedPtr<MergeKnnBase> merge_knn_base_{};
};
} // namespace hybridsearch