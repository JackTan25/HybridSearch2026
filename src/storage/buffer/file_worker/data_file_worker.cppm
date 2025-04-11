

module;

export module data_file_worker;

import stl;
import file_worker;
import file_worker_type;
import persistence_manager;

namespace hybridsearch {

export class DataFileWorker : public FileWorker {
public:
    explicit DataFileWorker(SharedPtr<String> data_dir,
                            SharedPtr<String> temp_dir,
                            SharedPtr<String> file_dir,
                            SharedPtr<String> file_name,
                            SizeT buffer_size,
                            PersistenceManager *persistence_manager);

    virtual ~DataFileWorker() override;

public:
    void AllocateInMemory() override;

    void FreeInMemory() override;

    SizeT GetMemoryCost() const override { return buffer_size_; }

    FileWorkerType Type() const override { return FileWorkerType::kDataFile; }

protected:
    bool WriteToFileImpl(bool to_spill, bool &prepare_success, const FileWorkerSaveCtx &ctx) override;

    void ReadFromFileImpl(SizeT file_size, bool from_spill) override;

    bool ReadFromMmapImpl(const void *ptr, SizeT size) override;

    void FreeFromMmapImpl() override;

private:
    const SizeT buffer_size_;
};
} // namespace hybridsearch