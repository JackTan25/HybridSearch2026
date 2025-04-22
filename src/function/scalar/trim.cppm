module;

import stl;

export module trim;

namespace hybridsearch {

class Catalog;

export void RegisterTrimFunction(const UniquePtr<Catalog> &catalog_ptr);

} // namespace hybridsearch