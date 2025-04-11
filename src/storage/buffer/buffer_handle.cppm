

module;

import stl;
import third_party;

export module buffer_handle;

namespace hybridsearch {

class BufferObj;
class FileWorker;

export class BufferHandle {

public:
    BufferHandle() = default;

    explicit BufferHandle(BufferObj *buffer_handle, void *data);

    BufferHandle(const BufferHandle &other);

    BufferHandle &operator=(const BufferHandle &other);

    BufferHandle(BufferHandle &&other);

    BufferHandle &operator=(BufferHandle &&other);

    ~BufferHandle();

public:
    [[nodiscard]] const void *GetData() const;

    [[nodiscard]] void *GetDataMut();

    const FileWorker *GetFileWorker() const;

    FileWorker *GetFileWorkerMut();

private:
    BufferObj *buffer_obj_{};

    void *data_{};
};

} // namespace hybridsearch