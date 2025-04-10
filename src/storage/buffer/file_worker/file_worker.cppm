

module;

#include <cassert>

export module file_worker;

import stl;
import local_file_handle;
import third_party;
import file_worker_type;
import persistence_manager;
import global_resource_usage;
import hybridsearch_exception;
import defer_op;

namespace hybridsearch {

export struct FileWorkerSaveCtx {};

export class FileWorker {
public:
    // spill_dir_ is not init here
    explicit FileWorker(SharedPtr<String> data_dir,
                        SharedPtr<String> temp_dir,
                        SharedPtr<String> file_dir,
                        SharedPtr<String> file_name,
                        PersistenceManager *persistence_manager)
        : data_dir_(std::move(data_dir)), temp_dir_(std::move(temp_dir)), file_dir_(std::move(file_dir)), file_name_(std::move(file_name)),
          persistence_manager_(persistence_manager) {
        assert(!std::filesystem::path(*file_dir_).is_absolute());
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("FileWorker");
#endif
    }

    // No destruct here
    virtual ~FileWorker();

public:
    [[nodiscard]] bool WriteToFile(bool to_spill, const FileWorkerSaveCtx &ctx = {});

    void ReadFromFile(bool from_spill);

    void MoveFile();

    virtual void AllocateInMemory() = 0;

    virtual void FreeInMemory() = 0;

    virtual SizeT GetMemoryCost() const = 0;

    virtual FileWorkerType Type() const = 0;

    void *GetData() const { return data_; }

    // Get absolute file path. As key of buffer handle.
    String GetFilePath() const;

    void CleanupFile() const;

    void CleanupTempFile() const;

protected:
    virtual bool WriteToFileImpl(bool to_spill, bool &prepare_success, const FileWorkerSaveCtx &ctx = {}) = 0;

    virtual void ReadFromFileImpl(SizeT file_size, bool from_spill) = 0;

    String ChooseFileDir(bool spill) const;

    Pair<Optional<DeferFn<std::function<void()>>>, String> GetFilePathInner(bool spill);

public:
    const SharedPtr<String> data_dir_{};
    const SharedPtr<String> temp_dir_{};
    const SharedPtr<String> file_dir_{};
    const SharedPtr<String> file_name_{};
    PersistenceManager *persistence_manager_{};
    ObjAddr obj_addr_{};

protected:
    void *data_{nullptr};
    UniquePtr<LocalFileHandle> file_handle_{nullptr};

public:
    void *GetMmapData() const { return mmap_data_; }

    void Mmap();

    void Munmap();

    void MmapNotNeed();

protected:
    virtual bool ReadFromMmapImpl([[maybe_unused]] const void *ptr, [[maybe_unused]] SizeT size) {
        UnrecoverableError("Not implemented");
        return false;
    }

    virtual void FreeFromMmapImpl() { UnrecoverableError("Not implemented"); }

protected:
    u8 *mmap_addr_{nullptr};
    u8 *mmap_data_{nullptr};
};
} // namespace hybridsearch