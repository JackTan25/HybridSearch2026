
module;

import stl;
import global_resource_usage;

export module allocator;

namespace hybridsearch {

export class Allocator {
public:
    static ptr_t allocate(SizeT bytes) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrRawMemCount("Allocator");
#endif
        return new char[bytes];
    }

    static void deallocate(ptr_t ptr) {
        delete[] ptr;
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrRawMemCount("Allocator");
#endif
    }
};

} // namespace hybridsearch