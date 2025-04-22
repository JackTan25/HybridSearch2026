module;

module memory_chunk;

import stl;

// #include <memory>
// #include <limits.h>
// #include <fcntl.h>
// #include <algorithm>
// #include <sys/resource.h>
// #include <fstream>
// #include <iostream>
// #include <unistd.h>
// #include <chrono>
// #include<cstring>
// #include <thread>
// using namespace std;

namespace hybridsearch {

// inline size_t getProcessPeakRSS() {
//     struct rusage rusage;
//     getrusage(RUSAGE_SELF, &rusage);
//     return (size_t) rusage.ru_maxrss / 1024L;
// }

// inline size_t getCurrentRSS() {
//     long rss = 0L;
//     FILE *fp = NULL;
//     if ((fp = fopen("/proc/self/statm", "r")) == NULL)
//         return (size_t) 0L;      /* Can't open? */
//     if (fscanf(fp, "%*s%ld", &rss) != 1) {
//         fclose(fp);
//         return (size_t) 0L;      /* Can't read? */
//     }
//     fclose(fp);
//     return (size_t) (rss * (size_t) sysconf(_SC_PAGESIZE))/1024/1024L;
// }

// void monitorMemoryUsage(){
//     while (true) {
//         size_t peak_rss = getProcessPeakRSS();
//         size_t current_rss = getCurrentRSS();
//         std::cout << "Peak RSS: " << peak_rss << " KB, Current RSS: " << current_rss << " KB" << std::endl;
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 每100毫秒记录一次
//     }
// }

void *MemoryChunk::Allocate(SizeT num_bytes) {
    if (size_ - pos_ < num_bytes) {
        return nullptr;
    }
    void *ptr = (void *)((char *)(this) + pos_);
    pos_ += num_bytes;
    return ptr;
}

MemoryChunk *ChunkAllocator::Allocate(SizeT num_bytes) {
    u32 alloc_size = num_bytes + sizeof(ChainedMemoryChunk);
    if (alloc_size <= chunk_size_) {
        alloc_size = chunk_size_;
    }

    ChainedMemoryChunk *next_chunk = current_chunk_ ? current_chunk_->Next() : chunk_header_;
    if (next_chunk && alloc_size <= chunk_size_) {
        current_chunk_ = next_chunk;
    } else {
        const auto allocated_chunk = static_cast<void *>(new char[alloc_size]);
        if (!allocated_chunk) {
            return nullptr;
        }
        next_chunk = new (allocated_chunk) ChainedMemoryChunk(alloc_size);
        if (!next_chunk) {
            return nullptr;
        }
        total_bytes_ += alloc_size;
        if (!chunk_header_) {
            current_chunk_ = chunk_header_ = next_chunk;
        } else {
            if (current_chunk_) {
                next_chunk->Next() = current_chunk_->Next();
                if (current_chunk_->Next()) {
                    current_chunk_->Next()->Prev() = next_chunk;
                }
                current_chunk_->Next() = next_chunk;
                next_chunk->Prev() = current_chunk_;
            } else {
                next_chunk->Next() = chunk_header_;
                chunk_header_->Prev() = next_chunk;
                chunk_header_ = next_chunk;
            }
            current_chunk_ = next_chunk;
        }
    }
    used_bytes_ += current_chunk_->GetTotalBytes();
    return current_chunk_;
}

SizeT ChunkAllocator::Release() {
    ChainedMemoryChunk *pChunk = chunk_header_;
    ChainedMemoryChunk *pChunk2 = nullptr;
    while (pChunk) {
        pChunk2 = pChunk;
        pChunk = pChunk2->Next();
        delete[] (char *)pChunk2;
    }
    chunk_header_ = current_chunk_ = nullptr;
    SizeT total_bytes = total_bytes_;
    used_bytes_ = total_bytes_ = 0;
    return total_bytes;
}

SizeT ChunkAllocator::Reset() {
    SizeT total_bytes = total_bytes_;
    if (current_chunk_ == nullptr) {
        // skip useless reset to avoid cache-miss
        return total_bytes;
    }
    for (ChainedMemoryChunk *chunk = chunk_header_; chunk;) {
        if (chunk->GetTotalBytes() <= chunk_size_) {
            chunk->Reset();
            chunk = chunk->Next();
        } else {
            ChainedMemoryChunk *prev_chunk = chunk->Prev();
            ChainedMemoryChunk *next_chunk = chunk->Next();
            total_bytes_ -= chunk->GetTotalBytes();
            delete[] (char *)chunk;
            chunk = next_chunk;
            if (prev_chunk) {
                prev_chunk->Next() = next_chunk;
            } else {
                chunk_header_ = next_chunk;
            }
            if (next_chunk) {
                next_chunk->Prev() = prev_chunk;
            }
        }
    }
    used_bytes_ = 0;
    current_chunk_ = nullptr;
    return total_bytes;
}

void ChunkAllocator::Clear() {
    ChainedMemoryChunk *pChunk = chunk_header_;
    while (pChunk) {
        pChunk->Clear();
        pChunk = pChunk->Next();
    }
}

} // namespace hybridsearch