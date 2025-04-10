

module;

export module memindex_tracer;

import stl;
import bg_task;
import third_party;
import logger;
import global_resource_usage;

namespace hybridsearch {

class BaseMemIndex;
class Txn;
struct Catalog;
class TxnManager;

export struct MemIndexTracerInfo {
public:
    MemIndexTracerInfo(SharedPtr<String> index_name, SharedPtr<String> table_name, SharedPtr<String> db_name, SizeT mem_used, SizeT row_count)
        : index_name_(std::move(index_name)), table_name_(std::move(table_name)), db_name_(std::move(db_name)), mem_used_(mem_used),
          row_count_(row_count) {}
    SharedPtr<String> index_name_;
    SharedPtr<String> table_name_;
    SharedPtr<String> db_name_;
    SizeT mem_used_;
    SizeT row_count_;
};

export class MemIndexTracer {
public:
    MemIndexTracer(SizeT index_memory_limit) : index_memory_limit_(index_memory_limit) {}

    virtual ~MemIndexTracer() = default;

    void DecreaseMemUsed(SizeT mem_used);

    void IncreaseMemoryUsage(SizeT usage);

    void DumpDone(SizeT actual_dump_size, BaseMemIndex *mem_index);

    void DumpFail(BaseMemIndex *mem_index);

    Vector<MemIndexTracerInfo> GetMemIndexTracerInfo(Txn *txn);

    Vector<BaseMemIndex *> GetUndumpedMemIndexes(Txn *txn);

    virtual void TriggerDump(UniquePtr<DumpIndexTask> task) = 0;

    SizeT cur_index_memory() const { return cur_index_memory_.load(); }

protected:
    virtual Txn *GetTxn() = 0;

    virtual Vector<BaseMemIndex *> GetAllMemIndexes(Txn *txn) = 0;

    using MemIndexMapIter = HashSet<BaseMemIndex *>::iterator;

    UniquePtr<DumpIndexTask> MakeDumpTask();

    static SizeT ChooseDump(const Vector<BaseMemIndex *> &mem_indexes);

protected:
    std::mutex mtx_;

    const SizeT index_memory_limit_;
    Atomic<SizeT> cur_index_memory_ = 0;

    HashMap<BaseMemIndex *, SizeT> proposed_dump_{};
    Atomic<SizeT> acc_proposed_dump_ = 0;
};

inline void MemIndexTracer::IncreaseMemoryUsage(SizeT add) {
    // LOG_TRACE(fmt::format("Add mem used: {}, mem index limit: {}", add, index_memory_limit_));
    if (add == 0 || index_memory_limit_ == 0) {
        return;
    }
    SizeT old_index_memory = cur_index_memory_.fetch_add(add);
    if (SizeT new_index_memory = old_index_memory + add; new_index_memory > index_memory_limit_) {
        if (new_index_memory > index_memory_limit_ + acc_proposed_dump_.load()) {
            LOG_TRACE(fmt::format("acc_proposed_dump_ = {}", acc_proposed_dump_.load()));
            auto dump_task = MakeDumpTask();
            if (dump_task.get() != nullptr) {
                LOG_TRACE(fmt::format("Dump triggered!"));
                TriggerDump(std::move(dump_task));
            }
        }
    }
}

export class BGMemIndexTracer : public MemIndexTracer {
public:
    BGMemIndexTracer(SizeT index_memory_limit, Catalog *catalog, TxnManager *txn_mgr)
        : MemIndexTracer(index_memory_limit), catalog_(catalog), txn_mgr_(txn_mgr) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("BGMemIndexTracer");
#endif
    }

    ~BGMemIndexTracer() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("BGMemIndexTracer");
#endif
    }

    void TriggerDump(UniquePtr<DumpIndexTask> task) override;

protected:
    Txn *GetTxn() override;

    Vector<BaseMemIndex *> GetAllMemIndexes(Txn *txn) override;

private:
    Catalog *catalog_;
    TxnManager *txn_mgr_;
};

} // namespace hybridsearch
