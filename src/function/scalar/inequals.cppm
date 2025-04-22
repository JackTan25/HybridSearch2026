

module;

export module inequals;

import stl;

namespace hybridsearch {

class Catalog;

export void RegisterInEqualFunction(const UniquePtr<Catalog> &catalog_ptr);
export void RegisterInEqualAliasFunction(const UniquePtr<Catalog> &catalog_ptr);

} // namespace hybridsearch
