

module;

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

module local_file_handle;

import third_party;
import status;
import virtual_store;
import hybridsearch_exception;
import logger;

namespace hybridsearch {

LocalFileHandle::~LocalFileHandle() {
    Status status = Sync();
    if (!status.ok()) {
        return;
    }

    if (fd_ == -1) {
        String error_message = fmt::format("File was closed before or not open");
        UnrecoverableError(error_message);
    }

    i32 ret = close(fd_);
    if (ret == -1) {
        String error_message = fmt::format("Close file: {}, error: {}", path_, strerror(errno));
        UnrecoverableError(error_message);
    }

    fd_ = -1;
    path_.clear();
    access_mode_ = FileAccessMode::kInvalid;

#ifdef hybridsearch_DEBUG
    GlobalResourceUsage::DecrObjectCount("LocalFileHandle");
#endif
}

Status LocalFileHandle::Close() {
    Status status = Sync();
    if (!status.ok()) {
        return status;
    }

    if (fd_ == -1) {
        String error_message = fmt::format("File was closed before");
        UnrecoverableError(error_message);
    }

    i32 ret = close(fd_);
    if (ret == -1) {
        String error_message = fmt::format("Close file: {}, error: {}", path_, strerror(errno));
        UnrecoverableError(error_message);
    }

    fd_ = -1;
    path_.clear();
    access_mode_ = FileAccessMode::kInvalid;
    return Status::OK();
}

Status LocalFileHandle::Append(const void *buffer, u64 nbytes) {
    if (access_mode_ != FileAccessMode::kWrite) {
        String error_message = fmt::format("File: {} isn't open.", path_);
        UnrecoverableError(error_message);
    }
    i64 written = 0;
    while (written < (i64)nbytes) {
        i64 write_count = write(fd_, (char *)buffer + written, nbytes - written);
        if (write_count == -1) {
            String error_message = fmt::format("Can't write file: {}: {}. fd: {}", path_, strerror(errno), fd_);
            UnrecoverableError(error_message);
        }
        written += write_count;
    }
    return Status::OK();
}

Status LocalFileHandle::Append(const String &buffer, u64 nbytes) { return Append(buffer.data(), nbytes); }

Tuple<SizeT, Status> LocalFileHandle::Read(void *buffer, u64 nbytes) {
    i64 read_n = 0;
    while (read_n < (i64)nbytes) {
        SizeT a = nbytes - read_n;
        i64 read_count = read(fd_, (char *)buffer + read_n, a);
        if (read_count == 0) {
            break;
        }
        if (read_count == -1) {
            String error_message = fmt::format("Can't read file: {}: {}", path_, strerror(errno));
            UnrecoverableError(error_message);
        }
        read_n += read_count;
    }
    return {read_n, Status::OK()};
}

Tuple<SizeT, Status> LocalFileHandle::Read(String &buffer, u64 nbytes) {
    i64 read_n = 0;
    while (read_n < (i64)nbytes) {
        SizeT a = nbytes - read_n;
        i64 read_count = read(fd_, buffer.data() + read_n, a);
        if (read_count == 0) {
            break;
        }
        if (read_count == -1) {
            String error_message = fmt::format("Can't read file: {}: {}", path_, strerror(errno));
            UnrecoverableError(error_message);
        }
        read_n += read_count;
    }
    return {read_n, Status::OK()};
}

Status LocalFileHandle::Seek(u64 nbytes) {
    if ((off_t)-1 == lseek(fd_, nbytes, SEEK_SET)) {
        String error_message = fmt::format("Can't seek file: {}: {}", path_, strerror(errno));
        UnrecoverableError(error_message);
    }
    return Status::OK();
}

i64 LocalFileHandle::FileSize() {
    struct stat s{};
    if (fstat(fd_, &s) == -1) {
        return -1;
    }
    return s.st_size;
}

Tuple<char *, SizeT, Status> LocalFileHandle::MmapRead(const String &name) { return {nullptr, 0, Status::OK()}; }

Status LocalFileHandle::Unmmap(const String &name) { return Status::OK(); }

Status LocalFileHandle::Sync() {
    if (access_mode_ != FileAccessMode::kWrite) {
        return Status::OK();
    }

    fsync(fd_);
    return Status::OK();
}

} // namespace hybridsearch
