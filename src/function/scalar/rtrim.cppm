module;

import stl;

export module rtrim;

namespace hybridsearch {

class Catalog;

export void RegisterRtrimFunction(const UniquePtr<Catalog> &catalog_ptr);

} // namespace hybridsearch