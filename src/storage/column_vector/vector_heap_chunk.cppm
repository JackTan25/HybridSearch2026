

module;

export module vector_heap_chunk;

import stl;
import global_resource_usage;
import allocator;
import buffer_obj;
import buffer_handle;
import hybridsearch_exception;

namespace hybridsearch {

export using ChunkId = i64;
export constexpr ChunkId INVALID_CHUNK_ID = -1;

export struct VectorHeapChunk {
public:
    explicit VectorHeapChunk(BufferObj *buffer_obj) : ptr_(buffer_obj->Load()) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("VectorHeapChunk");
#endif
    }

    explicit VectorHeapChunk(u64 capacity) : ptr_(MakeUniqueForOverwrite<char[]>(capacity)) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("VectorHeapChunk");
#endif
    }

    VectorHeapChunk(const VectorHeapChunk &) = delete;

    VectorHeapChunk(VectorHeapChunk &&other) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("VectorHeapChunk");
#endif
        if (std::holds_alternative<UniquePtr<char[]>>(other.ptr_)) {
            ptr_ = std::move(std::get<UniquePtr<char[]>>(other.ptr_));
        } else {
            ptr_ = std::move(std::get<BufferHandle>(other.ptr_));
        }
    }

    VectorHeapChunk &operator=(const VectorHeapChunk &) = delete;

    VectorHeapChunk &operator=(VectorHeapChunk &&) = delete;

    ~VectorHeapChunk() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("VectorHeapChunk");
#endif
    }

    const char *GetPtr() const { // Pattern Matching here
        if (std::holds_alternative<UniquePtr<char[]>>(ptr_)) {
            return std::get<UniquePtr<char[]>>(ptr_).get();
        } else {
            return static_cast<const char *>(std::get<BufferHandle>(ptr_).GetData());
        }
    }

    char *GetPtrMut() {
        if (std::holds_alternative<UniquePtr<char[]>>(ptr_)) {
            return std::get<UniquePtr<char[]>>(ptr_).get();
        } else {
            return static_cast<char *>(std::get<BufferHandle>(ptr_).GetDataMut());
        }
    }

private:
    std::variant<UniquePtr<char[]>, BufferHandle> ptr_;
};

} // namespace hybridsearch
