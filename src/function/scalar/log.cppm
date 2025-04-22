module;

export module log;

import stl;

namespace hybridsearch {

class Catalog;

export void RegisterLogFunction(const UniquePtr<Catalog> &catalog_ptr);

} // namespace hybridsearch