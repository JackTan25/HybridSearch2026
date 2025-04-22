module;

import stl;

export module lower;

namespace hybridsearch {

class Catalog;

export void RegisterLowerFunction(const UniquePtr<Catalog> &catalog_ptr);

} // namespace hybridsearch