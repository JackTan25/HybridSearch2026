

module;

#include <set>

module hnsw_file_worker;

import hybridsearch_exception;
import stl;
import index_file_worker;
import hnsw_alg;
import hnsw_common;
import index_hnsw;

import index_base;
import third_party;
import logger;
import logical_type;
import embedding_info;
import create_index_info;
import internal_types;
import abstract_hnsw;
import virtual_store;
import persistence_manager;
import local_file_handle;

namespace hybridsearch {

HnswFileWorker::HnswFileWorker(SharedPtr<String> data_dir,
                               SharedPtr<String> temp_dir,
                               SharedPtr<String> file_dir,
                               SharedPtr<String> file_name,
                               SharedPtr<IndexBase> index_base,
                               SharedPtr<ColumnDef> column_def,
                               PersistenceManager *persistence_manager,
                               SizeT index_size)
    : IndexFileWorker(std::move(data_dir),
                      std::move(temp_dir),
                      std::move(file_dir),
                      std::move(file_name),
                      std::move(index_base),
                      std::move(column_def),
                      persistence_manager) {
    if (index_size == 0) {

        String index_path = GetFilePath();
        auto [file_handle, status] = VirtualStore::Open(index_path, FileAccessMode::kRead);
        if (status.ok()) {
            // When replay by full checkpoint, the data is deleted, but catalog is recovered. Do not read file in recovery.
            index_size = file_handle->FileSize();
        }
    }
    index_size_ = index_size;
}

HnswFileWorker::~HnswFileWorker() {
    if (data_ != nullptr) {
        FreeInMemory();
        data_ = nullptr;
    }
}

void HnswFileWorker::AllocateInMemory() {
    if (data_) {
        String error_message = "Data is already allocated.";
        UnrecoverableError(error_message);
    }
    data_ = static_cast<void *>(new AbstractHnsw());
}

void HnswFileWorker::FreeInMemory() {
    if (!data_) {
        String error_message = "FreeInMemory: Data is not allocated.";
        UnrecoverableError(error_message);
    }
    auto *p = reinterpret_cast<AbstractHnsw *>(data_);
    std::visit(
        [&](auto &&index) {
            using T = std::decay_t<decltype(index)>;
            if constexpr (!std::is_same_v<T, std::nullptr_t>) {
                if (index != nullptr) {
                    delete index;
                }
            }
        },
        *p);
    delete p;
    data_ = nullptr;
}

bool HnswFileWorker::WriteToFileImpl(bool to_spill, bool &prepare_success, const FileWorkerSaveCtx &ctx) {
    if (!data_) {
        String error_message = "WriteToFileImpl: Data is not allocated.";
        UnrecoverableError(error_message);
    }
    auto *hnsw_index = reinterpret_cast<AbstractHnsw *>(data_);
    std::visit(
        [&](auto &&index) {
            using T = std::decay_t<decltype(index)>;
            if constexpr (std::is_same_v<T, std::nullptr_t>) {
                UnrecoverableError("Invalid index type.");
            } else {
                using IndexT = std::decay_t<decltype(*index)>;
                if constexpr (IndexT::kOwnMem) {
                    index->SaveToPtr(*file_handle_);
                } else {
                    UnrecoverableError("Invalid index type.");
                }
            }
        },
        *hnsw_index);
    prepare_success = true;
    return true;
}

void HnswFileWorker::ReadFromFileImpl(SizeT file_size, bool from_spill) {
    if (data_ != nullptr) {
        UnrecoverableError("Data is already allocated.");
    }
    data_ = static_cast<void *>(new AbstractHnsw(HnswIndexInMem::InitAbstractIndex(index_base_.get(), column_def_.get())));
    auto *hnsw_index = reinterpret_cast<AbstractHnsw *>(data_);
    std::visit(
        [&](auto &&index) {
            using T = std::decay_t<decltype(index)>;
            if constexpr (std::is_same_v<T, std::nullptr_t>) {
                UnrecoverableError("Invalid index type.");
            } else {
                using IndexT = std::decay_t<decltype(*index)>;
                if constexpr (IndexT::kOwnMem) {
                    if (from_spill) {
                        index = IndexT::Load(*file_handle_).release();
                    } else {
                        index = IndexT::LoadFromPtr(*file_handle_, file_size).release();
                    }
                } else {
                    UnrecoverableError("Invalid index type.");
                }
            }
        },
        *hnsw_index);
}

bool HnswFileWorker::ReadFromMmapImpl(const void *ptr, SizeT size) {
    if (mmap_data_ != nullptr) {
        UnrecoverableError("Mmap data is already allocated.");
    }
    mmap_data_ = reinterpret_cast<u8 *>(new AbstractHnsw(HnswIndexInMem::InitAbstractIndex(index_base_.get(), column_def_.get(), false)));
    auto *hnsw_index = reinterpret_cast<AbstractHnsw *>(mmap_data_);
    std::visit(
        [&](auto &&index) {
            using T = std::decay_t<decltype(index)>;
            if constexpr (std::is_same_v<T, std::nullptr_t>) {
                UnrecoverableError("Invalid index type.");
            } else {
                using IndexT = std::decay_t<decltype(*index)>;
                if constexpr (!IndexT::kOwnMem) {
                    const auto *p = static_cast<const char *>(ptr);
                    index = IndexT::LoadFromPtr(p, size).release();
                } else {
                    UnrecoverableError("Invalid index type.");
                }
            }
        },
        *hnsw_index);
    return true;
}

void HnswFileWorker::FreeFromMmapImpl() {
    if (mmap_data_ == nullptr) {
        UnrecoverableError("Mmap data is not allocated.");
    }
    auto *hnsw_index = reinterpret_cast<AbstractHnsw *>(mmap_data_);
    std::visit(
        [&](auto &&index) {
            using T = std::decay_t<decltype(index)>;
            if constexpr (!std::is_same_v<T, std::nullptr_t>) {
                delete index;
            }
        },
        *hnsw_index);
    delete hnsw_index;
    mmap_data_ = nullptr;
}

} // namespace hybridsearch