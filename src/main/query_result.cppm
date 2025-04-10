

export module query_result;

import stl;
import data_table;
import status;
import logical_node_type;
import global_resource_usage;

namespace hybridsearch {

export struct BaseResult {
public:
    BaseResult() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("BaseResult");
#endif
    }

    ~BaseResult() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("BaseResult");
#endif
    }

    BaseResult(BaseResult &other) : status_(other.status_), result_table_(other.result_table_) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("BaseResult");
#endif
    }

    BaseResult(BaseResult &&other) : status_(std::move(other.status_)), result_table_(std::move(other.result_table_)) {}

    BaseResult &operator=(BaseResult &&other) noexcept {
        status_ = std::move(other.status_);
        result_table_ = std::move(other.result_table_);
        return *this;
    }

    [[nodiscard]] inline bool IsOk() const { return status_.ok(); }
    [[nodiscard]] inline ErrorCode ErrorCode() const { return status_.code(); }
    [[nodiscard]] inline DataTable *ResultTable() const { return result_table_.get(); }
    [[nodiscard]] inline const char *ErrorMsg() const { return status_.message(); }
    [[nodiscard]] inline String &ErrorStr() const { return *status_.msg_; }

public:
    Status status_{};
    SharedPtr<DataTable> result_table_{};
};

export struct QueryResult : public BaseResult {
    LogicalNodeType root_operator_type_{LogicalNodeType::kInvalid};
    String ToString() const;

    static QueryResult UnusedResult() { return {}; }
};

} // namespace hybridsearch
