

module;

#include <algorithm>
#include <sstream>
#include <string>

module index_secondary;

import stl;
import status;
import base_table_ref;
import hybridsearch_exception;
import third_party;
import logger;

namespace hybridsearch {

void IndexSecondary::ValidateColumnDataType(const SharedPtr<BaseTableRef> &base_table_ref, const String &column_name) {
    auto &column_names_vector = *(base_table_ref->column_names_);
    auto &column_types_vector = *(base_table_ref->column_types_);
    SizeT column_id = std::find(column_names_vector.begin(), column_names_vector.end(), column_name) - column_names_vector.begin();
    if (column_id == column_names_vector.size()) {
        Status status = Status::ColumnNotExist(column_name);
        RecoverableError(status);
    } else if (auto &data_type = column_types_vector[column_id]; !(data_type->CanBuildSecondaryIndex())) {
        Status status =
            Status::InvalidIndexDefinition(fmt::format("Attempt to create index on column: {}, data type: {}.", column_name, data_type->ToString()));
        RecoverableError(status);
    }
}

String IndexSecondary::BuildOtherParamsString() const { return ""; }

} // namespace hybridsearch