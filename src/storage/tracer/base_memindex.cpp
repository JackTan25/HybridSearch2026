

module;

module base_memindex;

import stl;
import memindex_tracer;
import hybridsearch_context;

namespace hybridsearch {

void BaseMemIndex::IncreaseMemoryUsageBase(SizeT mem) {
    auto *memindex_tracer = hybridsearchContext::instance().storage()->memindex_tracer();
    memindex_tracer->IncreaseMemoryUsage(mem);
}

void BaseMemIndex::DecreaseMemoryUsageBase(SizeT mem) {
    auto *memindex_tracer = hybridsearchContext::instance().storage()->memindex_tracer();
    memindex_tracer->DecreaseMemUsed(mem);
}

} // namespace hybridsearch