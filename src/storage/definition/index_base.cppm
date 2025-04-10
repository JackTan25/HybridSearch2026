

module;

export module index_base;

import stl;
import third_party;
import create_index_info;
import global_resource_usage;

namespace hybridsearch {

// TODO shenyushi: use definition in knn_exprs.h
export enum class MetricType {
    kMetricCosine,
    kMetricInnerProduct,
    kMetricL2,
    kInvalid,
};

export String MetricTypeToString(MetricType metric_type);

export MetricType StringToMetricType(const String &str);

export class IndexBase {
protected:
    explicit IndexBase(IndexType index_type,
                       SharedPtr<String> index_name,
                       SharedPtr<String> index_comment,
                       const String &file_name,
                       Vector<String> column_names)
        : index_type_(index_type), index_name_(std::move(index_name)), index_comment_(std::move(index_comment)), file_name_(file_name),
          column_names_(std::move(column_names)) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("IndexBase");
#endif
    }

public:
    explicit IndexBase(SharedPtr<String> index_name) : index_name_(std::move(index_name)) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("IndexBase");
#endif
    }

    IndexBase(const IndexBase &other)
        : index_type_(other.index_type_), index_name_(other.index_name_), index_comment_(other.index_comment_), file_name_(other.file_name_),
          column_names_(other.column_names_) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("IndexBase");
#endif
    }

    virtual ~IndexBase() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("IndexBase");
#endif
    }

    bool operator==(const IndexBase &other) const;

    bool operator!=(const IndexBase &other) const;

public:
    // Estimated serialized size in bytes, ensured be no less than Write requires, allowed be larger.
    virtual i32 GetSizeInBytes() const;

    // Write to a char buffer
    virtual void WriteAdv(char *&ptr) const;

    // Read char from buffer
    static SharedPtr<IndexBase> ReadAdv(const char *&ptr, i32 maxbytes);

    virtual String ToString() const;
    virtual String BuildOtherParamsString() const { return ""; }
    virtual nlohmann::json Serialize() const;

    static SharedPtr<IndexBase> Deserialize(const nlohmann::json &index_def_json);

    inline String column_name() const { return column_names_[0]; }

public:
    IndexType index_type_{IndexType::kInvalid};
    SharedPtr<String> index_name_{};
    SharedPtr<String> index_comment_{};
    const String file_name_{};
    const Vector<String> column_names_{};
};
} // namespace hybridsearch
