

module;

export module function;

import stl;

namespace hybridsearch {

export enum class FunctionType {
    kInvalid,
    kScalar,
    kAggregate,
    kTable,
    kWindow,
    kSpecial,
};

export class Function {
public:
    explicit Function(String name, FunctionType type) : name_(std::move(name)), type_(type) {}

    virtual ~Function() = default;

    [[nodiscard]] inline FunctionType type() const { return type_; }

    [[nodiscard]] inline const String &name() const { return name_; }

    [[nodiscard]] virtual String ToString() const = 0;

protected:
    String name_;
    FunctionType type_{FunctionType::kInvalid};
};

} // namespace hybridsearch
