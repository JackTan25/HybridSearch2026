

module;

export module file_writer;

import stl;
import local_file_handle;

namespace hybridsearch {

export class FileWriter {
public:
    explicit FileWriter(const String &path, SizeT buffer_size);

    String path_{};
    UniquePtr<char_t[]> data_{};
    SizeT offset_{};
    SizeT total_written_{};
    SizeT buffer_size_{};
    UniquePtr<LocalFileHandle> file_handle_{};

public:
    void WriteByte(const u8 b);

    void WriteShort(const i16 i);

    void WriteInt(const i32 i);

    void WriteVInt(const i32 i);

    void WriteLong(const i64 i);

    void WriteVLong(const i64 i);

    void Write(const char_t *buffer, SizeT bytes_count);

    void Sync();

    void Flush();

    i64 GetFileSize();

    SizeT TotalWrittenBytes() const;
};

} // namespace hybridsearch
