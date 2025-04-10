

module;

export module var_heap;

import global_resource_usage;
import stl;
import allocator;
import default_values;
import vector_heap_chunk;

namespace hybridsearch {
#if 0
export struct VarHeapManager {
    // Use to store string.
    static constexpr u64 CHUNK_COUNT_LIMIT = MAX_VECTOR_CHUNK_COUNT;
    static constexpr u64 INVALID_CHUNK_OFFSET = std::numeric_limits<u64>::max();

public:
    inline explicit VarHeapManager(u64 chunk_size = MIN_VECTOR_CHUNK_SIZE) : current_chunk_size_(chunk_size) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount();
#endif
    }

    inline ~VarHeapManager() {
#ifdef hybridsearch_DEBUG
GlobalResourceUsage::DecrObjectCount();
#endif
}

    // return value: start chunk id & chunk offset
    Pair<u64, u64> AppendToHeap(const char* data_ptr, SizeT nbytes);

    // Read #nbytes size of data from offset: #chunk_offset of chunk: #chunk_id to buffer: #buffer, Make sure the buffer has enough space to hold
    // the size of data.
    void ReadFromHeap(char* buffer, u64 chunk_id, u64 chunk_offset, SizeT nbytes);

    [[nodiscard]] String Stats() const;


public:
    // Shouldn't use it except unit test
    [[nodiscard]] inline SizeT chunks() const { return chunks_.size(); }

    [[nodiscard]] inline u64 current_chunk_idx() const { return current_chunk_idx_; }

    [[nodiscard]] inline u64 current_chunk_size() const { return current_chunk_size_; }

    [[nodiscard]] inline u64 total_size() const { return current_chunk_size_ * current_chunk_idx_ + current_chunk_offset_; }

    [[nodiscard]] inline u64 total_mem() const { return current_chunk_size_ * (current_chunk_idx_ + 1); }

private:
    // Allocate new chunk if current chunk is not enough.
    // return value: start chunk id and start offset of the chunk id
    Pair<u64, u64> Allocate(SizeT nbytes);

private:
    Vector<UniquePtr<VectorHeapChunk>> chunks_{};
    u64 current_chunk_size_{MIN_VECTOR_CHUNK_SIZE};
    u64 current_chunk_idx_{INITIAL_VECTOR_CHUNK_ID};
    u64 current_chunk_offset_{0};
};
#endif
} // namespace hybridsearch
