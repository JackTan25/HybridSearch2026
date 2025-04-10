

module;

#include <cassert>

export module column_length_io;

import stl;
import index_defines;
import internal_types;
import chunk_index_entry;
import memory_indexer;
import buffer_obj;
import buffer_handle;
import column_index_reader;

namespace hybridsearch {
class FileSystem;

export class FullTextColumnLengthReader {
public:
    explicit FullTextColumnLengthReader(ColumnIndexReader *reader);

    inline u32 GetColumnLength(RowID row_id) {
        if (row_id >= current_chunk_base_rowid_ && row_id < current_chunk_base_rowid_ + current_chunk_row_count_) [[likely]] {
            assert(column_lengths_ != nullptr);
            return column_lengths_[row_id - current_chunk_base_rowid_];
        }
        if (memory_indexer_.get() != nullptr) {
            RowID base_rowid = memory_indexer_->GetBaseRowId();
            u32 doc_count = memory_indexer_->GetDocCount();
            if (row_id >= base_rowid && row_id < base_rowid + doc_count) {
                return memory_indexer_->GetColumnLength(row_id - base_rowid);
            }
        }
        return SeekFile(row_id);
    }

    inline u64 GetTotalDF() const { return total_df_; }
    inline float GetAvgColumnLength() const { return avg_column_len_; }

private:
    u32 SeekFile(RowID row_id);
    const String &index_dir_;
    const Vector<SharedPtr<ChunkIndexEntry>> &chunk_index_entries_; // must in ascending order
    SharedPtr<MemoryIndexer> memory_indexer_;
    u64 total_df_;
    float avg_column_len_;
    const u32 *column_lengths_{nullptr};
    RowID current_chunk_base_rowid_{(u64)0};
    u32 current_chunk_row_count_{0};
    BufferHandle current_chunk_buffer_handle_{};
};

} // namespace hybridsearch