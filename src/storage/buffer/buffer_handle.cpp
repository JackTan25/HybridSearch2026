

module;

import stl;
import buffer_handle;
import buffer_obj;
import logger;
import file_worker_type;

module buffer_handle;

namespace hybridsearch {
BufferHandle::BufferHandle(BufferObj *buffer_obj, void *data) : buffer_obj_(buffer_obj), data_(data) {}

BufferHandle::BufferHandle(const BufferHandle &other) : buffer_obj_(other.buffer_obj_), data_(other.data_) { buffer_obj_->LoadInner(); }

BufferHandle &BufferHandle::operator=(const BufferHandle &other) {
    if (buffer_obj_) {
        buffer_obj_->UnloadInner();
    }
    buffer_obj_ = other.buffer_obj_;
    data_ = other.data_;
    buffer_obj_->LoadInner();
    return *this;
}

BufferHandle::BufferHandle(BufferHandle &&other) : buffer_obj_(other.buffer_obj_), data_(other.data_) {
    other.buffer_obj_ = nullptr;
    other.data_ = nullptr;
}

BufferHandle &BufferHandle::operator=(BufferHandle &&other) {
    if (buffer_obj_) {
        buffer_obj_->UnloadInner();
    }
    buffer_obj_ = other.buffer_obj_;
    data_ = other.data_;
    other.buffer_obj_ = nullptr;
    other.data_ = nullptr;
    return *this;
}

BufferHandle::~BufferHandle() {
    if (buffer_obj_) {
        buffer_obj_->UnloadInner();
    }
    buffer_obj_ = nullptr;
    data_ = nullptr;
}

const void *BufferHandle::GetData() const { return data_; }

void *BufferHandle::GetDataMut() {
    data_ = buffer_obj_->GetMutPointer();
    return data_;
}

const FileWorker *BufferHandle::GetFileWorker() const { return buffer_obj_->file_worker(); }

FileWorker *BufferHandle::GetFileWorkerMut() {
    data_ = buffer_obj_->GetMutPointer();
    return buffer_obj_->file_worker();
}

} // namespace hybridsearch