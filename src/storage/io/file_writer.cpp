

module;

#include <set>

module file_writer;

import stl;
import virtual_store;
import local_file_handle;
import hybridsearch_exception;
import logger;

namespace hybridsearch {

FileWriter::FileWriter(const String &path, SizeT buffer_size)
    : path_(path), data_(MakeUnique<char_t[]>(buffer_size)), offset_(0), total_written_(0), buffer_size_(buffer_size) {
    // Fixme: Open file out of constructor
    auto [file_handle, status] = VirtualStore::Open(path, FileAccessMode::kWrite);
    if (!status.ok()) {
        UnrecoverableError(status.message());
    }
    file_handle_ = std::move(file_handle);
}

void FileWriter::WriteByte(const u8 b) {
    if (offset_ == buffer_size_) {
        Flush();
    }
    data_[offset_++] = b;
}

void FileWriter::WriteShort(const i16 i) {
    WriteByte(static_cast<u8>(i >> 8));
    WriteByte(static_cast<u8>(i));
}

void FileWriter::WriteInt(const i32 i) {
    WriteByte((u8)(i >> 24));
    WriteByte((u8)(i >> 16));
    WriteByte((u8)(i >> 8));
    WriteByte((u8)i);
}

void FileWriter::WriteVInt(const i32 vi) {
    u32 i = vi;
    while ((i & ~0x7F) != 0) {
        WriteByte((u8)((i & 0x7f) | 0x80));
        i >>= 7;
    }
    WriteByte((u8)i);
}

void FileWriter::WriteLong(const i64 i) {
    WriteInt((i32)(i >> 32));
    WriteInt((i32)i);
}

void FileWriter::WriteVLong(const i64 vi) {
    u64 i = vi;
    while ((i & ~0x7F) != 0) {
        WriteByte((u8)((i & 0x7f) | 0x80));
        i >>= 7;
    }
    WriteByte((u8)i);
}

void FileWriter::Write(const char_t *buffer, SizeT bytes_count) {
    char_t *start_pos = (char_t *)buffer;
    char_t *end_pos = start_pos + bytes_count;
    while (start_pos < end_pos) {
        i64 byte_count1 = end_pos - start_pos;
        i64 byte_count2 = buffer_size_ - offset_;
        i64 to_write = std::min(byte_count1, byte_count2);
        std::memcpy(data_.get() + offset_, start_pos, to_write);
        offset_ += to_write;
        start_pos += to_write;
        if (offset_ == buffer_size_) {
            Flush();
        }
    }
}

void FileWriter::Sync() {
    Flush();
    file_handle_->Sync();
}

void FileWriter::Flush() {
    if (offset_ == 0) {
        return;
    }
    file_handle_->Append(data_.get(), offset_);
    total_written_ += offset_;
    offset_ = 0;
}

i64 FileWriter::GetFileSize() { return file_handle_->FileSize() + offset_; }

SizeT FileWriter::TotalWrittenBytes() const { return total_written_ + offset_; }

} // namespace hybridsearch
