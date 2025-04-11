

module;

module ivf_index_file_worker;

import stl;
import index_file_worker;
import file_worker;
import logger;
import index_base;
import ivf_index_data;
import hybridsearch_exception;
import third_party;
import persistence_manager;

namespace hybridsearch {

IVFIndexFileWorker::~IVFIndexFileWorker() {
    if (data_ != nullptr) {
        FreeInMemory();
        data_ = nullptr;
    }
}

void IVFIndexFileWorker::AllocateInMemory() {
    if (data_) [[unlikely]] {
        UnrecoverableError("AllocateInMemory: Already allocated.");
    }
    data_ = static_cast<void *>(IVFIndexInChunk::GetNewIVFIndexInChunk(index_base_.get(), column_def_.get()));
}

void IVFIndexFileWorker::FreeInMemory() {
    if (data_) [[likely]] {
        auto index = static_cast<IVFIndexInChunk *>(data_);
        delete index;
        data_ = nullptr;
        LOG_TRACE("Finished FreeInMemory(), deleted data_ ptr.");
    } else {
        UnrecoverableError("FreeInMemory: Data is not allocated.");
    }
}

bool IVFIndexFileWorker::WriteToFileImpl(bool to_spill, bool &prepare_success, const FileWorkerSaveCtx &ctx) {
    if (data_) [[likely]] {
        auto index = static_cast<IVFIndexInChunk *>(data_);
        index->SaveIndexInner(*file_handle_);
        prepare_success = true;
        LOG_TRACE("Finished WriteToFileImpl(bool &prepare_success).");
    } else {
        UnrecoverableError("WriteToFileImpl: data_ is nullptr");
    }
    return true;
}

void IVFIndexFileWorker::ReadFromFileImpl(SizeT file_size, bool from_spill) {
    if (!data_) [[likely]] {
        auto index = IVFIndexInChunk::GetNewIVFIndexInChunk(index_base_.get(), column_def_.get());
        index->ReadIndexInner(*file_handle_);
        data_ = static_cast<void *>(index);
        LOG_TRACE("Finished ReadFromFileImpl().");
    } else {
        UnrecoverableError("ReadFromFileImpl: data_ is not nullptr");
    }
}

} // namespace hybridsearch