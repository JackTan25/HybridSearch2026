

module;

export module secondary_index_file_worker;

import stl;
import index_file_worker;
import file_worker;

import index_base;
import hybridsearch_exception;
import default_values;
import column_def;
import file_worker_type;
import persistence_manager;

namespace hybridsearch {

// pgm index
export class SecondaryIndexFileWorker final : public IndexFileWorker {
public:
    explicit SecondaryIndexFileWorker(SharedPtr<String> data_dir,
                                      SharedPtr<String> temp_dir,
                                      SharedPtr<String> file_dir,
                                      SharedPtr<String> file_name,
                                      SharedPtr<IndexBase> index_base,
                                      SharedPtr<ColumnDef> column_def,
                                      u32 row_count,
                                      PersistenceManager *persistence_manager)
        : IndexFileWorker(std::move(data_dir),
                          std::move(temp_dir),
                          std::move(file_dir),
                          std::move(file_name),
                          std::move(index_base),
                          std::move(column_def),
                          persistence_manager),
          row_count_(row_count) {}

    ~SecondaryIndexFileWorker() override;

    void AllocateInMemory() override;

    void FreeInMemory() override;

    FileWorkerType Type() const override { return FileWorkerType::kSecondaryIndexFile; }

protected:
    bool WriteToFileImpl(bool to_spill, bool &prepare_success, const FileWorkerSaveCtx &ctx) override;

    void ReadFromFileImpl(SizeT file_size, bool from_spill) override;

    const u32 row_count_{};
};

} // namespace hybridsearch
