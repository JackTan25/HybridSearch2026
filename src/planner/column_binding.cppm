

module;

#include <functional>

import stl;

export module column_binding;

namespace hybridsearch {

export struct ColumnBinding {

    ColumnBinding() = default;

    explicit ColumnBinding(SizeT tbl_idx, SizeT col_idx) : table_idx(tbl_idx), column_idx(col_idx) {}

    friend auto operator<=>(const ColumnBinding &lhs, const ColumnBinding &rhs) = default;

    SizeT table_idx{};
    SizeT column_idx{};
};

} // namespace hybridsearch

namespace std {

template <>
class hash<hybridsearch::ColumnBinding> {
public:
    std::size_t operator()(const hybridsearch::ColumnBinding &rhs) const {
        return hash<std::size_t>().operator()(rhs.table_idx) ^ hash<std::size_t>().operator()(rhs.column_idx);
    }
};

} // namespace std
