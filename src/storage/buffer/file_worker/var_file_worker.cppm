

module;

export module var_file_worker;

import stl;
import file_worker;
import file_worker_type;
import buffer_obj;
import persistence_manager;

namespace hybridsearch {

export class VarFileWorker : public FileWorker {
public:
    explicit VarFileWorker(SharedPtr<String> data_dir,
                           SharedPtr<String> temp_dir,
                           SharedPtr<String> file_dir,
                           SharedPtr<String> file_name,
                           SizeT buffer_size,
                           PersistenceManager *persistence_manager);

    virtual ~VarFileWorker() override;

public:
    void SetBufferObj(BufferObj *buffer_obj) { buffer_obj_ = buffer_obj; }

    void AllocateInMemory() override;

    void FreeInMemory() override;

    SizeT GetMemoryCost() const override;

    SizeT GetBufferSize() const { return buffer_size_; }

    void SetBufferSize(SizeT buffer_size) { buffer_size_ = buffer_size; }

    FileWorkerType Type() const override { return FileWorkerType::kVarFile; }

protected:
    bool WriteToFileImpl(bool to_spill, bool &prepare_success, const FileWorkerSaveCtx &ctx) override;

    void ReadFromFileImpl(SizeT file_size, bool from_spill) override;

    bool ReadFromMmapImpl(const void *ptr, SizeT size) override;

    void FreeFromMmapImpl() override;

private:
    SizeT buffer_size_ = 0;
    BufferObj *buffer_obj_ = nullptr;
};

} // namespace hybridsearch
