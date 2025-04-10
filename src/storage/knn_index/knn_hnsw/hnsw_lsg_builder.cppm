

module;

namespace hybridsearch {
struct SegmentEntry;
class ColumnDef;
struct RowID;
} // namespace hybridsearch

export module hnsw_lsg_builder;

import stl;

namespace hybridsearch {

struct HnswIndexInMem;
class BufferManager;
struct IVF_Search_Params;
class KnnDistanceBase1;
class IVFIndexInChunk;
class IndexHnsw;

export class HnswLSGBuilder {
public:
    HnswLSGBuilder(const IndexHnsw *index_hnsw, SharedPtr<ColumnDef> column_def);

    ~HnswLSGBuilder();

    UniquePtr<HnswIndexInMem> Make(const SegmentEntry *segment_entry, SizeT column_id, TxnTimeStamp begin_ts, bool check_ts, BufferManager *buffer_mgr, bool trace = true);

    template <typename Iter, typename DataType, typename DistanceDataType>
    UniquePtr<HnswIndexInMem> MakeImplIter(Iter iter, SizeT row_count, const RowID &base_row_id, bool trace);

private:
    template <typename DataType, typename DistanceDataType>
    UniquePtr<HnswIndexInMem> MakeImpl(const SegmentEntry *segment_entry, SizeT column_id, TxnTimeStamp begin_ts, bool check_ts, BufferManager *buffer_mgr, bool trace);

public:
    template <typename Iter, typename DataType, typename DistanceDataType>
    UniquePtr<float[]> GetLSAvg(Iter iter, SizeT row_count, const RowID &base_row_id);

private:
    UniquePtr<IVFIndexInChunk> MakeIVFIndex();

    IVF_Search_Params MakeIVFSearchParams() const;

    template <typename Iter, typename DataType, template <typename, typename> typename Compare, typename DistanceDataType>
    UniquePtr<float[]> GetAvgByIVF(Iter iter, SizeT row_count);

    template <typename Iter, typename DataType, template <typename, typename> typename Compare, typename DistanceDataType>
    UniquePtr<float[]> GetAvgBF(Iter iter, SizeT row_count);

private:
    const IndexHnsw *index_hnsw_ = nullptr;
    const SharedPtr<ColumnDef> column_def_ = nullptr;

    UniquePtr<KnnDistanceBase1> knn_distance_;
};

} // namespace hybridsearch
