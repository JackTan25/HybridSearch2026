

module;

import stl;
import function_data;
import table_function;
import global_block_id;
import block_index;

export module table_scan_function_data;

namespace hybridsearch {

export class TableScanFunctionData : public TableFunctionData {
public:
    TableScanFunctionData(const BlockIndex *block_index, const SharedPtr<Vector<GlobalBlockID>> &global_block_ids, const Vector<SizeT> &column_ids)
        : block_index_(block_index), global_block_ids_(global_block_ids), column_ids_(column_ids) {}

    const BlockIndex *block_index_{};
    const SharedPtr<Vector<GlobalBlockID>> &global_block_ids_{};
    const Vector<SizeT> &column_ids_{};

    u64 current_block_ids_idx_{0};
    SizeT current_read_offset_{0};
};

} // namespace hybridsearch
