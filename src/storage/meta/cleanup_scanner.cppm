

module;

export module cleanup_scanner;

import stl;
import buffer_manager;
import global_resource_usage;

namespace hybridsearch {

class Catalog;
class EntryInterface;
class MetaInterface;
class BaseEntry;

export class CleanupInfoTracer {
public:
    CleanupInfoTracer() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("CleanupInfoTracer");
#endif
    }

    ~CleanupInfoTracer() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("CleanupInfoTracer");
#endif
    }

    void ResetInfo(TxnTimeStamp cleanup_ts);

    void AddCleanupInfo(String path);

    String GetCleanupInfo() const;

private:
    mutable std::mutex mutex_;
    TxnTimeStamp cleanup_ts_ = 0;

    Vector<String> cleanup_info_;
};

export class CleanupScanner {
public:
    CleanupScanner(Catalog *catalog, TxnTimeStamp visible_ts, BufferManager *buffer_mgr);

    void Scan();

    void Cleanup(CleanupInfoTracer *info_tracer = nullptr) &&;

    void AddEntry(SharedPtr<BaseEntry> entry, bool dropped = true);

    TxnTimeStamp visible_ts() const { return visible_ts_; }

    static void CleanupDir(const String &abs_dir);

private:
    Catalog *const catalog_;

    const TxnTimeStamp visible_ts_;

    BufferManager *buffer_mgr_;

    Vector<Pair<SharedPtr<BaseEntry>, bool>> entries_;
};

} // namespace hybridsearch
