

module;

import stl;

export module plus;

namespace hybridsearch {

class Catalog;

export void RegisterPlusFunction(const UniquePtr<Catalog> &catalog_ptr);
export void AppendRegisterPlusFunction(const UniquePtr<Catalog> &catalog_ptr);

} // namespace hybridsearch
