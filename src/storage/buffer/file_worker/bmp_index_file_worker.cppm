

module;

export module bmp_index_file_worker;

import stl;
import index_file_worker;
import file_worker;
import index_base;
import column_def;
import file_worker_type;
import sparse_info;
import persistence_manager;

namespace hybridsearch {

export class BMPIndexFileWorker final : public IndexFileWorker {
public:
    explicit BMPIndexFileWorker(SharedPtr<String> data_dir,
                                SharedPtr<String> temp_dir,
                                SharedPtr<String> file_dir,
                                SharedPtr<String> file_name,
                                SharedPtr<IndexBase> index_base,
                                SharedPtr<ColumnDef> column_def,
                                PersistenceManager *persistence_manager,
                                SizeT index_size = 0);

    ~BMPIndexFileWorker() override;

public:
    void AllocateInMemory() override;

    void FreeInMemory() override;

    FileWorkerType Type() const override { return FileWorkerType::kBMPIndexFile; }

    SizeT GetMemoryCost() const override { return index_size_; }

protected:
    bool WriteToFileImpl(bool to_spill, bool &prepare_success, const FileWorkerSaveCtx &ctx) override;

    void ReadFromFileImpl(SizeT file_size, bool from_spill) override;

    bool ReadFromMmapImpl(const void *ptr, SizeT size) override;

    void FreeFromMmapImpl() override;

private:
    SizeT index_size_{};
};

} // namespace hybridsearch