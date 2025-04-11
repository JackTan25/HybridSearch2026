

module;

module secondary_index_file_worker;

import stl;
import index_file_worker;
import file_worker;

import logger;
import index_base;
import index_secondary;
import secondary_index_data;
import hybridsearch_exception;
import third_party;
import persistence_manager;

namespace hybridsearch {

SecondaryIndexFileWorker::~SecondaryIndexFileWorker() {
    if (data_ != nullptr) {
        FreeInMemory();
        data_ = nullptr;
    }
}

void SecondaryIndexFileWorker::AllocateInMemory() {
    if (data_) [[unlikely]] {
        UnrecoverableError("AllocateInMemory: Already allocated.");
    } else if (auto &data_type = column_def_->type(); data_type->CanBuildSecondaryIndex()) [[likely]] {
        data_ = static_cast<void *>(GetSecondaryIndexData(data_type, row_count_, true));
        LOG_TRACE("Finished AllocateInMemory().");
    } else {
        UnrecoverableError(fmt::format("Cannot build secondary index on data type: {}", data_type->ToString()));
    }
}

void SecondaryIndexFileWorker::FreeInMemory() {
    if (data_) [[likely]] {
        auto index = static_cast<SecondaryIndexData *>(data_);
        delete index;
        data_ = nullptr;
        LOG_TRACE("Finished FreeInMemory(), deleted data_ ptr.");
    } else {
        UnrecoverableError("FreeInMemory: Data is not allocated.");
    }
}

bool SecondaryIndexFileWorker::WriteToFileImpl(bool to_spill, bool &prepare_success, const FileWorkerSaveCtx &ctx) {
    if (data_) [[likely]] {
        auto index = static_cast<SecondaryIndexData *>(data_);
        index->SaveIndexInner(*file_handle_);
        prepare_success = true;
        LOG_TRACE("Finished WriteToFileImpl(bool &prepare_success).");
    } else {
        UnrecoverableError("WriteToFileImpl: data_ is nullptr");
    }
    return true;
}

void SecondaryIndexFileWorker::ReadFromFileImpl(SizeT file_size, bool from_spill) {
    if (!data_) [[likely]] {
        auto index = GetSecondaryIndexData(column_def_->type(), row_count_, false);
        index->ReadIndexInner(*file_handle_);
        data_ = static_cast<void *>(index);
        LOG_TRACE("Finished ReadFromFileImpl().");
    } else {
        UnrecoverableError("ReadFromFileImpl: data_ is not nullptr");
    }
}

} // namespace hybridsearch
