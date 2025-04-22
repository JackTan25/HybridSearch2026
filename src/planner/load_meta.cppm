

module;

export module load_meta;

import stl;

import column_binding;
import internal_types;
import data_type;

namespace hybridsearch {

export struct LoadMeta {
    ColumnBinding binding_{};
    SizeT index_{};
    SharedPtr<DataType> type_{};
    String column_name_{};
};

} // namespace hybridsearch