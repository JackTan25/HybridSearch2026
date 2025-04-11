

module;

export module version_file_worker;

import stl;
import file_worker;
import file_worker_type;
import buffer_obj;
import persistence_manager;

namespace hybridsearch {

export struct VersionFileWorkerSaveCtx : public FileWorkerSaveCtx {
    VersionFileWorkerSaveCtx(TxnTimeStamp checkpoint_ts) : checkpoint_ts_(checkpoint_ts) {}

    TxnTimeStamp checkpoint_ts_{};
};

export class VersionFileWorker : public FileWorker {
public:
    explicit VersionFileWorker(SharedPtr<String> data_dir,
                               SharedPtr<String> temp_dir,
                               SharedPtr<String> file_dir,
                               SharedPtr<String> file_name,
                               SizeT capacity,
                               PersistenceManager *persistence_manager);

    virtual ~VersionFileWorker() override;

public:
    void AllocateInMemory() override;

    void FreeInMemory() override;

    SizeT GetMemoryCost() const override;

    FileWorkerType Type() const override { return FileWorkerType::kVersionDataFile; }

protected:
    bool WriteToFileImpl(bool to_spill, bool &prepare_success, const FileWorkerSaveCtx &ctx) override;

    void ReadFromFileImpl(SizeT file_size, bool from_spill) override;

private:
    SizeT capacity_{};
};

} // namespace hybridsearch