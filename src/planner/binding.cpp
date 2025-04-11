

module;

module binding;

import stl;
import block_index;
import hybridsearch_exception;
import logger;

namespace hybridsearch {

SharedPtr<Binding> Binding::MakeBinding(BindingType binding_type,
                                        const String &name,
                                        u64 table_index,
                                        SharedPtr<Vector<SharedPtr<DataType>>> column_types,
                                        SharedPtr<Vector<String>> column_names) {
    SharedPtr<Binding> binding = MakeShared<Binding>();
    binding->binding_type_ = binding_type;
    binding->table_name_ = name;
    binding->table_index_ = table_index;

    SizeT column_count = column_names->size();
    if (column_count != column_types->size()) {
        String error_message = "Make binding error: column size isn't valid.";
        UnrecoverableError(error_message);
    }
    binding->column_types_ = std::move(column_types);
    binding->column_names_ = std::move(column_names);

    for (SizeT idx = 0; idx < column_count; ++idx) {
        binding->name2index_[binding->column_names_->at(idx)] = static_cast<i64>(idx);
    }
    return binding;
}

SharedPtr<Binding> Binding::MakeBinding(BindingType binding_type,
                                        const String &binding_alias,
                                        u64 table_index,
                                        SharedPtr<TableInfo> table_info,
                                        SharedPtr<Vector<SharedPtr<DataType>>> column_types,
                                        SharedPtr<Vector<String>> column_names,
                                        SharedPtr<BlockIndex> block_index) {
    auto binding = MakeBinding(binding_type, binding_alias, table_index, std::move(column_types), std::move(column_names));
    binding->table_info_ = table_info;
    binding->block_index_ = std::move(block_index);
    //    binding->logical_node_ptr_ = std::move(logical_node_ptr);
    //    binding->logical_node_id_ = logical_node_id;
    return binding;
}

} // namespace hybridsearch