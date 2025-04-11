

module;

export module raw_file_worker;

import stl;
import file_worker;
import file_worker_type;
import persistence_manager;

namespace hybridsearch {

// RawFileWorker is FileWorker for raw file:
// - There's no file header nor footer.
// - The buffer size is just the file size.
// - The file size is consistant since creation.
export class RawFileWorker : public FileWorker {
public:
    explicit RawFileWorker(SharedPtr<String> data_dir,
                           SharedPtr<String> temp_dir,
                           SharedPtr<String> file_dir,
                           SharedPtr<String> file_name,
                           u32 file_size,
                           PersistenceManager *persistence_manager);

    virtual ~RawFileWorker() override;

public:
    void AllocateInMemory() override;

    void FreeInMemory() override;

    SizeT GetMemoryCost() const override { return buffer_size_; }

    FileWorkerType Type() const override { return FileWorkerType::kRawFile; }

protected:
    bool WriteToFileImpl(bool to_spill, bool &prepare_success, const FileWorkerSaveCtx &ctx) override;

    void ReadFromFileImpl(SizeT file_size, bool from_spill) override;

private:
    SizeT buffer_size_;
};
} // namespace hybridsearch