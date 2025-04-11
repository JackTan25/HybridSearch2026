

module;

export module invert_task;

import stl;
import column_vector;
import internal_types;

namespace hybridsearch {

export struct BatchInvertTask {
public:
    BatchInvertTask(u64 task_seq, SharedPtr<ColumnVector> column_vector, u32 row_offset, u32 row_count, u32 start_doc_id)
        : task_seq_(task_seq), column_vector_(column_vector), row_offset_(row_offset), row_count_(row_count), start_doc_id_(start_doc_id) {}

    u64 task_seq_;
    SharedPtr<ColumnVector> column_vector_;
    u32 row_offset_;
    u32 row_count_;
    u32 start_doc_id_;
};
} // namespace hybridsearch
