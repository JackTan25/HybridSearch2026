

module;

export module multivector_util;

import stl;
import third_party;

namespace hybridsearch {

export template <typename ElementT>
class MultiVectorRef {
    Span<const char> raw_data_ = {};
    SizeT embedding_num_ = 0;

public:
    using ElementType = ElementT;
    MultiVectorRef &operator=(const Pair<Span<const char>, SizeT> &data) {
        raw_data_ = data.first;
        embedding_num_ = data.second;
        return *this;
    }
    [[nodiscard]] Span<const char> raw_data() const { return raw_data_; }
    [[nodiscard]] SizeT embedding_num() const { return embedding_num_; }
};

} // namespace hybridsearch