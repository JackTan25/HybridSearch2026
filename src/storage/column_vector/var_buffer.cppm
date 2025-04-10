

module;

export module var_buffer;

import stl;
import buffer_obj;
import buffer_handle;
import logger;

namespace hybridsearch {

struct BlockColumnEntry;
class BufferManager;

export class VarBuffer {
    friend class VarFileWorker;

public:
    VarBuffer() = default;

    VarBuffer(BufferObj *buffer_obj) : buffer_size_prefix_sum_({0}), buffer_obj_(buffer_obj) {}

    // this is called by VarFileWorker
    VarBuffer(BufferObj *buffer_obj, UniquePtr<char[]> buffer, SizeT size) : buffer_size_prefix_sum_({0, size}), buffer_obj_(buffer_obj) {
        std::get<Vector<UniquePtr<char[]>>>(buffers_).push_back(std::move(buffer));
    }

    VarBuffer(BufferObj *buffer_obj, const char *buffer, SizeT size) : buffer_size_prefix_sum_({0, size}), buffer_obj_(buffer_obj) {
        buffers_ = buffer;
    }

public:
    SizeT Append(UniquePtr<char[]> buffer, SizeT size, bool *free_success = nullptr);

    SizeT Append(const char *data, SizeT size, bool *free_success = nullptr);

    const char *Get(SizeT offset, SizeT size) const;

    SizeT Write(char *ptr) const;

    SizeT Write(char *ptr, SizeT offset, SizeT size) const;

    SizeT TotalSize() const;

private:
    mutable std::shared_mutex mtx_;

    std::variant<Vector<UniquePtr<char[]>>, const char *> buffers_;
    Vector<SizeT> buffer_size_prefix_sum_ = {0};

    BufferObj *buffer_obj_ = nullptr;
};

export class VarBufferManager {
public:
    VarBufferManager() : type_(BufferType::kBuffer), mem_buffer_(nullptr) {}

    VarBufferManager(BlockColumnEntry *block_column_entry, BufferManager *buffer_mgr);

    SizeT Append(UniquePtr<char[]> buffer, SizeT size, bool *free_success = nullptr);

    SizeT Append(const char *data, SizeT size, bool *free_success = nullptr);

    const char *Get(SizeT offset, SizeT size) { return GetInner()->Get(offset, size); }

    SizeT Write(char *ptr) { return GetInner()->Write(ptr); }

    SizeT Write(char *ptr, SizeT offset, SizeT size) { return GetInner()->Write(ptr, offset, size); }

    SizeT TotalSize() { return GetInner()->TotalSize(); }

private:
    void InitBuffer();

    VarBuffer *GetInnerMut();

    const VarBuffer *GetInner();

private:
    enum class BufferType { kBuffer, kBufferObj } type_;
    UniquePtr<VarBuffer> mem_buffer_;
    Optional<BufferHandle> buffer_handle_;
    BlockColumnEntry *block_column_entry_ = nullptr;
    BufferManager *buffer_mgr_ = nullptr;
};

} // namespace hybridsearch