

module;

export module persist_result_handler;

import stl;
import persistence_manager;
import global_resource_usage;

namespace hybridsearch {

export class PersistResultHandler {
public:
    PersistResultHandler() : pm_(nullptr) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("PersistResultHandler");
#endif
    }

    PersistResultHandler(PersistenceManager *pm) : pm_(pm) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("PersistResultHandler");
#endif
    }

    ~PersistResultHandler() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("PersistResultHandler");
#endif
    }

    void HandleWriteResult(const PersistWriteResult &result);

    ObjAddr HandleReadResult(const PersistReadResult &result);

private:
    PersistenceManager *pm_;
};

} // namespace hybridsearch
