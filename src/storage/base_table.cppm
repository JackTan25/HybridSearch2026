

module;

export module base_table;

import stl;
import table_entry_type;
import logger;
import hybridsearch_exception;
import global_resource_usage;

namespace hybridsearch {

enum class BaseTableType {
    kInvalid,
    kTable,
    kCollection,
};

inline String ToString(BaseTableType type) {
    switch (type) {
        case BaseTableType::kTable: {
            return "Table";
        }
        case BaseTableType::kCollection: {
            return "Collection";
        }
        default: {
            String error_message = "Invalid base table type";
            UnrecoverableError(error_message);
        }
    }

    return String();
}

export class BaseTable {
public:
    explicit BaseTable(TableEntryType kind, SharedPtr<String> schema_name, SharedPtr<String> table_name)
        : kind_(kind), schema_name_(std::move(schema_name)), table_name_(std::move(table_name)) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("BaseTable");
#endif
    }

    BaseTable(const BaseTable &other_table) = delete;
    BaseTable(BaseTable &&other) = delete;

    virtual ~BaseTable() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("BaseTable");
#endif
    }

    [[nodiscard]] inline TableEntryType kind() const { return kind_; }

    TableEntryType kind_{TableEntryType::kTableEntry};

    [[nodiscard]] inline const SharedPtr<String> &schema_name() const { return schema_name_; }

    [[nodiscard]] inline const SharedPtr<String> &table_name() const { return table_name_; }

    const SharedPtr<String> schema_name_{};
    const SharedPtr<String> table_name_{};
};

} // namespace hybridsearch
