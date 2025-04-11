

module;

export module function_set;

import stl;

import function;
import base_expression;
import function_expr;
//

namespace hybridsearch {

struct Catalog;

export class FunctionSet {
public:
    static SharedPtr<FunctionSet> GetFunctionSet(Catalog *catalog, const FunctionExpr &expr);

    static String ToString(const String &name, const Vector<SharedPtr<BaseExpression>> &arguments);

public:
    explicit FunctionSet(String name, FunctionType type) : name_(std::move(name)), type_(type) {}

    virtual ~FunctionSet() = default;

    [[nodiscard]] const String &name() const { return name_; }

    FunctionType type() const { return type_; }

    String name_;
    FunctionType type_;
};

} // namespace hybridsearch
