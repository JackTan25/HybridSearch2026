

module;

export module index_file_worker;

import index_base;
import file_worker;
import stl;
import column_def;
import file_worker_type;
import persistence_manager;

namespace hybridsearch {

export class IndexFileWorker : public FileWorker {
protected:
    SharedPtr<ColumnDef> column_def_{};
    SharedPtr<IndexBase> index_base_{};

public:
    explicit IndexFileWorker(SharedPtr<String> data_dir,
                             SharedPtr<String> temp_dir,
                             SharedPtr<String> file_dir,
                             SharedPtr<String> file_name,
                             SharedPtr<IndexBase> index_base,
                             SharedPtr<ColumnDef> column_def,
                             PersistenceManager *persistence_manager)
        : FileWorker(std::move(data_dir), std::move(temp_dir), std::move(file_dir), std::move(file_name), persistence_manager),
          column_def_(std::move(column_def)), index_base_(std::move(index_base)) {}

    SizeT GetMemoryCost() const override { return 0; }

    FileWorkerType Type() const override { return FileWorkerType::kIndexFile; }

    ~IndexFileWorker() override = default;

    void SetIndexDef(SharedPtr<IndexBase> index_base) { index_base_ = std::move(index_base); }
};

} // namespace hybridsearch