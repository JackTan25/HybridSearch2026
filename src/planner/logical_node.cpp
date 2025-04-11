

module;

#include <vector>
module logical_node;

namespace hybridsearch {

Vector<ColumnBinding> LogicalCommonFunctionUsingLoadMeta::GetColumnBindings(const LogicalNode &op) {
    auto bindings = op.left_node()->GetColumnBindings();
    if (auto ptr = op.load_metas(); ptr) {
        for (auto &load_meta : *ptr) {
            bindings.push_back(load_meta.binding_);
        }
    }
    return bindings;
}

SharedPtr<Vector<String>> LogicalCommonFunctionUsingLoadMeta::GetOutputNames(const LogicalNode &op) {
    auto output_names = op.left_node()->GetOutputNames();
    if (auto ptr = op.load_metas(); ptr) {
        for (auto &load_meta : *ptr) {
            output_names->insert(output_names->begin() + load_meta.index_, load_meta.column_name_);
        }
    }
    return output_names;
}

SharedPtr<Vector<SharedPtr<DataType>>> LogicalCommonFunctionUsingLoadMeta::GetOutputTypes(const LogicalNode &op) {
    auto output_types = op.left_node()->GetOutputTypes();
    if (auto ptr = op.load_metas(); ptr) {
        for (auto &load_meta : *ptr) {
            output_types->insert(output_types->begin() + load_meta.index_, load_meta.type_);
        }
    }
    return output_types;
}

} // namespace hybridsearch
