

module;

export module ivf_index_data_in_mem;

import stl;
import internal_types;
import index_ivf;
import ivf_index_storage;
import column_def;
import logical_type;
import buffer_handle;
import base_memindex;
import memindex_tracer;
import table_index_entry;

namespace hybridsearch {

struct BlockColumnEntry;
class BufferManager;
struct ChunkIndexEntry;
struct SegmentIndexEntry;
class IndexBase;
class KnnDistanceBase1;

export class IVFIndexInMem : public BaseMemIndex {
protected:
    const RowID begin_row_id_ = {};
    std::atomic_flag have_ivf_index_ = {};
    mutable std::shared_mutex rw_mutex_ = {};
    u32 input_row_count_ = 0;
    u32 input_embedding_count_ = 0;
    IVF_Index_Storage *ivf_index_storage_ = nullptr;
    bool own_ivf_index_storage_ = true;
    BufferHandle dump_handle_{};
    SegmentIndexEntry *segment_index_entry_ = nullptr;

    const IndexIVFOption &ivf_option() const { return ivf_index_storage_->ivf_option(); }
    u32 embedding_dimension() const { return ivf_index_storage_->embedding_dimension(); }

public:
    IVFIndexInMem(RowID begin_row_id,
                  const IndexIVFOption &ivf_option,
                  LogicalType column_logical_type,
                  EmbeddingDataType embedding_data_type,
                  u32 embedding_dimension,
                  SegmentIndexEntry *segment_index_entry);
    virtual ~IVFIndexInMem();
    u32 GetInputRowCount() const;
    virtual void InsertBlockData(SegmentOffset block_offset,
                                 BlockColumnEntry *block_column_entry,
                                 BufferManager *buffer_manager,
                                 u32 row_offset,
                                 u32 row_count) = 0;
    virtual SharedPtr<ChunkIndexEntry> Dump(SegmentIndexEntry *segment_index_entry, BufferManager *buffer_mgr, SizeT *p_dump_size = nullptr) = 0;
    void SearchIndex(const KnnDistanceBase1 *knn_distance,
                     const void *query_ptr,
                     EmbeddingDataType query_element_type,
                     u32 nprobe,
                     const std::function<bool(SegmentOffset)> &satisfy_filter_func,
                     const std::function<void(f32, SegmentOffset)> &add_result_func) const;
    static SharedPtr<IVFIndexInMem>
    NewIVFIndexInMem(const ColumnDef *column_def, const IndexBase *index_base, RowID begin_row_id, SegmentIndexEntry *segment_index_entry);

    virtual SizeT MemoryUsed() const = 0;

private:
    virtual void SearchIndexInMem(const KnnDistanceBase1 *knn_distance,
                                  const void *query_ptr,
                                  EmbeddingDataType query_element_type,
                                  const std::function<bool(SegmentOffset)> &satisfy_filter_func,
                                  const std::function<void(f32, SegmentOffset)> &add_result_func) const = 0;
};

} // namespace hybridsearch
