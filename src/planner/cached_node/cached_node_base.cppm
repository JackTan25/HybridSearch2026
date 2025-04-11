

module;

export module cached_node_base;

import stl;
import logical_node_type;

namespace hybridsearch {

export class CachedNodeBase {
public:
    CachedNodeBase(LogicalNodeType type, SharedPtr<Vector<String>> output_names) : type_(type), output_names_(output_names) {}

    virtual ~CachedNodeBase() = default;

    virtual u64 Hash() const {
        u64 h = 0;
        h ^= std::hash<LogicalNodeType>{}(type_);
        return h;
    }

    virtual bool Eq(const CachedNodeBase &other) const { return type_ == other.type_; }

    LogicalNodeType type() const { return type_; }

    SharedPtr<Vector<String>> output_names() const { return output_names_; }

protected:
    LogicalNodeType type_;
    SharedPtr<Vector<String>> output_names_;
};

} // namespace hybridsearch