module;

import stl;

export module regex;

namespace hybridsearch {

class Catalog;

export void RegisterRegexFunction(const UniquePtr<Catalog> &catalog_ptr);

} // namespace hybridsearch