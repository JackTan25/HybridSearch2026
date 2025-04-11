

module;

export module local_file_handle;

import stl;
import status;
import global_resource_usage;

namespace hybridsearch {

export enum class FileAccessMode { kWrite, kRead, kMmapRead, kInvalid };

export class LocalFileHandle {
public:
    LocalFileHandle(i32 fd, const String &path, FileAccessMode file_access_mode) : fd_(fd), path_(path), access_mode_(file_access_mode) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("LocalFileHandle");
#endif
    }
    ~LocalFileHandle();

    Status Append(const void *buffer, u64 nbytes);
    Status Append(const String &buffer, u64 nbytes);
    Tuple<SizeT, Status> Read(void *buffer, u64 nbytes);
    Tuple<SizeT, Status> Read(String &buffer, u64 nbytes);
    Status Seek(u64 nbytes);
    i64 FileSize();
    Tuple<char *, SizeT, Status> MmapRead(const String &name);
    Status Unmmap(const String &name);
    Status Sync();

public:
    i32 FileDescriptor() const { return fd_; }

    String Path() const { return path_; }

private:
    Status Close();

private:
    i32 fd_{-1};
    String path_{};
    FileAccessMode access_mode_{FileAccessMode::kInvalid};
};

} // namespace hybridsearch