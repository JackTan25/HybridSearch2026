

module;

export module task_result;

import stl;
import data_type;
import data_block;
import internal_types;

namespace hybridsearch {

export enum class TaskResultType { kInvalid };

export class TaskResult {
public:
    explicit TaskResult(TaskResultType task_result_type) : task_result_type_(task_result_type) {}

    void Init(const Vector<SharedPtr<DataType>> &types, SizeT capacity);

    void Seal();

private:
    UniquePtr<DataBlock> data_{};
    TaskResultType task_result_type_{TaskResultType::kInvalid};
};

} // namespace hybridsearch
