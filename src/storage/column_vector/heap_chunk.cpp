

module;

#include <sstream>

module heap_chunk;

import logger;
import stl;
import hybridsearch_exception;

namespace hybridsearch {

ptr_t StringHeapMgr::Allocate(SizeT nbytes) {
    if (nbytes == 0) {
        String error_message = "Attempt to allocate zero size memory.";
        UnrecoverableError(error_message);
    }
    if (current_chunk_idx_ == std::numeric_limits<u64>::max()) {
        // First chunk
        while (current_chunk_size_ < nbytes) {
            current_chunk_size_ *= 2;
        }
        chunks_.emplace_back(MakeUnique<HeapChunk>(current_chunk_size_));
        current_chunk_idx_ = 0;
    } else {
        if (chunks_[current_chunk_idx_]->current_offset_ + nbytes > current_chunk_size_) {
            // Current chunk can't afford size of nbytes object
            while (current_chunk_size_ < nbytes) {
                current_chunk_size_ *= 2;
            }
            chunks_.emplace_back(MakeUnique<HeapChunk>(current_chunk_size_));
            ++current_chunk_idx_;
        }
        if (chunks_[current_chunk_idx_]->current_offset_ + nbytes > current_chunk_size_) {
            String error_message = "Unexpected string chunk error";
            UnrecoverableError(error_message);
        }
    }

    auto &current_chunk = chunks_[current_chunk_idx_];

    ptr_t result = current_chunk->ptr_ + current_chunk->current_offset_;

    // Allocate the space
    current_chunk->current_offset_ += nbytes;

    // A new object
    ++current_chunk->object_count_;
    return result;
}

String StringHeapMgr::Stats() const {
    std::stringstream ss;
    SizeT chunk_count = chunks_.size();
    ss << "Chunk count: " << chunk_count << std::endl;
    ;
    for (SizeT idx = 0; idx < chunk_count; ++idx) {
        auto &chunk = chunks_[idx];
        ss << "Chunk id: " << idx++ << ", Capacity: " << chunk->capacity_ << ", Current pos: " << chunk->current_offset_
           << ", object count: " << chunk->object_count_ << std::endl;
        ;
    }
    return ss.str();
}

} // namespace hybridsearch
