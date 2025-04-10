

module;

export module heap_chunk;

import global_resource_usage;
import stl;
import allocator;

namespace hybridsearch {

export struct HeapChunk {
public:
    inline explicit HeapChunk(u64 capacity) : current_offset_(0), capacity_(capacity), object_count_(0) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("HeapChunk");
#endif
        ptr_ = Allocator::allocate(capacity);
    }

    inline ~HeapChunk() {
        Allocator::deallocate(ptr_);
        ptr_ = nullptr;
        capacity_ = 0;
        current_offset_ = 0;
        object_count_ = 0;
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("HeapChunk");
#endif
    }

    ptr_t ptr_{nullptr};
    u64 current_offset_{0};
    u64 capacity_{0};
    u64 object_count_{0};
};

export struct StringHeapMgr {
    // Use to store string.
    static constexpr u64 CHUNK_SIZE = 4096;

public:
    inline explicit StringHeapMgr(u64 chunk_size = CHUNK_SIZE) : current_chunk_size_(chunk_size) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("StringHeapMgr");
#endif
    }

    inline ~StringHeapMgr() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("StringHeapMgr");
#endif
    }

    // return pointer of required size. Allocate new chunk if current chunk is not enough.
    ptr_t Allocate(SizeT nbytes);

    [[nodiscard]] String Stats() const;

public:
    [[nodiscard]] inline SizeT chunks() const { return chunks_.size(); }

    [[nodiscard]] inline u64 current_chunk_idx() const { return current_chunk_idx_; }

    [[nodiscard]] inline u64 current_chunk_size() const { return current_chunk_size_; }

private:
    Vector<UniquePtr<HeapChunk>> chunks_;
    u64 current_chunk_size_{CHUNK_SIZE};
    u64 current_chunk_idx_{std::numeric_limits<u64>::max()};
};

} // namespace hybridsearch
