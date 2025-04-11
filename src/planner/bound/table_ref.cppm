

module;

import stl;

import table_reference;

export module table_ref;

namespace hybridsearch {

export class TableRef {
public:
    explicit TableRef(TableRefType type, String alias) : type_(type), alias_(std::move(alias)) {}
    virtual ~TableRef() = default;

    [[nodiscard]] inline TableRefType type() const { return type_; }
    [[nodiscard]] inline const String &alias() const { return alias_; }

    TableRefType type_{TableRefType::kTable};
    String alias_{};
};

} // namespace hybridsearch
