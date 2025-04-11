

module;

import stl;
import data_block;

module task_result;

namespace hybridsearch {

void TaskResult::Init(const Vector<SharedPtr<DataType>> &types, SizeT capacity) {
    data_ = MakeUnique<DataBlock>();
    data_->Init(types, capacity);
}

void TaskResult::Seal() { data_->Finalize(); }

} // namespace hybridsearch