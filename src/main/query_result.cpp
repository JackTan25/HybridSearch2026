

module;

#include <sstream>

import stl;
import logical_node_type;
import data_block;

module query_result;

namespace hybridsearch {

String QueryResult::ToString() const {
    std::stringstream ss;

    switch (root_operator_type_) {
        case LogicalNodeType::kInsert: {
            return "INSERT 0 1";
        }
        case LogicalNodeType::kUpdate: {
            return "UPDATE 0 1";
        }
        case LogicalNodeType::kDelete: {
            return "DELETE 0 1";
        }
        default: {
            ss << std::endl;
        }
    }

    SizeT column_count = result_table_->ColumnCount();
    for (SizeT idx = 0; idx < column_count; ++idx) {
        String end;
        if (idx != column_count - 1) {
            end = " ";
        }
        ss << result_table_->GetColumnNameById(idx) << end;
    }
    ss << std::endl;

    // Get Block count
    SizeT block_count = result_table_->DataBlockCount();

    // Iterate all blocks
    for (SizeT idx = 0; idx < block_count; ++idx) {
        // Get current block
        DataBlock *current_block = result_table_->GetDataBlockById(idx).get();
        ss << current_block->ToString();
    }

    return ss.str();
}

} // namespace hybridsearch