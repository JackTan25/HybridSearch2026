

module;

export module base_memindex;

import stl;
import memindex_tracer;

namespace hybridsearch {

struct TableIndexEntry;

export class BaseMemIndex : public EnableSharedFromThis<BaseMemIndex> {
public:
    virtual ~BaseMemIndex() = default;

    virtual MemIndexTracerInfo GetInfo() const = 0;

    virtual TableIndexEntry *table_index_entry() const = 0;

protected:
    void IncreaseMemoryUsageBase(SizeT mem);
    void DecreaseMemoryUsageBase(SizeT mem);
};

} // namespace hybridsearch
