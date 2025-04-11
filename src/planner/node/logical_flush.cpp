

module;

#include <sstream>

module logical_flush;

import stl;

import column_binding;
import internal_types;

namespace hybridsearch {

Vector<ColumnBinding> LogicalFlush::GetColumnBindings() const { return {}; }

SharedPtr<Vector<String>> LogicalFlush::GetOutputNames() const { return MakeShared<Vector<String>>(); }

SharedPtr<Vector<SharedPtr<DataType>>> LogicalFlush::GetOutputTypes() const { return MakeShared<Vector<SharedPtr<DataType>>>(); }

String LogicalFlush::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }

    switch (flush_type_) {
        case FlushType::kDelta:
            ss << String(space, ' ') << "-> "
               << "Flush Delta: ";
            break;
        case FlushType::kData:
            ss << String(space, ' ') << "-> "
               << "Flush Data: ";
            break;
        case FlushType::kLog:
            ss << String(space, ' ') << "-> "
               << "Flush Log: ";
            break;
        case FlushType::kBuffer:
            ss << String(space, ' ') << "-> "
               << "Flush Buffer: ";
            break;
    }
    space += arrow_str.size();

    return ss.str();
}

} // namespace hybridsearch
