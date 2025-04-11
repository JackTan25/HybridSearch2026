

module;

module physical_filter;

import stl;
import txn;
import query_context;
import table_def;
import data_table;

import physical_operator_type;
import operator_state;
import expression_state;
import expression_selector;
import data_block;
import logger;
import third_party;

import hybridsearch_exception;

namespace hybridsearch {

void PhysicalFilter::Init(QueryContext* query_context) {
    //    executor.Init({condition_});
    //    input_table_ = left_->output();
    //    ExecutorAssert(input_table_ != nullptr, "No input.");
    //
    //    SizeT column_count = input_table_->ColumnCount();
    //    Vector<SharedPtr<ColumnDef>> columns;
    //    columns.reserve(column_count);
    //    for(SizeT idx = 0; idx < column_count; ++ idx) {
    //        DataType col_type = input_table_->GetColumnTypeById(idx);
    //        String col_name = input_table_->GetColumnNameById(idx);
    //
    //        SharedPtr<ColumnDef> col_def = ColumnDef::Make(col_name, idx, col_type, Set<ConstrainType>());
    //        columns.emplace_back(col_def);
    //    }
    //
    //    SharedPtr<TableDef> table_def = TableDef::Make("filter", columns, false);
    //
    //    output_ = DataTable::Make(table_def, TableType::kIntermediate);
}

bool PhysicalFilter::Execute(QueryContext *, OperatorState *operator_state) {
    auto *prev_op_state = operator_state->prev_op_state_;
    auto *filter_operator_state = static_cast<FilterOperatorState *>(operator_state);

    if (prev_op_state->data_block_array_.empty()) {
        String error_message = "No input data array from input";
        UnrecoverableError(error_message);
    }

    SizeT input_block_count = prev_op_state->data_block_array_.size();

    for (SizeT block_idx = 0; block_idx < input_block_count; ++block_idx) {

        // create uninitialized data block for output
        UniquePtr<DataBlock> data_block = DataBlock::MakeUniquePtr();
        DataBlock *output_data_block = data_block.get();
        operator_state->data_block_array_.emplace_back(std::move(data_block));

        SharedPtr<ExpressionState> condition_state = ExpressionState::CreateState(condition_);
        DataBlock *input_data_block = prev_op_state->data_block_array_[block_idx].get();

        // selector contains a pointer to input data, which should not be shared by multiple tasks
        ExpressionSelector selector;
        SizeT selected_count = selector.Select(condition_, condition_state, input_data_block, output_data_block, input_data_block->row_count());

        LOG_TRACE(fmt::format("{} rows after filter", selected_count));
    }

    // Clean input data block array;
    prev_op_state->data_block_array_.clear();
    if (prev_op_state->Complete()) {
        filter_operator_state->SetComplete();
    }
    return true;
}

} // namespace hybridsearch
