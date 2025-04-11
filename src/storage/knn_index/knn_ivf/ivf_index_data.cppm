

module;

export module ivf_index_data;

import stl;
import index_ivf;
import ivf_index_storage;
import column_def;
import embedding_info;
import internal_types;
import logical_type;
import local_file_handle;
import hybridsearch_exception;

namespace hybridsearch {

class IndexBase;
struct SegmentEntry;
class BufferManager;

export class IVFDataAccessorBase {
public:
    virtual const_ptr_t GetEmbedding(SizeT offset) = 0;

    virtual Pair<Span<const char>, SizeT> GetMultiVector(SizeT offset) = 0;
};

export class IVFIndexInChunk : protected IVF_Index_Storage {
    using IVF_Index_Storage::IVF_Index_Storage;

public:
    using IVF_Index_Storage::GetMemData;

    IVF_Index_Storage *GetIVFIndexStoragePtr() { return this; }

    const IVF_Index_Storage *GetIVFIndexStoragePtr() const { return this; }

    void BuildIVFIndex(RowID base_rowid,
                       u32 row_count,
                       const SegmentEntry *segment_entry,
                       const SharedPtr<ColumnDef> &column_def,
                       BufferManager *buffer_mgr);

    void BuildIVFIndex(RowID base_rowid,
                       u32 row_count,
                       IVFDataAccessorBase *data_accessor,
                       const SharedPtr<ColumnDef> &column_def);

    void SaveIndexInner(LocalFileHandle &file_handle) const;

    void ReadIndexInner(LocalFileHandle &file_handle);

    static IVFIndexInChunk *GetNewIVFIndexInChunk(const IndexBase *ivf_index, const ColumnDef *column_def);

private:
    template <LogicalType column_t, EmbeddingDataType embedding_t>
    void BuildIVFIndexT(RowID base_rowid,
                        u32 row_count,
                        IVFDataAccessorBase *data_accessor,
                        const SharedPtr<ColumnDef> &column_def);
};

} // namespace hybridsearch
