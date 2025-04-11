

module;

export module ivf_index_file_worker;

import stl;
import index_file_worker;
import file_worker;
import index_base;
import column_def;
import file_worker_type;
import persistence_manager;

namespace hybridsearch {

export class IVFIndexFileWorker final : public IndexFileWorker {
public:
    explicit IVFIndexFileWorker(SharedPtr<String> data_dir,
                                SharedPtr<String> temp_dir,
                                SharedPtr<String> file_dir,
                                SharedPtr<String> file_name,
                                SharedPtr<IndexBase> index_base,
                                SharedPtr<ColumnDef> column_def,
                                PersistenceManager *persistence_manager)
        : IndexFileWorker(std::move(data_dir),
                          std::move(temp_dir),
                          std::move(file_dir),
                          std::move(file_name),
                          std::move(index_base),
                          std::move(column_def),
                          persistence_manager) {}

    ~IVFIndexFileWorker() override;

    void AllocateInMemory() override;

    void FreeInMemory() override;

    FileWorkerType Type() const override { return FileWorkerType::kIVFIndexFile; }

protected:
    bool WriteToFileImpl(bool to_spill, bool &prepare_success, const FileWorkerSaveCtx &ctx) override;

    void ReadFromFileImpl(SizeT file_size, bool from_spill) override;
};

} // namespace hybridsearch
