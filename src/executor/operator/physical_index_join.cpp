

module;

#include <string>
import stl;
import query_context;
import operator_state;

module physical_index_join;

namespace hybridsearch {

void PhysicalIndexJoin::Init(QueryContext* query_context) {}

bool PhysicalIndexJoin::Execute(QueryContext *, OperatorState *) { return true; }

SharedPtr<Vector<String>> PhysicalIndexJoin::GetOutputNames() const {
    SharedPtr<Vector<String>> result = MakeShared<Vector<String>>();
    SharedPtr<Vector<String>> left_output_names = left_->GetOutputNames();
    SharedPtr<Vector<String>> right_output_names = right_->GetOutputNames();

    result->reserve(left_output_names->size() + right_output_names->size());
    for (auto &name_str : *left_output_names) {
        result->emplace_back(name_str);
    }

    for (auto &name_str : *right_output_names) {
        result->emplace_back(name_str);
    }

    return result;
}

SharedPtr<Vector<SharedPtr<DataType>>> PhysicalIndexJoin::GetOutputTypes() const {
    SharedPtr<Vector<SharedPtr<DataType>>> result = MakeShared<Vector<SharedPtr<DataType>>>();
    SharedPtr<Vector<SharedPtr<DataType>>> left_output_types = left_->GetOutputTypes();
    SharedPtr<Vector<SharedPtr<DataType>>> right_output_types = right_->GetOutputTypes();

    result->reserve(left_output_types->size() + right_output_types->size());
    for (auto &left_type : *left_output_types) {
        result->emplace_back(left_type);
    }

    for (auto &right_type : *right_output_types) {
        result->emplace_back(right_type);
    }

    return result;
}

} // namespace hybridsearch
