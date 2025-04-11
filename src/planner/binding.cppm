

module;

export module binding;

import stl;
import block_index;
import meta_info;
import internal_types;
import data_type;

namespace hybridsearch {

export enum class BindingType { kInvalid, kTable, kSubquery, kCTE, kView };

export class Binding {
public:
    Binding() = default;

    virtual ~Binding() = default;

    static SharedPtr<Binding> MakeBinding(BindingType binding_type,
                                          const String &name,
                                          u64 table_index,
                                          SharedPtr<Vector<SharedPtr<DataType>>> column_types,
                                          SharedPtr<Vector<String>> column_names);

    static SharedPtr<Binding> MakeBinding(BindingType binding_type,
                                          const String &name,
                                          u64 table_index,
                                          SharedPtr<TableInfo> table_info,
                                          SharedPtr<Vector<SharedPtr<DataType>>> column_types,
                                          SharedPtr<Vector<String>> column_names,
                                          SharedPtr<BlockIndex> block_index);

    // Binding type
    BindingType binding_type_{BindingType::kInvalid};

    // Binding table alias or name
    String table_name_{};

    u64 table_index_{};

    // Node id of the corresponding logical node
    //    i64 logical_node_id_{-1};

    // ptr to the corresponding logical node.
    //    SharedPtr<LogicalNode> logical_node_ptr_{nullptr};

    // if the binding is table, this is the table_ptr
    SharedPtr<TableInfo> table_info_{nullptr};

    SharedPtr<BlockIndex> block_index_{};

    // Column types of the binding
    SharedPtr<Vector<SharedPtr<DataType>>> column_types_{};

    // Column names of the binding
    SharedPtr<Vector<String>> column_names_{};

    // Column name -> column index mapping
    HashMap<String, i64> name2index_{};
};

// class TableBinding: public GenericBinding {
// public:
//
// };

} // namespace hybridsearch
