

module;

import stl;

export module builtin_functions;

namespace hybridsearch {

class Catalog;

export class BuiltinFunctions : public EnableSharedFromThis<BuiltinFunctions> {
public:
    explicit BuiltinFunctions(UniquePtr<Catalog> &catalog_ptr);

    void Init();

private:
    UniquePtr<Catalog> &catalog_ptr_;

private:
    void RegisterAggregateFunction();

    void RegisterScalarFunction();

    void RegisterTableFunction();

    void RegisterSpecialFunction();
};

} // namespace hybridsearch
