

module;

module physical_merge_limit;

import stl;
import query_context;
import base_expression;
import load_meta;
import physical_operator_type;
import value_expression;
import physical_limit;
import operator_state;

namespace hybridsearch {

PhysicalMergeLimit::PhysicalMergeLimit(u64 id,
                                       UniquePtr<PhysicalOperator> left,
                                       SharedPtr<BaseTableRef> base_table_ref,
                                       SharedPtr<BaseExpression> limit_expr,
                                       SharedPtr<BaseExpression> offset_expr,
                                       SharedPtr<Vector<LoadMeta>> load_metas)
    : PhysicalOperator(PhysicalOperatorType::kMergeLimit, std::move(left), nullptr, id, std::move(load_metas)),
      base_table_ref_(std::move(base_table_ref)), limit_expr_(std::move(limit_expr)), offset_expr_(std::move(offset_expr)) {
    i64 offset = 0;
    i64 limit = (static_pointer_cast<ValueExpression>(limit_expr_))->GetValue().value_.big_int;

    if (offset_expr_.get() != nullptr) {
        offset = (static_pointer_cast<ValueExpression>(offset_expr_))->GetValue().value_.big_int;
    }
    counter_ = MakeUnique<UnSyncCounter>(offset, limit);
}

void PhysicalMergeLimit::Init(QueryContext* query_context) {}

bool PhysicalMergeLimit::Execute(QueryContext *query_context, OperatorState *operator_state) {
    MergeLimitOperatorState *limit_op_state = (MergeLimitOperatorState *)operator_state;

    if (limit_op_state->input_data_blocks_.empty()) {
        if (counter_->IsLimitOver() || limit_op_state->input_complete_) {
            limit_op_state->SetComplete();
        }
        return true;
    }
    auto result = PhysicalLimit::Execute(query_context, limit_op_state->input_data_blocks_, limit_op_state->data_block_array_, counter_.get(), false);

    if (counter_->IsLimitOver() || limit_op_state->input_complete_) {
        limit_op_state->input_complete_ = true;
        limit_op_state->SetComplete();
    }
    limit_op_state->input_data_blocks_.clear();
    return result;
}

} // namespace hybridsearch
